#include "WeaponPickup.h"
#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"

#include "Player.h"
#include "Light.h"

#include "Laser.h"
#include "Plasma.h"
#include "Minigun.h"

class WeaponPickupRenderer : public IRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getGame()->getTextureMgr();
		mTex[ LASER1 ][ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTex[ LASER1 ][ RP_NORMAL  ] = texMgr->getTexture("weapon1Normal.tga");
		mTex[ LASER1 ][ RP_GLOW    ] = texMgr->getTexture("oruzje1Glow.tga");

		mTex[ PLAZMA1 ][ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTex[ PLAZMA1 ][ RP_NORMAL  ] = texMgr->getTexture("weapon1Normal.tga");
		mTex[ PLAZMA1 ][ RP_GLOW    ] = texMgr->getTexture("oruzje2Glow.tga");

		mTex[ MINIGUN1 ][ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTex[ MINIGUN1 ][ RP_NORMAL  ] = texMgr->getTexture("weapon1Normal.tga");
		mTex[ MINIGUN1 ][ RP_GLOW    ] = texMgr->getTexture("oruzje3Glow.tga");

	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		WeaponPickup* pickup = static_cast< WeaponPickup* >( object );
		drawSprite( pickup->getPos() + Vec2f( pickup->getSize().x/2-8,0),Vec2f(16,32), pickup->rotation , mTex[ pickup->id ][ pass ] );
	}
	Texture* mTex[ 3 ][ NUM_RENDER_PASS ];
};

DEFINE_RENDERER( WeaponPickup , WeaponPickupRenderer )


void WeaponPickup::Init(Vec2f poz,int id)
{
	ItemPickup::Init(poz);

	mSize.x=32;
	mSize.y=32;
	this->id=id;
	rotation=0;
}

void WeaponPickup::onSpawn()
{
	BaseClass::onSpawn();

	s = getLevel()->createLight( getCenterPos() , 256 , false );
	if(id==LASER1)
		s->setColorParam(Vec3(0.2,1.0,0.2),6);
	else if(id==PLAZMA1)
		s->setColorParam(Vec3(0.2,0.2,1.0),6);
}

void WeaponPickup::onDestroy()
{
	s->destroy();
	BaseClass::onDestroy();
}

void WeaponPickup::tick()
{
	BaseClass::tick();
	rotation+=100*TICK_TIME;
	if(rotation>360)
		rotation -= 360;
}


void WeaponPickup::onPick(Player* player)
{
	getLevel()->playSound("pickup.wav");		

	if(id==LASER1)
		player->addWeapon(new Laser());
	else if(id==PLAZMA1)
		player->addWeapon(new Plasma());
	else if(id==MINIGUN1)
		player->addWeapon(new Minigun());

	destroy();
}
