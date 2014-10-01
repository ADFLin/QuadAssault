#ifndef MINIGUN_H
#define MINIGUN_H

#include "Weapon.h"

class Minigun : public Weapon
{
	typedef Weapon BaseClass;
public:
	void init( Player* player );
	void tick();
	void doFire( FireHelper& heler );
};

#endif