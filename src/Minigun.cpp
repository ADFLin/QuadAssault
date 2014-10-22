#include "Minigun.h"

#include "Level.h"
#include "Player.h"
#include "MinigunBullet.h"

void Minigun::init( Player* player )
{
	BaseClass::init( player );
	mCDSpeed=1500;
	mEnergyCast=0.5;
}

void Minigun::tick()
{
	BaseClass::tick();
}

void Minigun::doFire( FireHelper& helper )
{
	for(int i=0; i<1; i++)
	{
		Vec2f offset = -4 * helper.dir;
		helper.fire( IBulletFactory::Make< MinigunBullet >()  , offset );		
	}
}