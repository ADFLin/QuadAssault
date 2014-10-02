#ifndef Player_h__
#define Player_h__

#include "Actor.h"

#include "ColBody.h"
#include "Block.h"
#include "Weapon.h"
#include "Message.h"

class Light;

class Player : public Actor
{
	typedef Actor BaseClass;
public:

	virtual ObjectType getType(){ return OT_PLAYER; }
	virtual void onSpawn();
	virtual void onDestroy();
	virtual void onBodyCollision( ColBody& self , ColBody& other );
	virtual IRenderer* getRenderer();

	void  init();
	void  update( Vec2f const& aimPos );

	void  shoot( Vec2f const& posTaget );
	void  addWeapon(Weapon* o);
	void  addHP(float kolicina);
	void  loseEnergy(float e);

	bool  isDead();
	float getEnergy() const { return mEnergy; }
	float getMaxEnergy() const { return 100; }
	float getHP() const { return mHP; }
	float getMaxHP() const { return 100; }

	void addMoment(float x);
	void takeDamage(Bullet* p);
	
private:
	void clearWeapons();
	bool testCollision( Vec2f const& offset );

	void updateHeadlight();

	ColBody mBody;

	float brzina; //najveca moguca brzina kretanja	

	bool  mIsDead;

	float akceleracija;

	float rotationAim;

	float mHP; //zdravlje	
	float mEnergy; //za pucanje

	static int const NUM_WEAPON_SLOT = 4;
	Weapon* mWeaponSlot[ NUM_WEAPON_SLOT ];
	bool    haveShoot;

	Light* light;

	float  shiftTrack;

	friend class PlayerRenderer;

};


#endif // Player_h__