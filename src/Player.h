#ifndef Player_h__
#define Player_h__

#include "Actor.h"

#include "ColBody.h"
#include "Block.h"
#include "Weapon.h"
#include "Light.h"

class LightObject;

class Player : public Actor
{
	DECLARE_OBJECT_CLASS( Player , Actor )
public:

	Player();

	int getPlayerId(){ return mPlayerId; }

	virtual void init();
	virtual void onSpawn( unsigned flag );
	virtual void onDestroy( unsigned flag );
	virtual void onBodyCollision( ColBody& self , ColBody& other );
	virtual void updateEdit();
	virtual IObjectRenderer* getRenderer();

	
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


	friend class Level;
	int   mPlayerId;
	

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

	Light  mHeadLight;

	float  shiftTrack;

	friend class PlayerRenderer;


	BEGIN_CLASS_PROP()
	END_CLASS_PROP()
};


#endif // Player_h__