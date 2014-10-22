#include "WeaponPickup.h"

#include "Level.h"

#include "Player.h"

#include "Laser.h"
#include "Plasma.h"
#include "Minigun.h"

#include "RenderUtility.h"


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

void WeaponPickup::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );

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

void WeaponPickup::onDestroy( unsigned flag )
{
	mLight.remove();
	BaseClass::onDestroy( flag );
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
