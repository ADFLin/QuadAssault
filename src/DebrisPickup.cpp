#include "DebrisPickup.h"

#include "DebrisParticle.h"

#include "Level.h"
#include "Explosion.h"
#include "Player.h"
#include "RenderUtility.h"

IMPL_OBJECT_CLASS( DebrisPickup , OT_ITEM , "Pickup.Debris" )

DebrisPickup::DebrisPickup( Vec2f const& pos ) 
	:BaseClass( pos )
{

}

void DebrisPickup::init()
{
	BaseClass::init();

	mSize.x=16;
	mSize.y=16;	

	brzina=200;
	cesticaTimer=1.0;
}

void DebrisPickup::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );

	dir.x=getLevel()->random(1,10)-5;
	dir.x/=5;
	dir.y=getLevel()->random(1,10)-5;
	dir.y/=5;
	Math::normalize( dir );

	mLight.host = this;
	mLight.isExplosion = true;
	mLight.radius = 64;
	mLight.setColorParam(Vec3f(1.0, 0.75, 0.5),4);
	getLevel()->addLight( mLight );
}

void DebrisPickup::onDestroy( unsigned flag )
{
	//mLight.remove();
	BaseClass::onDestroy( flag );
}

void DebrisPickup::tick()
{
	BaseClass::tick();		
	
	if(brzina>0.0)
	{
		if(cesticaTimer>=1.0)
		{
			DebrisParticle* c = new DebrisParticle( getPos() );
			c->init();	
			getLevel()->addParticle( c );
				
			cesticaTimer = 0.0;
		}
		else
		{
			cesticaTimer += TICK_TIME*10;
		}

		Vec2f off = ( brzina * TICK_TIME ) * dir;

		Vec2f offset = Vec2f( 0 , 0 );

		offset.y += off.y;
		if( testCollision( offset ) )
		{
			offset.y = 0;
			dir.y =-dir.y;
		}
		offset.x += off.x;
		if( testCollision( offset ) )
		{
			offset.x = 0;
			dir.x=-dir.x;
		}

		mPos += offset;
		brzina-=100*TICK_TIME;
	}
	else
	{
		brzina=0.0;
	}
}

bool DebrisPickup::testCollision( Vec2f const& offset )
{
	ColInfo info;
	return getLevel()->getColManager().testCollision( info , offset , mBody  , COL_TERRAIN );
}

void DebrisPickup::onPick(Player* player)
{
	getLevel()->playSound("pickup.wav");		
	
	Explosion* e= getLevel()->createExplosion( getPos(),128 );
	e->setParam(12,100,50);
	e->setColor(Vec3f(1.0, 0.75, 0.5));	

	player->addHP(2);

	destroy();
}
