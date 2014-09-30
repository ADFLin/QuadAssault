#include "PlasmaBullet.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "Smoke.h"
#include "Explosion.h"
#include "Light.h"


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

void PlasmaBullet::init(Vec2f const& poz, Vec2f const& dir, int team )
{
	Bullet::init(poz,dir,team );
	speed=800;
	domet=800;
	mDamage=12;

	mSize = Vec2f(1,1);

	dimTimer=0.0;
}

void PlasmaBullet::onSpawn()
{
	Bullet::onSpawn();

	light->init( getPos() , 256 );
	light->setColorParam(Vec3(0.25, 0.5, 1.0),18);	

	getLevel()->playSound("plazma1.wav");	

}
void PlasmaBullet::tick()
{
	Bullet::tick();
	dimTimer += TICK_TIME * 750;
	if(dimTimer>=10.0)
	{
		Particle* c= getLevel()->addParticle(new Smoke());
		c->Init( getPos() );
		dimTimer=0.0;
	}
}

void PlasmaBullet::onDestroy()
{		
	light->destroy();
	Explosion* e = getLevel()->createExplosion( getRenderPos() , 256 );	
	e->setParam(20,1000,50);
	e->setColor(Vec3(1.0, 0.75, 0.5));

	getLevel()->playSound("explosion1.wav");		
}
