#include "ItemPickup.h"
#include "Level.h"
#include "Player.h"

ItemPickup::ItemPickup( Vec2f const& pos ) 
	:BaseClass( pos )
{

}

void ItemPickup::init()
{

	setSize( Vec2f( 32 , 32 ) );
	mBody.setSize( Vec2f( 32 , 32 ) );
	mBody.setMask( COL_ITEM );
	mBody.setMaskCheck( COL_PLAYER );
}

void ItemPickup::onSpawn()
{
	BaseClass::onSpawn();
	getLevel()->getColManager().addBody( *this , mBody );
}

void ItemPickup::onDestroy()
{
	getLevel()->getColManager().removeBody( mBody );
	BaseClass::onDestroy();
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
