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

void LaserMob::init(Vec2f const& poz)
{
	BaseClass::init(poz);

	mSize.x=64;
	mSize.y=64;
	
	maxbrzina=150;	
	brzinaPunjenja=250;	
	domet=512;

	hp=40;
}

void LaserMob::onSpawn()
{
	BaseClass::onSpawn();
}

void LaserMob::onDestroy()
{
	getLevel()->playSound("explosion1.wav");		
	BaseClass::onDestroy();
}

void LaserMob::tick()
{
	Mob::tick();

	shoot( BulletFactoryT< LaserBullet >() );

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