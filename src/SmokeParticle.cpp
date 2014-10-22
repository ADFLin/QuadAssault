#include "SmokeParticle.h"

#include "Level.h"

SmokeParticle::SmokeParticle( Vec2f const& pos ) 
	:BaseClass( pos )
{

}


void SmokeParticle::init()
{
	BaseClass::init();
	maxZivot=45;
	zivot=maxZivot;
}

void SmokeParticle::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );

	mPos.x += getLevel()->random(8,16)-8;
	mPos.y += getLevel()->random(8,16)-8;
}

void SmokeParticle::tick()
{
	BaseClass::tick();
}
