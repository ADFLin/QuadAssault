#include "Light.h"


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
	isExplosion = false;

	dir   = Vec2f(0.0, 0.0);
	angle = 0.0;
}

void Light::setColorParam( Vec3f const& color , float intensity)
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
	this->isExplosion = explozija;
}

void Light::tick()
{

}

