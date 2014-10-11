#include "LaserBullet.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "Explosion.h"
#include "RenderUtility.h"

class LaserBulletRenderer : public IRenderer
{
public:
	virtual void init()
	{
		texG = getGame()->getTextureMgr()->getTexture("laser1Glow.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		if( pass !=RP_GLOW )
			return;

		LaserBullet* bullet = object->cast< LaserBullet >();

		Vec2f size = Vec2f(16,32);
		float rot= Math::atan2( bullet->mDir.y, bullet->mDir.x ) + Math::toRad( 90 );
		drawSprite( bullet->getPos() - size / 2 , size , rot , texG );		
	}
	Texture* texG;
};

DEFINE_RENDERER( LaserBullet , LaserBulletRenderer )

void LaserBullet::init()
{
	BaseClass::init();
	mSpeed   = 910;
	mLifeTime= 0.6;
	mDamage=4;
}

void LaserBullet::onSpawn()
{
	BaseClass::onSpawn();

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


void LaserBullet::onDestroy()
{
	mLight.remove();
	Explosion* e = getLevel()->createExplosion( getPos() , 128 );
	e->setParam(8,100,80);
	e->setColor(Vec3f(1.0, 0.75, 0.5));	
}

