#include "Laser.h"

#include "Level.h"
#include "Player.h"
#include "LaserBullet.h"


void Laser::init( Player* player )
{
	BaseClass::init( player );

	mCDSpeed=600;
	mEnergyCast=2;
}

void Laser::tick()
{
	BaseClass::tick();
}

void Laser::doFire( FireHelper& helper )
{
	for(int i=0; i<1; i++)
	{
		Vec2f offset = -4 * helper.dir;
		helper.fire( IBulletFactory::Make< LaserBullet >() , offset );		
	}
}