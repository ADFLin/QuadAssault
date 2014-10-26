#include "LaserBullet.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "Explosion.h"
#include "RenderUtility.h"

IMPL_OBJECT_CLASS( LaserBullet , OT_BULLET , "Bullet.Laser" )

void LaserBullet::init()
{
	BaseClass::init();
	mSpeed   = 910;
	mLifeTime= 0.6;
	mDamage=4;
}

void LaserBullet::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );

	mLight.host = this;
	mLight.radius = 128;
	mLight.setColorParam(Vec3f(0.5, 1.0, 0.2),12);
	getLevel()->addLight( mLight );

	getLevel()->playSound("laser1.wav");		
	
}

void LaserBullet::tick()
{
	BaseClass::tick();
}

void LaserBullet::onDestroy( unsigned flag )
{
	mLight.remove();
	if ( flag & SDF_CAST_EFFECT )
	{
		Explosion* e = getLevel()->createExplosion( getPos() , 128 );
		e->setParam(8,100,80);
		e->setColor(Vec3f(1.0, 0.75, 0.5));	
	}
	BaseClass::onDestroy( flag );
}

