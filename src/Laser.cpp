#include "Laser.h"

#include "GameInterface.h"
#include "Level.h"
#include "Player.h"
#include "TextureManager.h"
#include "LaserBullet.h"

void Laser::init( Player* player )
{
	BaseClass::init( player );

	TextureManager* texMgr = getGame()->getTextureMgr();
	mTextues[ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
	mTextues[ RP_NORMAL ]  = texMgr->getTexture("weapon1Normal.tga");
	mTextues[ RP_GLOW ]    = texMgr->getTexture("oruzje1Glow.tga");

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