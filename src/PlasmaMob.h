#ifndef PlasmaMob_h__
#define PlasmaMob_h__

#include "Mob.h"

class PlasmaMob : public Mob
{
	DECLARE_OBJECT_CLASS( PlasmaMob , Mob )
public:
	virtual void init();
	virtual void onDestroy( unsigned flag );
	virtual void onSpawn( unsigned flag );
	virtual void tick();

	void takeDamage(Bullet* p);
	virtual IObjectRenderer* getRenderer();
};

#endif // PlasmaMob_h__