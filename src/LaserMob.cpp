#include "LaserMob.h"

#include "Level.h"
#include "Player.h"
#include "LaserBullet.h"

IMPL_OBJECT_CLASS( LaserMob , OT_MOB , "Mob.Laser" )

void LaserMob::init()
{
	BaseClass::init();

	mSize.x=64;
	mSize.y=64;
	
	maxbrzina=150;	
	brzinaPunjenja=250;	
	domet=512;

	mHP=40;
}

void LaserMob::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );
}

void LaserMob::onDestroy( unsigned flag )
{
	if ( flag & SDF_CAST_EFFECT )
	{
		getLevel()->playSound("explosion1.wav");	
	}	
	BaseClass::onDestroy( flag );
}

void LaserMob::tick()
{
	Mob::tick();

	shoot( IBulletFactory::Make< LaserBullet >() );

	Vec2f dir = getLevel()->getPlayer()->getPos()- getPos();
	
	if( dir.length2()<300 * 300 )
	{
		brzina-=100*TICK_TIME;
		if(brzina<0)
			brzina=0;
	}
	else
	{
		brzina+=100*TICK_TIME;
		if(brzina>maxbrzina)
			brzina=maxbrzina;
	}
}


void LaserMob::shoot( IBulletFactory const& creator )
{	
	BaseClass::shoot( creator  );	
}

void LaserMob::takeDamage(Bullet* bullet)
{
	BaseClass::takeDamage(bullet);
}