#ifndef MinigunBullet_h__
#define MinigunBullet_h__

#include "Bullet.h"

class MinigunBullet : public Bullet
{
	typedef Bullet BaseClass;

public:	
	void init(Vec2f const& poz, Vec2f const& dir, int team);

	virtual void tick();
	virtual void onDestroy();
	virtual void onSpawn();
	virtual IRenderer* getRenderer();

private:
	Light* light;

};

#endif // MinigunBullet_h__