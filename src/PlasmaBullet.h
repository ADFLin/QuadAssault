#ifndef PlasmaBullet_h__
#define PlasmaBullet_h__

#include "Bullet.h"

class LevelStage;

class PlasmaBullet : public Bullet
{
private:
	float dimTimer; //dok ostvari 10, ostavlja cesticu dima
public:	
	void init(Vec2f const& poz, Vec2f const& dir, int team );
	void tick();
	void onDestroy();
	void onSpawn();

	virtual IRenderer* getRenderer();
};

#endif // PlasmaBullet_h__