#include "Explosion.h"
#include "Level.h"

#include "Light.h"

Explosion::Explosion()
{

}

Explosion::~Explosion()
{

}

void Explosion::Init( Vec2f poz, float radius )
{
	setPos( poz );
	this->radius=radius;

	mbDead=false;

	intensity=0;
	maxIntenzitet=4;
	growthRate=35;
	deathRate=10;

	color=Vec3(1.0, 0.75, 0.5);

}

void Explosion::onSpawn()
{
	BaseClass::onSpawn();
	light = getLevel()->createLight( getPos() , radius , false);
	light->setColorParam(color, 0);	
	light->isExplosion = true;
}

void Explosion::onDestroy()
{
	light->destroy();
	BaseClass::onDestroy();
}

void Explosion::setParam(float intensity, float brzinaRasta, float brzinaUmiranja)
{
	maxIntenzitet=intensity*2;
	this->growthRate=brzinaRasta;
	this->deathRate=brzinaUmiranja;
}

void Explosion::setColor( Vec3 const& c )
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
	light->setColorParam(color, intensity);	
}

