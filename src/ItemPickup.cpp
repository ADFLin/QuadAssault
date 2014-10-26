#include "ItemPickup.h"
#include "Level.h"
#include "Player.h"

IMPL_OBJECT_CLASS( ItemPickup , OT_ITEM , "Pickup" )

ItemPickup::ItemPickup( Vec2f const& pos ) 
	:BaseClass( pos )
{

}

ItemPickup::ItemPickup()
{

}

void ItemPickup::init()
{
	setSize( Vec2f( 32 , 32 ) );
	mBody.setSize( Vec2f( 32 , 32 ) );
	mBody.setMask( COL_ITEM | COL_RENDER );
	mBody.setMaskCheck( COL_PLAYER );
}

void ItemPickup::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );
	getLevel()->getColManager().addBody( *this , mBody );
}

void ItemPickup::onDestroy( unsigned flag )
{
	getLevel()->getColManager().removeBody( mBody );
	BaseClass::onDestroy( flag );
}

void ItemPickup::tick()
{

}

void ItemPickup::onPick(Player* player)
{

}

void ItemPickup::onBodyCollision( ColBody& self , ColBody& other )
{
	LevelObject* obj = other.getClient();
	switch( obj->getType() )
	{
	case OT_PLAYER:
		{
			onPick( obj->cast< Player >() );
		}
		break;
	}
}
