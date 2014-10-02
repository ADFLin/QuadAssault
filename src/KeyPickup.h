#ifndef KeyPickup_h__
#define KeyPickup_h__

#include "ItemPickup.h"


class KeyPickup : public ItemPickup
{
	typedef ItemPickup BaseClass;

public:
	KeyPickup( Vec2f const& pos , int id );
	void init();

	virtual void tick();
	virtual void onDestroy();
	virtual void onPick( Player* player );
	virtual void onSpawn();
	virtual IRenderer* getRenderer();


protected:
	float  mRotation;
	Light* mLight;
	int    mDoorId;
	friend class KeyPickupRenderer;

};

#endif // KeyPickup_h__