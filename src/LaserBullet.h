#ifndef LaserBullet_h__
#define LaserBullet_h__

#include "Bullet.h"
#include "Light.h"

class LaserBullet : public Bullet
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
};


#endif // LaserBullet_h__