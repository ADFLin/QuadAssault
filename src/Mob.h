#ifndef MOB_H
#define MOB_H

#include "Actor.h"
#include "ColBody.h"
#include "ObjectFactory.h"


class Player;

class Bullet;
typedef IFactoryT< Bullet > IBulletFactory;

class MobRenderer : public IRenderer
{
public:
	virtual void render( RenderPass pass , LevelObject* object );
	virtual void renderGroup( RenderPass pass , int numObj, LevelObject* object );
	
protected:
	Texture* mTextures[ NUM_RENDER_PASS ];
};

class Mob : public Actor
{
	typedef Actor BaseClass;

public:
	virtual void init();

	virtual ObjectType getType(){ return OT_MOB; }
	virtual void onSpawn( unsigned flag );
	virtual void onDestroy( unsigned flag );
	virtual void tick();
	virtual void onBodyCollision( ColBody& self , ColBody& other );

	virtual void shoot( IBulletFactory const& creator );
	

protected:

	virtual void spawnEffect();
	virtual void takeDamage(Bullet* p);

	bool testCollision( Vec2f const& offset );

	ColBody mBody;
	float   akceleracija;

	float punjenje;

	//NASLIJED:
	float   brzina;
	float   maxbrzina;
	short   mHP;
	float   brzinaPunjenja;
	float   domet;
	Player* mTarget;
	float   mTimeCantView;
	Vec2f   mPosLastView;

};
#endif