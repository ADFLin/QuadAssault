#include "MinigunMob.h"

#include "Level.h"
#include "Player.h"

#include "MinigunBullet.h"

IMPL_OBJECT_CLASS( MinigunMob , OT_MOB , "Mob.Minigun" )

void MinigunMob::init()
{
	BaseClass::init();

	mSize.x=64;
	mSize.y=64;
	
	mMaxSpeed=250;	
	brzinaPunjenja=750;	
	domet=512;

	mHP=35;

}

void MinigunMob::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );
	
}

void MinigunMob::onDestroy( unsigned flag )
{
	if ( flag & SDF_CAST_EFFECT )
	{
		getLevel()->playSound( "explosion1.wav" , true );	
	}
	BaseClass::onDestroy( flag );
}

void MinigunMob::tick()
{
	BaseClass::tick();

	shoot( IBulletFactory::Make< MinigunBullet >() );

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

void MinigunMob::shoot( IBulletFactory const& creator )
{	
	BaseClass::shoot( creator );	
}

void MinigunMob::takeDamage(Bullet* bullet )
{
	BaseClass::takeDamage(bullet);
}
