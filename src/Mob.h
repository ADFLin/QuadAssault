#ifndef MOB_H
#define MOB_H

#include "Actor.h"


#include "ColBody.h"
#include "ObjectFactory.h"

class Bullet;
class Player;

typedef IFactoryT< Bullet > IBulletFactory;

template< class T >
class BulletFactoryT : public IBulletFactory
{
public:
	T* create() const { return new T; }
};

class MobRenderer : public IRenderer
{
public:
	virtual void render( RenderPass pass , LevelObject* object );
protected:
	Texture* mTextures[ NUM_RENDER_PASS ];
};

class Mob : public Actor
{
	typedef Actor BaseClass;

public:
	virtual void init( Vec2f const& poz );

	virtual ObjectType getType(){ return OT_MOB; }
	virtual void onSpawn();
	virtual void onDestroy();
	virtual void tick();
	virtual void onBodyCollision( ColBody& self , ColBody& other );

	virtual void spawnEffect();
	virtual void shoot( IBulletFactory const& creator );
	virtual void takeDamage(Bullet* p);

	void DodajMoment(float x);
	void SudarProjektila();	


protected:
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