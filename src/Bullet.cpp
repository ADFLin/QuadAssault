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
	this->mPos=poz;
	this->dir=dir;
	this->team = team;	

	domet_timer=0;
	//NASLJEDJEN DIO:

	speed=1000;
	domet=600;
	mDamage=50;

	mSize.x=1;
	mSize.y=1;

}

void Bullet::onSpawn()
{
	BaseClass::onSpawn();

	light= getLevel()->createLight( mPos , 64 , false);	
	light->setColorParam(Vec3(1.0, 0.5, 0.25), 2);	
}

void Bullet::onDestroy()
{
	light->destroy();
	light=NULL;
	Explosion* e = getLevel()->createExplosion(mPos,128);
	e->setParam(4,100,20);
	e->setColor(Vec3(0.25, 0.5, 1.0));	
	e=NULL;

	BaseClass::onDestroy();
}
void Bullet::tick()
{
	mPos.y += dir.y * speed * TICK_TIME;
	mPos.x += dir.x * speed * TICK_TIME;
	light->changePos(mPos);		
	domet_timer += 1000 * TICK_TIME;

	if( domet_timer > domet )
		destroy();

	TileMap& terrain = getLevel()->getTerrain();
	for(int x=(mPos.x/BLOCK_SIZE)-2; x<(mPos.x/BLOCK_SIZE)+2; x++)
	{
		for(int y=(mPos.y/BLOCK_SIZE)-2; y<(mPos.y/BLOCK_SIZE)+2; y++)
		{
			if ( !terrain.isVaildRange( x , y ) )
				continue;

			Tile& tile = terrain.getData( x , y );
			Block* block = Block::FromType( tile.type );

			if( block->checkFlag( BF_COLLISION ) )
			{
				Rect k1;
				k1.min=mPos;
				k1.max=mPos+mSize;

				Rect k2;
				k2.min=Vec2f(x*BLOCK_SIZE,y*BLOCK_SIZE);
				k2.max=Vec2f(x*BLOCK_SIZE,y*BLOCK_SIZE)+Vec2f(BLOCK_SIZE,BLOCK_SIZE);
				if(k1.intersect(k2))
				{
					block->onCollision( tile , this );
					return;
				}
			}
		}
	}
}

