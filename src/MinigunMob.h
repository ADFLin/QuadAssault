#ifndef MinigunMob_h__
#define MinigunMob_h__

#include "Mob.h"

class MinigunMob : public Mob
{
	typedef Mob BaseClass;
public:
	void init(Vec2f const& poz);
	void tick();
	void takeDamage(Bullet* p);
	IRenderer* getRenderer();
	void onSpawn();
	void onDestroy();
	void shoot( IBulletFactory const& creator , float deltaT );
};

#endif // MinigunMob_h__