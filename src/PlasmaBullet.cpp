#include "PlasmaBullet.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "SmokeParticle.h"
#include "Explosion.h"
#include "RenderUtility.h"


class PlasmaBulletRenderer : public IRenderer
{
public:
	virtual void init()
	{
		tex = getGame()->getTextureMgr()->getTexture("granata1.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		if( pass !=RP_DIFFUSE )
			return ;

		drawSprite( object->getRenderPos() , object->getSize() ,0,tex);
	}
	Texture* tex;
};

DEFINE_RENDERER( PlasmaBullet , PlasmaBulletRenderer )

void PlasmaBullet::init()
{
	BaseClass::init();
	mSpeed = 800;
	mLifeTime = 0.8;
	mDamage=12;

	mSize = Vec2f(1,1);
	dimTimer=0.0;
}

void PlasmaBullet::onSpawn()
{
	BaseClass::onSpawn();

	mLight.host   = this;
	mLight.radius = 256;
	mLight.setColorParam(Vec3f(0.25, 0.5, 1.0),18);
	getLevel()->addLight( mLight );

	getLevel()->playSound("plazma1.wav");	

}
void PlasmaBullet::onDestroy()
{		
	mLight.remove();
	Explosion* e = getLevel()->createExplosion( getPos() , 256 );	
	e->setParam(20,1000,50);
	e->setColor(Vec3f(1.0, 0.75, 0.5));

	getLevel()->playSound("explosion1.wav");	

	BaseClass::onDestroy();
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
