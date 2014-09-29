#ifndef LASER_H
#define LASER_H

#include "Weapon.h"

class Laser : public Weapon
{
	typedef Weapon BaseClass;
public:
	void init( Player* player );
	void update(float deltaT);
	void doFire( FireHelper& heler );
};

#endif