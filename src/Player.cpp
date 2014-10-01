#include "Player.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "SoundManager.h"

#include "Light.h"
#include "Bullet.h"
#include "Mob.h"
#include "Explosion.h"

#include "RenderUtility.h"
#include "Texture.h"

bool gPlayerGodPower = true;

Vec2f const gWeaponSlotOffset[] = 
{
	Vec2f(-9,-20) ,
	Vec2f( 9,-20) , 
	Vec2f(-24,0 ) ,
	Vec2f( 24,0 ) ,
};

class PlayerRenderer : public IRenderer
{
public:
	virtual void init()
	{

		TextureManager* texMgr = getGame()->getTextureMgr();
		textura = texMgr->getTexture("tenkTorzoDiffuse.tga");	
		texturaN = texMgr->getTexture("tenkTorzoNormal.tga");

		podloga_tex = texMgr->getTexture("tenkPodlogaDiffuse.tga");
		podloga_normal = texMgr->getTexture("tenkPodlogaNormal.tga");

		tracnica_tex = texMgr->getTexture("tracnicaDiffuse.tga");	
		tracnica_normal = texMgr->getTexture("tracnicaNormal.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		Player* player = object->cast< Player >();

		if( player->mIsDead )
			return;

		switch( pass )
		{
		case RP_DIFFUSE:
			//Shadow
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPushMatrix();
			glTranslatef(4,4,0);
			glColor4f( 0.0, 0.0, 0.0, 0.5 );	
			RenderPodlogu( pass , player );
			RenderTrack( pass , player );
			RenderTorzo( pass , player );	
			glColor4f(1.0, 1.0, 1.0, 1.0);
			glPopMatrix();
			glDisable(GL_BLEND);
			//
		case RP_NORMAL:
			glColor3f( 1.0, 1.0, 1.0 );	
			RenderPodlogu( pass , player );
			RenderTrack( pass , player );
			RenderTorzo( pass , player );	
			glColor3f(1.0, 1.0, 1.0 );
			break;
		}


		for(int i=0; i<4; i++)
		{
			Weapon* weapon = player->mWeaponSlot[i];
			if( weapon )
			{
				Vec2f centerPos = player->getPos();

				glPushMatrix();
				glTranslatef( centerPos.x,centerPos.y,0 );
				glRotatef( Math::toDeg( player->rotationAim ) + 90 ,0,0,1 );
				weapon->render( pass );
				glPopMatrix();
			}
		}

		if ( pass == RP_GLOW )
		{
			Vec2f size = player->getSize();
			glPushMatrix();
			glTranslatef( player->getRenderPos().x , player->getRenderPos().y , 0 );
			glColor3f( 0 , 1 , 0 );
			glBegin( GL_LINE_LOOP );
			glVertex3f( 0 , 0 , 0 );
			glVertex3f( size.x , 0 , 0 );
			glVertex3f( size.x , size.y , 0 );
			glVertex3f( 0 , size.y , 0 );
			glEnd();
			glColor3f(1.0, 1.0, 1.0 );
			glPopMatrix();
		}
	}

	void RenderPodlogu( RenderPass pass , Player* player )
	{
		Texture* tex;
		if(pass==RP_DIFFUSE)
			tex=podloga_tex;
		if(pass==RP_NORMAL)
			tex=podloga_normal;

		glPushMatrix();	
		drawSprite( player->getRenderPos() , player->getSize() , player->getRotation() ,tex);	
		glPopMatrix();
	}
	void RenderTorzo(RenderPass pass , Player* player )
	{
		Texture* tex;	
		if(pass==RP_DIFFUSE)
			tex = textura;	
		if(pass==RP_NORMAL)
			tex = texturaN;	
		glPushMatrix();	
		drawSprite( player->getRenderPos() , player->getSize(), player->rotationAim + Math::toRad(90) , tex );			
		glPopMatrix();	
	}

	void RenderTrack( RenderPass pass , Player* player )
	{
		float razmak_tracnica=8;
		float odmak=24;

		Texture* tex;
		if(pass==RP_DIFFUSE)
			tex=tracnica_tex;
		if(pass==RP_NORMAL)
			tex=tracnica_normal;

		glEnable(GL_TEXTURE_2D);

		tex->bind();

		Vec2f centerPos = player->getPos();
		float shift = player->shiftTrack;
		Vec2f size = player->getSize();

		glPushMatrix();
		glTranslatef( centerPos.x, centerPos.y , 0 );
		glRotatef(Math::toDeg( player->getRotation()),0,0,1);
		glTranslatef( -odmak - razmak_tracnica, -size.y/2,0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, shift); glVertex2f(0.0, 0.0);
		glTexCoord2f(1.0, shift); glVertex2f(size.x/4, 0.0);
		glTexCoord2f(1.0, shift + 1.0); glVertex2f(size.x/4, size.y);	
		glTexCoord2f(0.0, shift + 1.0); glVertex2f(0.0, size.y);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef( centerPos.x, centerPos.y , 0 );
		glRotatef(Math::toDeg( player->getRotation()),0,0,1);
		glTranslatef( odmak - razmak_tracnica, -size.y/2,0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, shift); glVertex2f(0.0, 0.0);
		glTexCoord2f(1.0, shift); glVertex2f(size.x/4, 0.0);
		glTexCoord2f(1.0, shift + 1.0); glVertex2f(size.x/4, size.y);	
		glTexCoord2f(0.0, shift + 1.0); glVertex2f(0.0, size.y);
		glEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}

	Texture* textura;
	Texture* texturaN;
	Texture* tracnica_tex;
	Texture* tracnica_normal;
	Texture* podloga_tex;
	Texture* podloga_normal;

};

DEFINE_RENDERER( Player , PlayerRenderer )

Player::Player()
{
	for(int i=0; i<4; i++)
		mWeaponSlot[i] = NULL;
}
Player::~Player()
{
	
}

void Player::Init(Vec2f poz)
{
	setPos( poz );
	setSize( Vec2f(64,64));

	akceleracija = 0;

	for(int i=0; i<4; i++)
		mWeaponSlot[i]=NULL;

	mHP=100;
	mEnergy=100.0;
	haveShoot=false;
	mIsDead=false;

	shiftTrack=0.0;
	rotation=0.0;

	brzina=200;
}

void Player::onSpawn()
{
	BaseClass::onSpawn();

	light = getLevel()->createLight( Vec2f(0.0, 0.0), 1024 , false );
	light->setColorParam(Vec3(1.0, 1.0, 1.0), 16);
	light->drawShadow = true;
}

void Player::onDestroy()
{
	clearWeapons();
}

void Player::update( Vec2f const& aimPos )
{
	if( !mIsDead )
	{
		Vec2f moment;
		float rad = rotation - Math::toRad( 90 );
		moment.x=cos(rad)*akceleracija;
		moment.y=sin(rad)*akceleracija;	

		Vec2f offset = ( brzina * TICK_TIME ) * moment;
		mPos.y += offset.y;
		if(checkCollision())
			mPos.y -= offset.y;
		mPos.x += offset.x;
		if(checkCollision())
			mPos.x -= offset.x;

		if(moment.x!=0 || moment.y!=0)
		{
			if(akceleracija>0)
				shiftTrack += TICK_TIME;
			else
				shiftTrack -= TICK_TIME;
			if(shiftTrack>1.0)
				shiftTrack=0.0;
			if(shiftTrack<0.0)
				shiftTrack=1.0;
		}
		akceleracija=0;

		SudarProjektila();

		Vec2f dir = aimPos - getPos();
		rotationAim = Math::atan2( dir.y , dir.x );

		for(int i=0; i< NUM_WEAPON_SLOT ; i++)
		{
			if( mWeaponSlot[i] )
				mWeaponSlot[i]->tick();
		}

		updateHeadlight();

		if( !haveShoot )
		{
			mEnergy += 10* TICK_TIME;
			if(mEnergy>100.0)
				mEnergy=100.0;		
		}
		haveShoot=false;

		if(mHP<=0.0)
		{
			mHP=0.0;
			mIsDead=true;

			Explosion* e= getLevel()->createExplosion( getPos(),512 );
			e->setParam(256,3000,200);

			getLevel()->playSound("explosion1.wav");		

			Message* gameOverMsg = new Message();
			gameOverMsg->init("Base", "All units lost, mission Failed." , 4, "blip.wav" );
			getLevel()->addMessage( gameOverMsg );

			light->setColorParam(Vec3(0,0,0), 0);
		}
	}
}
void Player::updateHeadlight()
{
	Vec2f dir;
	float angle = rotationAim + Math::toRad(180);
	dir.x = cos( angle );
	dir.y = sin( angle );
	Math::normalize( dir );	
	light->PostavkeKuta(dir,2);

	Vec2f offset;
	offset.x=cos( rotationAim );
	offset.y=sin( rotationAim );
	light->setPos(getPos()+ 34 * offset);
}

void Player::DodajMoment(float x)
{
	akceleracija=x;
}


void Player::shoot( Vec2f const& posTaget )
{
	if( mIsDead )
		return;

	Vec2f dir = posTaget - getPos();
	Math::normalize( dir );

	for(int i=0; i<4; i++)
	{		
		if( !mWeaponSlot[i] )
			continue;

		if( mEnergy >= mWeaponSlot[i]->getEnergyCast() )
		{
			Vec2f offset = mWeaponSlot[i]->getPos();

			float angle = rotationAim + Math::atan2(offset.y,offset.x) + Math::toRad( 90 );

			Vec2f slotDir;
			slotDir.x = cos( angle );
			slotDir.y = sin( angle );
			mWeaponSlot[i]->fire( getPos() + sqrt( offset.length2() ) * slotDir , dir , TEAM_PLAYER );
			haveShoot=true;
		}
	}

}
bool Player::checkCollision()
{
	Rect bBox;
	calcBoundBox( bBox );
	bBox.min += Vec2f(4,4);
	bBox.max -= Vec2f(4,4);

	TileMap& terrian = getLevel()->getTerrain();

	if ( getLevel()->testTerrainCollision( bBox , BF_MOVABLE ) )
		return true;

	for( MobList::iterator iter = getLevel()->getMobs().begin() , itEnd = getLevel()->getMobs().end();
		iter != itEnd ; ++iter )
	{
		Mob* mob = *iter;

		Rect bBoxOther;
		mob->calcBoundBox( bBoxOther );
		if( bBox.intersect(bBoxOther) )
			return true;		
	}

	return false;
}
void Player::SudarProjektila()
{
	Rect bBox;
	calcBoundBox( bBox );
	bBox.min += Vec2f(4,4);
	bBox.max -= Vec2f(4,4);

	for( BulletList::iterator iter = getLevel()->getBullets().begin() , itEnd = getLevel()->getBullets().end();
		iter != itEnd ; ++iter )
	{
		Bullet* bt = *iter;
		if( bt->team == TEAM_EMPTY )
		{
			Rect bBoxOther;
			bt->calcBoundBox( bBoxOther );
			if( bBox.intersect(bBoxOther) )
			{
				takeDamage( bt );				
			}
		}
	}
}


void Player::takeDamage(Bullet* p)
{
	if( mIsDead )
		return;

	if ( !gPlayerGodPower )
	{
		mHP -= p->getDamage();
	}
	p->destroy();

}

void Player::addWeapon( Weapon* weapon )
{
	for(int i=0; i<4; i++ )
	{
		if( mWeaponSlot[i] )
			continue;

		weapon->init( this );
		weapon->setPos( gWeaponSlotOffset[i] );

		mWeaponSlot[i] = weapon;
		weapon = NULL;
		break;
	}

	if ( weapon )
	{
		delete weapon;
	}
}

bool Player::isDead()
{
	return mIsDead;
}

void Player::loseEnergy(float e)
{
	mEnergy-=e;
	if(mEnergy<0.0)
		mEnergy=0.0;
}
void Player::addHP(float kolicina)
{
	mHP+=kolicina;
	if(mHP>100.0)
		mHP=100.0;
}

void Player::clearWeapons()
{
	for(int i=0; i<4; i++)
	{
		if(mWeaponSlot[i])
		{
			delete mWeaponSlot[i];	
			mWeaponSlot[i]=NULL;
		}
	}
}
