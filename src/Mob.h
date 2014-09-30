#ifndef MOB_H
#define MOB_H

#include "Actor.h"
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
protected:			
	float akceleracija;	
	
	float punjenje;

	//NASLIJED:
	float brzina;
	float maxbrzina;
	short hp;
	float brzinaPunjenja;
	float domet;
	Player* mTarget;
	float   mTimeCantView;
	Vec2f   mPosLastView;
public:
	
	virtual void init( Vec2f const& poz );

	virtual void onSpawn();
	virtual void onDestroy();
	virtual ObjectType getType(){ return OT_MOB; }
	virtual void tick();
	virtual void spawnEffect();
	
	virtual void shoot( IBulletFactory const& creator );

	virtual void takeDamage(Bullet* p);


	void DodajMoment(float x);

	bool checkCollision();
	void SudarProjektila();	

};
#endif