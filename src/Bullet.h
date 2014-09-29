#ifndef Bullet_h__
#define Bullet_h__

#include "Object.h"

class Light;

enum Team
{
	TEAM_PLAYER ,
	TEAM_EMPTY  ,
};

class Bullet : public LevelObject
{
	typedef LevelObject BaseClass;
protected:
	Vec2f  dir;
	float  speed;

	float  domet, domet_timer;

	Light* light;

	float mDamage;

public:
	Bullet();
	~Bullet();

	virtual void init(Vec2f const& poz, Vec2f const& dir, int team );

	virtual ObjectType getType() { return OT_BULLET; }
	virtual void onSpawn();
	virtual void onDestroy();
	virtual void tick();
	
	float getDamage(){ return mDamage; }


	friend class MinigunBulletRenderer;
	friend class LaserBulletRenderer;
	int  team;
};

#endif // Bullet_h__