#ifndef WeaponPickup_h__
#define WeaponPickup_h__

#include "ItemPickup.h"

#define LASER1 0
#define PLAZMA1 1
#define MINIGUN1 2

class WeaponPickup : public ItemPickup
{
	typedef ItemPickup BaseClass;
protected:
	float rotation;
	Light* s;
	int id;
public:
	void Init(Vec2f poz, int id);
	void tick();
	void onPick(Player* igrac);

	void onSpawn();
	void onDestroy();

	virtual IRenderer* getRenderer();

	friend class WeaponPickupRenderer;

};

#endif // WeaponPickup_h__