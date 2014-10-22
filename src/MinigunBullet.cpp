#include "MinigunBullet.h"

#include "GameInterface.h"
#include "Level.h"

#include "Explosion.h"
#include "LightObject.h"
#include "RenderUtility.h"


void MinigunBullet::init()
{
	BaseClass::init();

	mSpeed    = 1000;
	mLifeTime = 0.5;
	mDamage   = 0.5;
}

void MinigunBullet::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );

	light.radius = 128;
	light.host   = this;
	light.setColorParam( Vec3f(1.0, 1.0, 0.1 ) , 12 );

	getLevel()->addLight( light );
	getLevel()->playSound("minigun1.wav");		
}

void MinigunBullet::onDestroy( unsigned flag )
{
	light.remove();

	if ( flag & SDF_CAST_EFFECT )
	{
		Explosion* e = getLevel()->createExplosion( getPos() , 64 );
		e->setParam(4,100,80);
		e->setColor(Vec3f(1.0, 0.75, 0.5));	
	}

	BaseClass::onDestroy( flag );
}

void MinigunBullet::tick()
{
	Bullet::tick();
}

