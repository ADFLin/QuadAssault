#ifndef KeyPickup_h__
#define KeyPickup_h__

#include "ItemPickup.h"

#include "Light.h"


class KeyPickup : public ItemPickup
{
	typedef ItemPickup BaseClass;

public:
	KeyPickup();
	KeyPickup( Vec2f const& pos , int id );

	virtual void init();
	virtual void tick();
	virtual void onDestroy();
	virtual void onPick( Player* player );
	virtual void onSpawn();
	virtual IRenderer* getRenderer();

	int       idDoor;
protected:
	float     mRotation;
	Light     mLight;
	
	friend class KeyPickupRenderer;

};

#endif // KeyPickup_h__