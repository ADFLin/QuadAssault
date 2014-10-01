#include "ItemPickup.h"
#include "Level.h"
#include "Player.h"

void ItemPickup::Init( Vec2f poz )
{
	setPos( poz );
	setSize( Vec2f( 32 , 32 ) );
}

void ItemPickup::tick()
{
	Rect bBox;
	calcBoundBox( bBox );

	Rect bBoxOther; 
	Player* player = getLevel()->getPlayer();
	player->calcBoundBox( bBoxOther );

	if( bBox.intersect(bBoxOther) )
	{
		onPick( player );
	}
}

void ItemPickup::onDestroy()
{

}

void ItemPickup::onPick(Player* igrac)
{

}