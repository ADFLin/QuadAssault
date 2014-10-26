#include "Bullet.h"
#include "Level.h"
#include "Explosion.h"

IMPL_OBJECT_CLASS( Bullet , OT_BULLET , "Bullet" )

Bullet::Bullet()
{
}

Bullet::~Bullet()
{	

}

void Bullet::init()
{
	mTime=0;
	mSpeed    = 1000;
	mLifeTime = 0.6;
	mDamage   = 50;
}

void Bullet::setup( Vec2f const& poz, Vec2f const& dir, int team )
{
	setPos( poz );
	setSize( Vec2f( 2 , 2 ) );
	mDir=dir;
	this->team = team;	
}

void Bullet::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );
	mBody.setSize( Vec2f( 2 , 2 ) );
	mBody.setMask( COL_BULLET | COL_RENDER );
	mBody.setMaskCheck( COL_SOILD | COL_FLY_SOILD | COL_TERRAIN );
	getLevel()->getColManager().addBody( *this , mBody );
}

void Bullet::onDestroy( unsigned flag )
{
	getLevel()->getColManager().removeBody( mBody );
	BaseClass::onDestroy( flag );
}

void Bullet::tick()
{
	if ( mNeedDestroy )
		return;

	mPos += ( mSpeed * TICK_TIME ) * mDir;

	mTime += TICK_TIME;

	if( mTime > mLifeTime )
		destroy();
}

void Bullet::onTileCollision( ColBody& self , Tile& tile )
{
	Block* block = Block::FromType( tile.type );
	block->onCollision( tile , this );
}

