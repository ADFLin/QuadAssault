#ifndef PlasmaMob_h__
#define PlasmaMob_h__

#include "Mob.h"

class PlasmaMob : public Mob
{
	typedef Mob BaseClass;
private:
public:
	virtual void init();
	virtual void onDestroy();
	virtual void onSpawn();
	virtual void tick();

	void takeDamage(Bullet* p);
	virtual IRenderer* getRenderer();
};

#endif // PlasmaMob_h__