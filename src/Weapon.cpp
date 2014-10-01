#include "Weapon.h"

#include "Level.h"
#include "Bullet.h"
#include "Player.h"

#include "TextureManager.h"
#include "Texture.h"

void FireHelper::fire( Bullet* bullet , Vec2f const& offset )
{
	bullet->init( pos + offset , dir ,team );
	weapon->getOwner()->getLevel()->addBullet( bullet );
	weapon->onFireBullet( bullet );
}

void Weapon::init( Player* player )
{	
	mOwner = player;
	mSize = Vec2f( 16 , 32 );

	mCDTime    = 100;
	mFireTimer = mCDTime;
	mCDSpeed   = 250;	
	mEnergyCast = 1;
}

void Weapon::update(float deltaT)
{	
	mFireTimer += mCDSpeed * deltaT;
}

void Weapon::fire( Vec2f const& pos, Vec2f const& dir, int team )
{
	if ( mFireTimer >= mCDTime )
	{
		FireHelper helper;
		helper.pos  = pos;
		helper.dir  = dir;
		helper.team = team;
		helper.weapon = this;
		doFire( helper );

		mFireTimer = 0;
	}
}

void Weapon::onFireBullet( Bullet* p )
{
	mOwner->loseEnergy(mEnergyCast);
}

void Weapon::render( RenderPass pass )
{
	float factor = std::min( 1.0f , mFireTimer / mCDTime );

	Vec2f rPos = getPos() - mSize / 2;

	float off;
	float len = 20;
	if ( factor < 0.5 )
	{
		off = len * factor;
	}
	else
	{
		off = len * ( 1 - factor );
	}

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.5f);
	glEnable(GL_TEXTURE_2D);

	mTextues[ pass ]->bind();

	glPushMatrix();
	glTranslatef( rPos.x , rPos.y + off , 0 );

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(0,0);	
	glTexCoord2f(1.0, 0.0); glVertex2f(mSize.x,0);	
	glTexCoord2f(1.0, 1.0); glVertex2f(mSize.x,mSize.y);	
	glTexCoord2f(0.0, 1.0); glVertex2f(0,mSize.y);	
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
}

