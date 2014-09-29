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
	mPos.x=0;
	mPos.y=0;
	mSize.x=16;
	mSize.y=32;

	mCDTime=100;
	mFireTimer=mCDTime;
	mCDSpeed=250;	
	mEnergyCast=1;
}

void Weapon::update(float deltaT)
{	
	mFireTimer += mCDSpeed * deltaT;
	if( mFireTimer > mCDTime )
		mFireTimer = mCDTime;
}

void Weapon::fire( Vec2f const& pos, Vec2f const& dir, int team )
{
	if ( mFireTimer == mCDTime )
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
	float o=mFireTimer/mCDTime*8;

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.5f);
	glEnable(GL_TEXTURE_2D);

	mTextues[ pass ]->bind();

	glPushMatrix();
	glTranslatef(-mSize.x/2,-o-mSize.y/2,0);

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

