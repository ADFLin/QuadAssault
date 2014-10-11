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

void LightObject::enumProp( IPropEditor& editor )
{
	BaseClass::enumProp( editor );
	editor.addProp( "Radius" , radius );
	editor.addProp( "Color" , color );
	editor.addProp( "Intensity" , intensity );
}

void LightObject::onSpawn()
{
	host = this;
	getLevel()->addLight( *this );
}

void LightObject::onDestroy()
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

