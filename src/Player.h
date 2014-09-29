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
private:		
	float brzina; //najveca moguca brzina kretanja	

	bool  mIsDead;

	float akceleracija;

	float rotationAim;

	float hp; //zdravlje	
	float energy; //za pucanje

	Weapon* weaponSlot[4];
	bool    haveShoot;

	Light* light;

	float  shiftTrack;

	friend class PlayerRenderer;

public:
	Player();
	~Player();

	

	ObjectType getType(){ return OT_PLAYER; }

	void Init( Vec2f poz );
	void onSpawn();
	void onDestroy();
	void Update(float deltaT, Vec2f mis);
	void updateHeadlight();

	void DodajMoment(float x);

	void RenderHP_Bar();
	void RenderEnergija_Bar();

	void Shoot(Vec2f misPoz, float deltaT);

	bool checkCollision();
	void clearWeapons();
	void SudarProjektila();
	void takeDamage(Bullet* p);

	bool isDead();
	void loseEnergy(float e);
	void DodajHP(float kolicina);

	void addWeapon(Weapon* o);

	virtual IRenderer* getRenderer();

};


#endif // Player_h__