#ifndef LASER_H
#define LASER_H

#include "Weapon.h"

class Laser : public Weapon
{
	typedef Weapon BaseClass;
public:
	virtual void init( Player* player );
	virtual void tick();
	virtual void doFire( FireHelper& heler );
	virtual WeaponRenderer* getRenderer();
};

#endif