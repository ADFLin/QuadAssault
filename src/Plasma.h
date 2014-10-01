#ifndef Plasma_h__
#define Plasma_h__

#include "Weapon.h"

class Plasma : public Weapon
{
private:
public:
	void init( Player* player );
	void tick();
	void doFire( FireHelper& heler );
};

#endif // Plasma_h__
