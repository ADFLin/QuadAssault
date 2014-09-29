#include "Minigun.h"

#include "GameInterface.h"
#include "Level.h"
#include "Player.h"
#include "TextureManager.h"
#include "MinigunBullet.h"

void Minigun::init( Player* player )
{
	BaseClass::init( player );

	TextureManager* texMgr = getGame()->getTextureMgr();

	mTextues[ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
	mTextues[ RP_NORMAL ]  = texMgr->getTexture("weapon1Normal.tga");
	mTextues[ RP_GLOW ]    = texMgr->getTexture("oruzje3Glow.tga");

	mCDSpeed=1500;
	mEnergyCast=0.5;
}
void Minigun::update(float deltaT)
{
	BaseClass::update(deltaT);
}

void Minigun::doFire( FireHelper& heler )
{
	for(int i=0; i<1; i++)
	{
		Vec2f offset = -4 * heler.dir;
		heler.fire( new MinigunBullet , offset );		
	}
}