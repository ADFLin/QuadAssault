#ifndef Player_h__
#define Player_h__

#include "Actor.h"
#include "Block.h"
#include "Weapon.h"
#include "Message.h"

class Light;

class Player : public Actor
{
	typedef Actor BaseClass;


public:
	Player();
	~Player();


	virtual ObjectType getType(){ return OT_PLAYER; }
	virtual void onSpawn();
	virtual void onDestroy();
	virtual IRenderer* getRenderer();

	void  Init( Vec2f poz );
	void  update( Vec2f const& aimPos );

	bool  isDead();
	void  addWeapon(Weapon* o);
	float getEnergy() const { return mEnergy; }
	float getMaxEnergy() const { return 100; }
	float getHP() const { return mHP; }
	float getMaxHP() const { return 100; }

	void DodajMoment(float x);
	void shoot( Vec2f const& posTaget );

	void takeDamage(Bullet* p);

	void loseEnergy(float e);
	void addHP(float kolicina);

	void RenderHP_Bar();
	void RenderEnergija_Bar();

	


private:
	void clearWeapons();
	bool checkCollision();
	void SudarProjektila();

	void updateHeadlight();

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