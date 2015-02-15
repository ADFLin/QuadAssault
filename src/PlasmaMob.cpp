#include "PlasmaMob.h"

#include "Level.h"
#include "PlasmaBullet.h"
#include "Player.h"

IMPL_OBJECT_CLASS( PlasmaMob , OT_MOB , "Mob.Plasma" )

void PlasmaMob::init()
{
	BaseClass::init();

	mSize.x=64;
	mSize.y=64;
	
	mSpeed=75;
	mMaxSpeed=75;
	brzinaPunjenja=50;	
	domet=512;
	mHP=60;

}

void PlasmaMob::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );
}

void PlasmaMob::tick()
{
	BaseClass::tick();
	
	shoot( IBulletFactory::Make< PlasmaBullet >() );	
	
	if ( mTarget )
	{
		Vec2f dir = mPosLastView - getPos();
		if( dir.length2() < 300 * 300 )
		{
			mSpeed -= 100*TICK_TIME;
			if( mSpeed < 0 )
				mSpeed = 0;
		}
		else
		{
			mSpeed += 100*TICK_TIME;
			if( mSpeed > mMaxSpeed )
				mSpeed = mMaxSpeed;
		}
	}
}

void PlasmaMob::onDestroy( unsigned flag )
{
	if ( flag & SDF_CAST_EFFECT )
	{
		getLevel()->playSound("explosion1.wav");
	}
	BaseClass::onDestroy( flag );
}

void PlasmaMob::takeDamage(Bullet* bullet)
{
	BaseClass::takeDamage(bullet);
}

