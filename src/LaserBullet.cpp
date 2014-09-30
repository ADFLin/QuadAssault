#include "LaserBullet.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "Light.h"
#include "Explosion.h"

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
		float rot= atan2( bullet->dir.y, bullet->dir.x ) + Math::toRad( 90 );
		drawSprite( bullet->getPos() - size / 2 , size , rot , texG );		
	}
	Texture* texG;
};

DEFINE_RENDERER( LaserBullet , LaserBulletRenderer )

void LaserBullet::init(Vec2f const& poz, Vec2f const& dir , int team )
{
	BaseClass::init(poz,dir,team );
	speed=910;
	domet=600;
	mDamage=4;
}

void LaserBullet::onSpawn()
{
	BaseClass::onSpawn();
	light->init( getPos() , 128 );
	light->setColorParam(Vec3(0.5, 1.0, 0.2),12);

	getLevel()->playSound("laser1.wav");		
	
}

void LaserBullet::tick()
{
	BaseClass::tick();
}


void LaserBullet::onDestroy()
{
	light->destroy();
	Explosion* e = getLevel()->createExplosion( getPos() , 128 );
	e->setParam(8,100,80);
	e->setColor(Vec3(1.0, 0.75, 0.5));	
}

