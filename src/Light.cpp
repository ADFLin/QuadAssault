#include "Light.h"

#include "Shader.h"
#include "GameInterface.h"
#include "Level.h"

Light::Light()
{

}
Light::~Light()
{	

}

void Light::init(Vec2f poz, float radius )
{
	setPos( poz );

	this->radius = radius;

	drawShadow = false;
	explozija = false;

	dir   = Vec2f(0.0, 0.0);
	angle = 0.0;

	
}

void Light::setColorParam( Vec3 const& color , float intensity)
{
	this->color = color;
	this->intensity=intensity;
}

void Light::PostavkeKuta( Vec2f const& dir, float angle)
{
	this->dir=dir;
	this->angle=angle;
}

void Light::SetExplozija(bool explozija)
{
	this->explozija = explozija;
}

void Light::tick()
{

}

