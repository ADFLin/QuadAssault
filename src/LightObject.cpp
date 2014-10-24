#include "LightObject.h"

#include "Level.h"


LightObject::LightObject()
{

}

LightObject::~LightObject()
{	
	
}

void LightObject::init()
{
	setSize( Vec2f( 32 , 32 ) );
}


void LightObject::tick()
{

}

void LightObject::onSpawn( unsigned flag )
{
	host = this;
	getLevel()->addLight( *this );
}

void LightObject::onDestroy( unsigned flag )
{
	Light::remove();
}

void LightObject::setupDefault()
{
	BaseClass::setupDefault();

	radius = 128;
	color = Vec3f( 1, 1 ,1 );
	intensity = 8;
}

