#ifndef MinigunBullet_h__
#define MinigunBullet_h__

#include "Bullet.h"

class MinigunBullet : public Bullet
{
	typedef Bullet BaseClass;
private:
	GLuint texG;
public:	
	void init(Vec2f const& poz, Vec2f const& dir, int team);
	void tick();
	void onDestroy();
	void onSpawn();

	virtual IRenderer* getRenderer();

};

#endif // MinigunBullet_h__