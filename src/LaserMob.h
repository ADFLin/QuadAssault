#ifndef LaserMob_h__
#define LaserMob_h__

#include "Mob.h"

class LaserMob : public Mob
{
	typedef Mob BaseClass;
public:
	void init(Vec2f const& poz);
	void onSpawn();
	void onDestroy();
	void tick();
	void takeDamage(Bullet* p);
	void shoot( IBulletFactory const& creator );

	virtual IRenderer* getRenderer();

};

#endif // LaserMob_h__