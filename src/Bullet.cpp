#include "Bullet.h"
#include "Level.h"
#include "Explosion.h"

#include "Light.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{	
}

void Bullet::init( Vec2f const& poz, Vec2f const& dir, int team )
{
	setPos( poz );
	setSize( Vec2f(2 ,2 ) );

	this->dir=dir;
	this->team = team;	

	mTime=0;

	mSpeed    = 1000;
	mLifeTime = 0.6;
	mDamage   = 50;
}

void Bullet::onSpawn()
{
	BaseClass::onSpawn();
}

void Bullet::onDestroy()
{
	Explosion* e = getLevel()->createExplosion( getPos() , 128 );
	e->setParam(4,100,20);
	e->setColor(Vec3(0.25, 0.5, 1.0));	

	BaseClass::onDestroy();
}
void Bullet::tick()
{
	if ( mNeedDestroy )
		return;

	mPos += ( mSpeed * TICK_TIME ) * dir;

	mTime += TICK_TIME;

	if( mTime > mLifeTime )
		destroy();

	Rect bBox;
	calcBoundBox( bBox );

	TileMap& terrain = getLevel()->getTerrain();
	Tile* tile = getLevel()->testTerrainCollision( bBox , BF_MOVABLE | BF_FLYABLE );
	if ( tile )
	{
		Block* block = Block::FromType( tile->type );
		block->onCollision( *tile , this );
	}
}

