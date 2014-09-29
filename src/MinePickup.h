#ifndef MinePickup_h__
#define MinePickup_h__

#include "ItemPickup.h"
#include "MineParticle.h"
#include "Light.h"

class MinePickup : public ItemPickup
{
	typedef ItemPickup BaseClass;
protected:	
	Vec2f dir;
	float brzina;

	float cesticaTimer; //dok dosegne 0, spawna cesticu

	Light* light;

public:
	void Init(Vec2f poz);
	void onSpawn();
	void onDestroy();
	void tick();

	bool checkCollision();
	void onPick(Player* igrac);

	virtual IRenderer* getRenderer();


};

#endif // MinePickup_h__