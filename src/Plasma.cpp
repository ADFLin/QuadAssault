#include "Plasma.h"

#include "GameInterface.h"
#include "Level.h"
#include "Player.h"
#include "TextureManager.h"
#include "PlasmaBullet.h"

void Plasma::init( Player* player )
{
	Weapon::init( player );

	TextureManager* texMgr = getGame()->getTextureMgr();

	mTextues[ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
	mTextues[ RP_NORMAL ]  = texMgr->getTexture("weapon1Normal.tga");
	mTextues[ RP_GLOW ]    = texMgr->getTexture("oruzje2Glow.tga");

	mCDSpeed=100;
	mEnergyCast=6;
}

void Plasma::update(float deltaT)
{
	Weapon::update(deltaT);
}

void Plasma::doFire( FireHelper& heler )
{
	heler.fire( new PlasmaBullet );
}