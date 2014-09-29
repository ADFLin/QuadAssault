#include "PlasmaMob.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "PlasmaBullet.h"
#include "Player.h"

class PlasmaMobRenderer : public MobRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getGame()->getTextureMgr();
		mTextures[ RP_DIFFUSE ] = texMgr->getTexture("mob1Diffuse.tga");
		mTextures[ RP_NORMAL  ] = texMgr->getTexture("mob1Normal.tga");
		mTextures[ RP_GLOW    ] = texMgr->getTexture("mob2Glow.tga");
	}
};

static PlasmaMobRenderer gRenderer;

void PlasmaMob::init(Vec2f const& poz )
{
	BaseClass::init(poz);

	mSize.x=64;
	mSize.y=64;
	
	brzina=75;
	maxbrzina=75;
	brzinaPunjenja=50;	
	domet=512;

	hp=60;

}

void PlasmaMob::onSpawn()
{
	BaseClass::onSpawn();
}

void PlasmaMob::tick()
{
	BaseClass::tick();
	
	shoot( BulletFactoryT< PlasmaBullet >() , TICK_TIME );	
	
	Vec2f dir;
	dir= getLevel()->getPlayer()->getCenterPos() - getCenterPos();
	if( dir.length2()<300 * 300 )
	{
		brzina-=100*TICK_TIME;
		if(brzina<0)
			brzina=0;
	}
	else
	{
		brzina+=100*TICK_TIME;
		if(brzina>maxbrzina)
			brzina=maxbrzina;
	}
}

void PlasmaMob::onDestroy()
{
	getLevel()->playSound("explozija1.wav");
	BaseClass::onDestroy();
}

void PlasmaMob::takeDamage(Bullet* bullet)
{
	BaseClass::takeDamage(bullet);
}

IRenderer* PlasmaMob::getRenderer()
{
	return &gRenderer;
}
