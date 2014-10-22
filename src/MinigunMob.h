#ifndef MinigunMob_h__
#define MinigunMob_h__

#include "Mob.h"

class MinigunMob : public Mob
{
	typedef Mob BaseClass;
public:
	virtual void init();
	virtual void tick();
	virtual void takeDamage(Bullet* p);
	virtual IObjectRenderer* getRenderer();
	void onSpawn( unsigned flag );
	void onDestroy( unsigned flag );
	void shoot( IBulletFactory const& creator );
};

#endif // MinigunMob_h__