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

	domet_timer=0;

	speed=1000;
	domet=600;
	mDamage=50;
}

void Bullet::onSpawn()
{
	BaseClass::onSpawn();
	light= getLevel()->createLight( getPos() , 64 , false);	
	light->setColorParam(Vec3(1.0, 0.5, 0.25), 2);	
}

void Bullet::onDestroy()
{
	light->destroy();
	light=NULL;
	Explosion* e = getLevel()->createExplosion( getPos() , 128 );
	e->setParam(4,100,20);
	e->setColor(Vec3(0.25, 0.5, 1.0));	

	BaseClass::onDestroy();
}
void Bullet::tick()
{
	if ( mNeedDestroy )
		return;

	mPos += ( speed * TICK_TIME ) * dir;

	light->setPos( getPos() );	

	domet_timer += 1000 * TICK_TIME;

	if( domet_timer > 100 * domet )
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

