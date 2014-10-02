#ifndef WeaponPickup_h__
#define WeaponPickup_h__

#include "ItemPickup.h"

#define LASER1 0
#define PLAZMA1 1
#define MINIGUN1 2

class WeaponPickup : public ItemPickup
{
	typedef ItemPickup BaseClass;

public:

	WeaponPickup( Vec2f const& pos , int id );
	void init();
	void tick();
	void onPick(Player* player);

	void onSpawn();
	void onDestroy();

	virtual IRenderer* getRenderer();

protected:
	float rotation;
	Light* s;
	int mId;

	friend class WeaponPickupRenderer;

};

#endif // WeaponPickup_h__