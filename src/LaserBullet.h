#ifndef LaserBullet_h__
#define LaserBullet_h__

#include "Bullet.h"
#include "Light.h"

class LaserBullet : public Bullet
{
	typedef Bullet BaseClass;
public:	
	virtual void init();
	virtual void onSpawn( unsigned flag );
	virtual void onDestroy( unsigned flag );
	virtual void tick();

	virtual IObjectRenderer* getRenderer();

private:
	Light mLight;
};


#endif // LaserBullet_h__