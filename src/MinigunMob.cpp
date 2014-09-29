#include "MinigunMob.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "Player.h"

#include "MinigunBullet.h"

class MinigunMobRenderer : public MobRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getGame()->getTextureMgr();
		mTextures[ RP_DIFFUSE ] = texMgr->getTexture("mob1Diffuse.tga");
		mTextures[ RP_NORMAL  ] = texMgr->getTexture("mob1Normal.tga");
		mTextures[ RP_GLOW    ] = texMgr->getTexture("mob3Glow.tga");
	}
};

static MinigunMobRenderer gRenderer;

void MinigunMob::init(Vec2f const& poz)
{
	BaseClass::init(poz);

	mSize.x=64;
	mSize.y=64;
	
	maxbrzina=250;	
	brzinaPunjenja=750;	
	domet=512;

	hp=35;

}

void MinigunMob::onSpawn()
{
	BaseClass::onSpawn();
	
}

void MinigunMob::onDestroy()
{
	getLevel()->playSound( "explozija1.wav" , true );		
	BaseClass::onDestroy();
}

void MinigunMob::tick()
{
	BaseClass::tick();
	shoot( BulletFactoryT< MinigunBullet >(), TICK_TIME );	
	Vec2f dir;
	dir= getLevel()->getPlayer()->getCenterPos()- getCenterPos();
	if( dir.length2()< 300 * 300 )
	{
		brzina-=100* TICK_TIME;
		if(brzina<0)
			brzina=0;
	}
	else
	{
		brzina+=100* TICK_TIME;
		if(brzina>maxbrzina)
			brzina=maxbrzina;
	}
}

void MinigunMob::shoot( IBulletFactory const& creator , float deltaT)
{	

	BaseClass::shoot(creator,deltaT);	
}

void MinigunMob::takeDamage(Bullet* bullet )
{

	BaseClass::takeDamage(bullet);
}

IRenderer* MinigunMob::getRenderer()
{
	return &gRenderer;
}
