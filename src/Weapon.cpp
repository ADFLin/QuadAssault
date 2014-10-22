#include "Weapon.h"

#include "Level.h"
#include "Bullet.h"
#include "Player.h"

#include "WeaponRenderer.h"

void FireHelper::fire( IBulletFactory& factory , Vec2f const& offset /*= Vec2f(0,0) */ )
{
	Bullet* bullet = factory.create();
	bullet->init();
	bullet->setup( pos + offset , dir ,team );
	weapon->getOwner()->getLevel()->addBullet( bullet );
	weapon->onFireBullet( bullet );
}

void Weapon::init( Player* player )
{	
	mOwner = player;
	mSize = Vec2f( 16 , 32 );

	mCDTime    = 100;
	mFireTimer = mCDTime;
	mCDSpeed   = 250;	
	mEnergyCast = 1;
}

void Weapon::tick()
{	
	mFireTimer += mCDSpeed * TICK_TIME;
}

void Weapon::fire( Vec2f const& pos, Vec2f const& dir, int team )
{
	if ( mFireTimer >= mCDTime )
	{
		FireHelper helper;
		helper.pos  = pos;
		helper.dir  = dir;
		helper.team = team;
		helper.weapon = this;
		doFire( helper );

		mFireTimer = 0;
	}
}

void Weapon::onFireBullet( Bullet* p )
{
	mOwner->loseEnergy(mEnergyCast);
}

void Weapon::render( RenderPass pass )
{
	getRenderer()->render( pass , this );
}


