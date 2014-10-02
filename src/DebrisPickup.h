#ifndef DebrisPickup_h__
#define DebrisPickup_h__

#include "ItemPickup.h"
#include "Light.h"

class DebrisPickup : public ItemPickup
{
	typedef ItemPickup BaseClass;

public:
	DebrisPickup( Vec2f const& pos );;

	void init();

	virtual void onSpawn();
	virtual void onDestroy();
	virtual void tick();
	virtual IRenderer* getRenderer();

	void onPick(Player* player);

	bool testCollision( Vec2f const& offset );
	

protected:	
	Vec2f dir;
	float brzina;

	float cesticaTimer; //dok dosegne 0, spawna cesticu

	Light* light;

};


#endif // DebrisPickup_h__