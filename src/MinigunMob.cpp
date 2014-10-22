#include "MinigunMob.h"

#include "Level.h"
#include "Player.h"

#include "MinigunBullet.h"


void MinigunMob::init()
{
	BaseClass::init();

	mSize.x=64;
	mSize.y=64;
	
	maxbrzina=250;	
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
	Vec2f dir;
	dir= getLevel()->getPlayer()->getPos()- getPos();
	if( dir.length2()< 300 * 300 )
	{
		brzina-=100* TICK_TIME;
		if(brzina<0)
			brzina=0;
	}
	else
	{
		brzina+=100* TICK_TIME;
		if(brzina>maxbrzina)
			brzina=maxbrzina;
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
