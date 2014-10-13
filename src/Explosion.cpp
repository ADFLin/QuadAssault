#include "Explosion.h"
#include "Level.h"

Explosion::Explosion()
{

}

Explosion::Explosion( Vec2f const& pos , float radius )
	:BaseClass( pos )
	,radius( radius )
{

}

Explosion::~Explosion()
{

}

void Explosion::init()
{
	BaseClass::init();

	mbDead=false;

	intensity=0;
	maxIntenzitet=4;
	growthRate=35;
	deathRate=10;

	color=Vec3f(1.0, 0.75, 0.5);
}

void Explosion::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );

	mLight.host   = this;
	mLight.radius = radius;
	mLight.setColorParam(color, 0);
	mLight.isExplosion = true;
	getLevel()->addLight( mLight );
}

void Explosion::onDestroy( unsigned flag )
{
	mLight.remove();
	BaseClass::onDestroy( flag );
}

void Explosion::setParam(float intensity, float brzinaRasta, float brzinaUmiranja)
{
	maxIntenzitet=intensity*2;
	this->growthRate=brzinaRasta;
	this->deathRate=brzinaUmiranja;
}

void Explosion::setColor( Vec3f const& c )
{
	this->color = c;
}

void Explosion::tick()
{

	if( mbDead )
	{
		if( intensity>0 )
			intensity -= deathRate * TICK_TIME;
		else
		{			
			destroy();
			intensity=0;
		}
	}
	else
	{
		if(intensity<maxIntenzitet)
			intensity += growthRate * TICK_TIME;
		else
		{
			mbDead=true;
			intensity=maxIntenzitet;
		}
	}	
	mLight.setColorParam(color, intensity);	
}

