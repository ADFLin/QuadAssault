#ifndef LaserBullet_h__
#define LaserBullet_h__

#include "Bullet.h"

class LaserBullet : public Bullet
{
public:	
	void init(Vec2f const& poz, Vec2f const& dir, int team );
	void tick();
	void onDestroy();
	void onSpawn();

	virtual IRenderer* getRenderer();

};


#endif // LaserBullet_h__