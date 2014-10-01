#ifndef DebrisPickup_h__
#define DebrisPickup_h__

#include "ItemPickup.h"
#include "Light.h"

class DebrisPickup : public ItemPickup
{
	typedef ItemPickup BaseClass;

public:
	void Init(Vec2f poz);

	virtual void onSpawn();
	virtual void onDestroy();
	virtual void tick();
	virtual IRenderer* getRenderer();

	bool checkCollision();
	void onPick(Player* igrac);

	

protected:	
	Vec2f dir;
	float brzina;

	float cesticaTimer; //dok dosegne 0, spawna cesticu

	Light* light;

};


#endif // DebrisPickup_h__