#ifndef Weapon_h__
#define Weapon_h__

#include "Object.h"
#include "Renderer.h"

class Level;
class Bullet;
class Player;
class Weapon;

#include "ObjectFactory.h"
typedef IFactoryT< Bullet > IBulletFactory;

class FireHelper
{
public:
	void fire( IBulletFactory& factory , Vec2f const& offset = Vec2f(0,0) );
	Vec2f pos;
	Vec2f dir;
	int   team;
	Weapon* weapon;
};

class WeaponRenderer;

class Weapon : public Object
{
public:
	virtual void init( Player* player );
	virtual void tick();

	void render( RenderPass pass );

	virtual WeaponRenderer* getRenderer(){ return NULL; }
	virtual void onFireBullet(Bullet* p);

	void fire( Vec2f const& pos , Vec2f const& dir, int team );

	Player* getOwner(){ return mOwner; }
	float   getEnergyCast(){  return mEnergyCast;  }

protected:
	virtual void doFire( FireHelper& helper ){}

	friend class WeaponRenderer;
	Texture* mTextues[ NUM_RENDER_PASS ];

	float   mCDTime, mFireTimer;
	float   mCDSpeed;
	float   mEnergyCast;
	Player* mOwner;
	Vec2f   mSize;
};

#endif // Weapon_h__