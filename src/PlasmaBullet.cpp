#include "PlasmaBullet.h"

#include "GameInterface.h"
#include "Level.h"
#include "SmokeParticle.h"
#include "Explosion.h"
#include "RenderUtility.h"


void PlasmaBullet::init()
{
	BaseClass::init();
	mSpeed = 800;
	mLifeTime = 0.8;
	mDamage=12;

	mSize = Vec2f(1,1);
	dimTimer=0.0;
}

void PlasmaBullet::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );

	mLight.host   = this;
	mLight.radius = 256;
	mLight.setColorParam(Vec3f(0.25, 0.5, 1.0),18);
	getLevel()->addLight( mLight );

	getLevel()->playSound("plazma1.wav");	

}
void PlasmaBullet::onDestroy( unsigned flag )
{		
	mLight.remove();

	if ( flag & SDF_CAST_EFFECT )
	{
		Explosion* e = getLevel()->createExplosion( getPos() , 256 );	
		e->setParam(20,1000,50);
		e->setColor(Vec3f(1.0, 0.75, 0.5));

		getLevel()->playSound("explosion1.wav");	
	}

	BaseClass::onDestroy( flag );
}

void PlasmaBullet::tick()
{
	BaseClass::tick();

	dimTimer += TICK_TIME * 750;
	if(dimTimer>=10.0)
	{
		SmokeParticle* p = new SmokeParticle( getPos() );
		p->init();
		getLevel()->addParticle( p );
		dimTimer=0.0;
	}
}
