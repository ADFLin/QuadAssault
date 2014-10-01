#include "Particle.h"

#include "Level.h"

Particle::Particle( Vec2f const& pos ) 
	:BaseClass( pos )
{

}

void Particle::init()
{
	zivot=100;
	maxZivot=zivot;	
}

void Particle::onSpawn()
{
	BaseClass::onSpawn();
	int xof= getLevel()->random(2,4)-2;
	int yof= getLevel()->random(2,4)-2;
	mSize = Vec2f(16+xof,16+yof);
	mPos += Vec2f(xof,yof);
}

void Particle::tick()
{
	BaseClass::tick();
	zivot -= TICK_TIME * 50;
	if(zivot<=0.0)
		destroy();
}
