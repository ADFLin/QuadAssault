#ifndef PlasmaBullet_h__
#define PlasmaBullet_h__

#include "Bullet.h"

#include "Light.h"

class LevelStage;

class PlasmaBullet : public Bullet
{
	typedef Bullet BaseClass;
public:	
	virtual void init();
	virtual void tick();
	virtual void onDestroy();
	virtual void onSpawn();
	virtual IRenderer* getRenderer();
	
private:
	Light mLight;
	float  dimTimer; //dok ostvari 10, ostavlja cesticu dima
};

#endif // PlasmaBullet_h__