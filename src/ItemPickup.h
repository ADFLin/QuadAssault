#ifndef ItemPickup_h__
#define ItemPickup_h__

#include "Object.h"

#include "ColBody.h"

class Player;

class ItemPickup : public LevelObject
{
	typedef LevelObject BaseClass;
public:
	ItemPickup();
	ItemPickup( Vec2f const& pos );
	virtual void init();
	virtual ObjectType getType(){ return OT_ITEM; }
	virtual void tick();
	virtual void onSpawn();
	virtual void onDestroy();
	virtual void onPick(Player* player);
	virtual void onBodyCollision( ColBody& self , ColBody& other );

protected:
	ColBody mBody;
};


#endif // ItemPickup_h__
