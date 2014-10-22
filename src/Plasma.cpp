#include "Plasma.h"

#include "RenderSystem.h"
#include "Level.h"
#include "Player.h"
#include "TextureManager.h"
#include "PlasmaBullet.h"

void Plasma::init( Player* player )
{
	BaseClass::init( player );

	mCDSpeed=100;
	mEnergyCast=6;
}

void Plasma::tick()
{
	BaseClass::tick();
}

void Plasma::doFire( FireHelper& heler )
{
	heler.fire( IBulletFactory::Make< PlasmaBullet >() );
}