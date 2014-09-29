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

		float rot= atan2( bullet->dir.y, bullet->dir.x ) + Math::toRad( 90 );
		drawSprite( object->getPos() - Vec2f(8,16) , Vec2f(16,32) , rot , texG );		
	}
	Texture* texG;
};

DEFINE_RENDERER( LaserBullet , LaserBulletRenderer )

void LaserBullet::init(Vec2f const& poz, Vec2f const& dir , int team )
{
	Bullet::init(poz,dir,team );
	speed=910;
	domet=600;
	mDamage=4;
}

void LaserBullet::onSpawn()
{
	Bullet::onSpawn();
	light->init(mPos,128);
	light->setColorParam(Vec3(0.5, 1.0, 0.2),12);

	getLevel()->playSound("laser1.wav");		
	
}

void LaserBullet::tick()
{
	Bullet::tick();
}


void LaserBullet::onDestroy()
{
	light->destroy();
	Explosion* e = getLevel()->createExplosion( mPos , 128 );
	e->setParam(8,100,80);
	e->setColor(Vec3(1.0, 0.75, 0.5));	
}

