#include "WeaponPickup.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"

#include "Player.h"

#include "Laser.h"
#include "Plasma.h"
#include "Minigun.h"

#include "RenderUtility.h"

class WeaponPickupRenderer : public IRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getGame()->getTextureMgr();
		mTex[ WEAPON_LASER ][ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTex[ WEAPON_LASER ][ RP_NORMAL  ] = texMgr->getTexture("weapon1Normal.tga");
		mTex[ WEAPON_LASER ][ RP_GLOW    ] = texMgr->getTexture("oruzje1Glow.tga");

		mTex[ WEAPON_PLAZMA ][ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTex[ WEAPON_PLAZMA ][ RP_NORMAL  ] = texMgr->getTexture("weapon1Normal.tga");
		mTex[ WEAPON_PLAZMA ][ RP_GLOW    ] = texMgr->getTexture("oruzje2Glow.tga");

		mTex[ WEAPON_MINIGUN ][ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTex[ WEAPON_MINIGUN ][ RP_NORMAL  ] = texMgr->getTexture("weapon1Normal.tga");
		mTex[ WEAPON_MINIGUN ][ RP_GLOW    ] = texMgr->getTexture("oruzje3Glow.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		WeaponPickup* pickup = static_cast< WeaponPickup* >( object );
		drawSprite( pickup->getRenderPos() + Vec2f( pickup->getSize().x/2-8,0),Vec2f(16,32), pickup->mRotation , mTex[ pickup->mId ][ pass ] );
	}
	Texture* mTex[ NUM_WEAPON_ID ][ NUM_RENDER_PASS ];
};

DEFINE_RENDERER( WeaponPickup , WeaponPickupRenderer )


WeaponPickup::WeaponPickup( Vec2f const& pos , int id ) 
	:BaseClass( pos ),mId( id )
{

}

WeaponPickup::WeaponPickup()
{

}

void WeaponPickup::init()
{
	BaseClass::init();

	mSize.x=32;
	mSize.y=32;
	mRotation=0;
}

void WeaponPickup::onSpawn()
{
	BaseClass::onSpawn();

	mLight.host   = this;
	mLight.radius = 256;
	switch( mId )
	{
	case WEAPON_LASER:  mLight.setColorParam(Vec3f(0.2,1.0,0.2),6); break;
	case WEAPON_PLAZMA: mLight.setColorParam(Vec3f(0.2,0.2,1.0),6); break;
	case WEAPON_MINIGUN: mLight.setColorParam(Vec3f(1.0,0.2,2.0),6); break;
	}
	getLevel()->addLight( mLight );
	
}

void WeaponPickup::onDestroy()
{
	mLight.remove();
	BaseClass::onDestroy();
}

void WeaponPickup::tick()
{
	BaseClass::tick();
	mRotation+=100*TICK_TIME;
	if(mRotation>360)
		mRotation -= 360;
}


void WeaponPickup::onPick(Player* player)
{
	getLevel()->playSound("pickup.wav");

	Weapon* weapon = NULL;

	switch( mId )
	{
	case WEAPON_LASER: weapon = new Laser; break;
	case WEAPON_PLAZMA: weapon = new Plasma; break;
	case WEAPON_MINIGUN: weapon = new Minigun; break;
	}

	if ( weapon )
	{
		player->addWeapon( weapon );
	}
	destroy();
}

void WeaponPickup::enumProp( IPropEditor& editor )
{
	BaseClass::enumProp( editor );
	editor.addProp( "WeaponId" , mId );
}

void WeaponPickup::setupDefault()
{
	BaseClass::setupDefault();
	mId = WEAPON_LASER;
}

void WeaponPickup::updateEdit()
{
	BaseClass::updateEdit();
	switch( mId )
	{
	case WEAPON_LASER:  mLight.setColorParam(Vec3f(0.2,1.0,0.2),6); break;
	case WEAPON_PLAZMA: mLight.setColorParam(Vec3f(0.2,0.2,1.0),6); break;
	case WEAPON_MINIGUN: mLight.setColorParam(Vec3f(1.0,0.2,2.0),6); break;
	}
}
