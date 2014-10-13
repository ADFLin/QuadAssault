#include "LaserMob.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "Player.h"
#include "LaserBullet.h"

class LaserMobRenderer : public MobRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getGame()->getTextureMgr();
		mTextures[ RP_DIFFUSE ] = texMgr->getTexture("mob1Diffuse.tga");
		mTextures[ RP_NORMAL  ] = texMgr->getTexture("mob1Normal.tga");
		mTextures[ RP_GLOW    ] = texMgr->getTexture("mob1Glow.tga");
	}
};

DEFINE_RENDERER( LaserMob , LaserMobRenderer );

void LaserMob::init()
{
	BaseClass::init();

	mSize.x=64;
	mSize.y=64;
	
	maxbrzina=150;	
	brzinaPunjenja=250;	
	domet=512;

	mHP=40;
}

void LaserMob::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );
}

void LaserMob::onDestroy( unsigned flag )
{
	if ( flag & SDF_CAST_EFFECT )
	{
		getLevel()->playSound("explosion1.wav");	
	}	
	BaseClass::onDestroy( flag );
}

void LaserMob::tick()
{
	Mob::tick();

	shoot( IBulletFactory::Make< LaserBullet >() );

	Vec2f dir = getLevel()->getPlayer()->getPos()- getPos();
	
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


void LaserMob::shoot( IBulletFactory const& creator )
{	
	BaseClass::shoot( creator  );	
}

void LaserMob::takeDamage(Bullet* bullet)
{
	BaseClass::takeDamage(bullet);
}