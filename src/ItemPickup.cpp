#include "ItemPickup.h"
#include "Level.h"
#include "Player.h"

void ItemPickup::Init( Vec2f poz )
{
	this->mPos=poz;
	mSize.x=32;
	mSize.y=32;	
}

void ItemPickup::tick()
{
	Rect k1;
	k1.min=mPos;
	k1.max=mPos+mSize;
	Rect k2; 
	Player* player = getLevel()->getPlayer();
	k2.min = player->getPos();
	k2.max = player->getPos() + player->getSize();
	if(k1.intersect(k2))
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