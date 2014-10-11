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

}


void LightObject::tick()
{

}

void LightObject::enumProp( IPropEditor& editor )
{
	BaseClass::enumProp( editor );
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

