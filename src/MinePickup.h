#ifndef MinePickup_h__
#define MinePickup_h__

#include "ItemPickup.h"
#include "MineParticle.h"
#include "Light.h"

class MinePickup : public ItemPickup
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

#endif // MinePickup_h__