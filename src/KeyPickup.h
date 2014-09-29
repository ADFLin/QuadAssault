#ifndef KP_H
#define KP_H
#include "ItemPickup.h"
#include "Light.h"

#define KEY_RED 0
#define KEY_BLUE 1
#define KEY_GREEN 2

class KeyPickup : public ItemPickup
{
	typedef ItemPickup BaseClass;

protected:
	float rotation;
	Light* s;
	int   id;
public:
	void Init(Vec2f poz, int id);
	void tick();
	void onDestroy();
	void onPick(Player* igrac);
	void onSpawn();

	virtual IRenderer* getRenderer();


	friend class KeyPickupRenderer;

};

#endif