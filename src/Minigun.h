#ifndef MINIGUN_H
#define MINIGUN_H

#include "Weapon.h"

class Minigun : public Weapon
{
	typedef Weapon BaseClass;
public:
	virtual void init( Player* player );
	virtual void tick();
	virtual void doFire( FireHelper& heler );
	virtual WeaponRenderer* getRenderer();
};

#endif