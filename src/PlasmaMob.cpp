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
	
	brzina=75;
	maxbrzina=75;
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
	
	Vec2f dir;
	dir= getLevel()->getPlayer()->getPos() - getPos();
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

