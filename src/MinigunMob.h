#ifndef MinigunMob_h__
#define MinigunMob_h__

#include "Mob.h"

class MinigunMob : public Mob
{
	DECLARE_OBJECT_CLASS( MinigunMob , Mob )
public:
	virtual void init();
	virtual void onSpawn( unsigned flag );
	virtual void onDestroy( unsigned flag );
	virtual void tick();
	virtual IObjectRenderer* getRenderer();

	virtual void takeDamage(Bullet* p);
	void shoot( IBulletFactory const& creator );

	BEGIN_CLASS_PROP()
	END_CLASS_PROP()
};

#endif // MinigunMob_h__