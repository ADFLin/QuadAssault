#include "MinigunBullet.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "Explosion.h"
#include "Light.h"

class MinigunBulletRenderer : public IRenderer
{
public:
	virtual void init()
	{
		texG = getGame()->getTextureMgr()->getTexture("minigun1Glow.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		if( pass !=RP_GLOW )
			return;

		MinigunBullet* bullet = object->cast< MinigunBullet >();

		float rot= atan2( bullet->dir.y, bullet->dir.x ) + Math::toRad( 90 );
		drawSprite( object->getPos() - Vec2f(8,16) , Vec2f(16,32) , rot , texG );

	}
	Texture* texG;
};


DEFINE_RENDERER( MinigunBullet , MinigunBulletRenderer )

void MinigunBullet::init(Vec2f const& poz, Vec2f const& dir, int team )
{
	Bullet::init(poz,dir,team );
	speed=1000;
	domet=500;
	mDamage=0.5;
}

void MinigunBullet::onSpawn()
{
	BaseClass::onSpawn();

	light->init(mPos,128);
	light->setColorParam(Vec3(1.0, 1.0, 0.1),12);

	getLevel()->playSound("minigun1.wav");		

}

void MinigunBullet::tick()
{
	Bullet::tick();
}


void MinigunBullet::onDestroy()
{
	light->destroy();
	Explosion* e= getLevel()->createExplosion(mPos,64);
	e->setParam(4,100,80);
	e->setColor(Vec3(1.0, 0.75, 0.5));	
}
