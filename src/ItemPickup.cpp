#include "ItemPickup.h"
#include "Level.h"
#include "Player.h"

void ItemPickup::Init( Vec2f poz )
{
	setPos( poz );
	mSize.x=32;
	mSize.y=32;	
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