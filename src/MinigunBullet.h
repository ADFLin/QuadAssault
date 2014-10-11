#ifndef MinigunBullet_h__
#define MinigunBullet_h__

#include "Bullet.h"
#include "Light.h"

class MinigunBullet : public Bullet
{
	typedef Bullet BaseClass;

public:	
	virtual void init();
	virtual void tick();
	virtual void onDestroy();
	virtual void onSpawn();
	virtual IRenderer* getRenderer();

private:
	Light light;

};

#endif // MinigunBullet_h__