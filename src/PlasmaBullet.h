#ifndef PlasmaBullet_h__
#define PlasmaBullet_h__

#include "Bullet.h"

class LevelStage;

class PlasmaBullet : public Bullet
{

public:	
	void init(Vec2f const& poz, Vec2f const& dir, int team );

	virtual void tick();
	virtual void onDestroy();
	virtual void onSpawn();
	virtual IRenderer* getRenderer();

private:
	Light* light;
	float  dimTimer; //dok ostvari 10, ostavlja cesticu dima
};

#endif // PlasmaBullet_h__