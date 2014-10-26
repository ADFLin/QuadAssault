#include "KeyPickup.h"

#include "Level.h"
#include "Player.h"
#include "Explosion.h"
#include "RenderUtility.h"

IMPL_OBJECT_CLASS( KeyPickup , OT_ITEM , "Pickup.Key" )

KeyPickup::KeyPickup( Vec2f const& pos , int id ) 
	:BaseClass( pos ),mId( id )
{

}

KeyPickup::KeyPickup()
{

}

void KeyPickup::init()
{
	ItemPickup::init();

	mSize.x=32;
	mSize.y=32;
	mRotation=0;
}

void KeyPickup::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );

	mLight.host = this;
	mLight.radius = 128;
	mLight.setColorParam(getDoorColor( mId ) , 4 );
	getLevel()->addLight( mLight );
}

void KeyPickup::onDestroy( unsigned flag )
{
	mLight.remove();
	BaseClass::onDestroy( flag );
}

void KeyPickup::tick()
{
	BaseClass::tick();
	mRotation += Math::toRad( 100*TICK_TIME );
	if(mRotation> 2 * PI )
		mRotation -= 2 * PI;
}

void KeyPickup::onPick( Player* player )
{
	getLevel()->playSound("pickup.wav");		

	TileMap& terrain = getLevel()->getTerrain();

	for(int x=0; x< terrain.getSizeX() ; ++x )
	for(int y=0; y< terrain.getSizeY() ; ++y )
	{
		Tile& tile = terrain.getData( x , y );

		if( tile.type == BID_DOOR && tile.meta == mId )
		{
			tile.type = BID_FLAT;
			tile.meta = 0;

			Explosion* e=getLevel()->createExplosion( Vec2f(x*BLOCK_SIZE+BLOCK_SIZE/2, y*BLOCK_SIZE+BLOCK_SIZE/2), 128 );
			e->setParam(20,1000,50);
			e->setColor( getDoorColor( mId ) );	
		}
	}

	destroy();
}

void KeyPickup::setupDefault()
{
	BaseClass::setupDefault();
	mId = DOOR_RED;
}

void KeyPickup::updateEdit()
{
	BaseClass::updateEdit();
	mLight.setColorParam(getDoorColor( mId ) ,4);
}