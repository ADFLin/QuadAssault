#include "Player.h"

#include "Level.h"
#include "SoundManager.h"

#include "Light.h"
#include "Bullet.h"
#include "Mob.h"
#include "Explosion.h"

#include "Message.h"
#include "RenderUtility.h"

bool gPlayerGodPower = true;

IMPL_OBJECT_CLASS( Player , OT_PLAYER , "Player" )

Vec2f const gWeaponSlotOffset[] = 
{
	Vec2f(-9,-20) ,
	Vec2f( 9,-20) , 
	Vec2f(-24,0 ) ,
	Vec2f( 24,0 ) ,
};


Player::Player()
{
	mPlayerId = -1;
}


void Player::init()
{
	setSize( Vec2f(64,64) );

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

	mBody.setSize( getSize() - Vec2f(4,4) );
	mBody.setTypeMask( COL_SOILD | COL_PLAYER | COL_RENDER );
	mBody.setColMask( COL_TERRAIN | COL_OBJECT );
}

void Player::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );

	getLevel()->getColManager().addBody( *this , mBody );

	mHeadLight.radius = 1024;
	mHeadLight.host = this;
	mHeadLight.angle  = 2;
	mHeadLight.setColorParam(Vec3f(1.0, 1.0, 1.0), 16);
	mHeadLight.drawShadow = true;
	getLevel()->addLight( mHeadLight );
}

void Player::onDestroy( unsigned flag )
{
	getLevel()->getColManager().removeBody( mBody );
	clearWeapons();
	BaseClass::onDestroy( flag );
}

void Player::update( Vec2f const& aimPos )
{
	if( !mIsDead )
	{
		Vec2f moment;
		float rad = rotation - Math::toRad( 90 );
		moment.x=cos(rad)*akceleracija;
		moment.y=sin(rad)*akceleracija;	

		Vec2f off = ( brzina * TICK_TIME ) * moment;

		Vec2f offset = Vec2f( 0 , 0 );

		offset.y += off.y;
		if( testCollision( offset ) )
			offset.y = 0;
		offset.x += off.x;
		if( testCollision( offset ) )
			offset.x = 0;

		mPos += offset;

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

		if( mHP<=0.0 )
		{
			mHP=0.0;
		
			Explosion* e= getLevel()->createExplosion( getPos(),512 );
			e->setParam(256,3000,200);

			getLevel()->playSound("explosion1.wav");		

			Message* gameOverMsg = new Message();
			gameOverMsg->init("Base", "All units lost, mission Failed." , 4, "blip.wav" );
			getLevel()->addMessage( gameOverMsg );

			mHeadLight.setColorParam(Vec3f(0,0,0), 0);

			mIsDead = true;

			LevelEvent event;
			event.id = LevelEvent::ePLAYER_DEAD;
			getLevel()->sendEvent( event );
		}
	}
}

void Player::onBodyCollision( ColBody& self , ColBody& other )
{
	LevelObject* obj = other.getClient();
	switch( obj->getType() )
	{
	case OT_BULLET:
		{
			Bullet* bullet = obj->cast< Bullet >();
			if ( bullet->team == TEAM_EMPTY )
			{
				takeDamage( bullet );
			}
		}
		break;
	}
}

void Player::updateHeadlight()
{
	Vec2f dir;
	float angle = rotationAim + Math::toRad(180);
	dir.x = cos( angle );
	dir.y = sin( angle );
	Math::normalize( dir );	
	mHeadLight.dir = dir;

	float dist = 34;
	mHeadLight.offset.x = dist * cos( rotationAim );
	mHeadLight.offset.y = dist * sin( rotationAim );
}

void Player::addMoment(float x)
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

bool Player::testCollision( Vec2f const& offset )
{
	ColInfo info;
	return getLevel()->getColManager().testCollision( info , offset , mBody , COL_SOILD | COL_TERRAIN );
}

void Player::updateEdit()
{
	BaseClass::updateEdit();
	updateHeadlight();
}
