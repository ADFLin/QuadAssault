#ifndef PlasmaMob_h__
#define PlasmaMob_h__

#include "Mob.h"

class PlasmaMob : public Mob
{
	typedef Mob BaseClass;
private:
public:
	void init(Vec2f const& poz);
	void tick();
	void takeDamage(Bullet* p);
	void onDestroy();
	void onSpawn();

	virtual IRenderer* getRenderer();
};

#endif // PlasmaMob_h__