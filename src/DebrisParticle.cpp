#include "DebrisParticle.h"

#include "Level.h"
#include "RenderUtility.h"

IMPL_OBJECT_CLASS( DebrisParticle , OT_PARTICLE , "Particle.Debris" )

DebrisParticle::DebrisParticle( Vec2f const& pos )
	:BaseClass( pos )
{

}


void DebrisParticle::init()
{
	BaseClass::init();
	maxZivot=45;
	zivot=maxZivot;	
}

void DebrisParticle::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );

}

void DebrisParticle::tick()
{
	BaseClass::tick();
}
