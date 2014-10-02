#ifndef Bullet_h__
#define Bullet_h__

#include "Object.h"

#include "ColBody.h"

class Light;

enum Team
{
	TEAM_PLAYER ,
	TEAM_EMPTY  ,
};

class Bullet : public LevelObject
{
	typedef LevelObject BaseClass;


public:
	Bullet();
	~Bullet();

	virtual void init(Vec2f const& poz, Vec2f const& dir, int team );

	virtual ObjectType getType() { return OT_BULLET; }
	virtual void onSpawn();
	virtual void onDestroy();
	virtual void tick();
	
	float getDamage(){ return mDamage; }

	virtual void onTileCollision( ColBody& self , Tile& tile );

	int  team;

protected:

	friend class MinigunBulletRenderer;
	friend class LaserBulletRenderer;
	Vec2f   dir;
	float   mLifeTime;
	float   mTime;
	float   mDamage;
	float   mSpeed;
	ColBody mBody;
};

#endif // Bullet_h__