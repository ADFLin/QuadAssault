#ifndef Plasma_h__
#define Plasma_h__

#include "Weapon.h"

class Plasma : public Weapon
{
	typedef Weapon BaseClass;
public:
	virtual void init( Player* player );
	virtual void tick();
	virtual void doFire( FireHelper& heler );
	virtual WeaponRenderer* getRenderer();
};

#endif // Plasma_h__
