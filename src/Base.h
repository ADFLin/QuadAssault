#ifndef Base_h__
#define Base_h__

#include "MathCore.h"
#include "IntegerType.h"
#include <cmath>

#include <string>
typedef std::string String;

#include "FixString.h"
typedef FixString< 512 > FString;

#include "THolder.h"

namespace Priv
{
	template< class T >
	void ReleaseFree( T * ptr ){ ptr->release(); }
}


template< class T >
class FObjectPtr : public TPtrFunFreeHolder< T , &Priv::ReleaseFree< T > >
{

};

template< class T >
class FPtr : public TPtrHolder< T >
{

};

#ifndef BIT
#define BIT( n ) ( 1 << ( n ) )
#endif

typedef TVector2< int > Vec2i;

#define ARRAY_SIZE( ARRAY ) ( sizeof(ARRAY) / sizeof( ARRAY[0] ) )

float const TICK_TIME = 60 / 1000.0f;
int const BLOCK_SIZE = 64;

class Level;
class LevelObject;
class Light;
class ItemPickup;
class Bullet;
class Block;
class AreaTrigger;

class Texture;

struct Rect
{
	Vec2f min;
	Vec2f max;

	bool intersect( Rect const& a ) const
	{
		if( max.x < a.min.x || max.y<a.min.y ||	
			min.x > a.max.x || min.y>a.max.y ) 
			return false;		
		return true;
	}

	bool intersect( Rect const& a , Vec2f const& offset ) const
	{
		if( max.x < a.min.x + offset.x || max.y< a.min.y + offset.y ||	
			min.x > a.max.x + offset.x || min.y>a.max.y + offset.y ) 
			return false;

		return true;
	}

	bool hitTest( Vec2f const& p ) const
	{
		return min.x < p.x && p.x < max.x &&
			   min.y < p.y && p.y < max.y ;
	}
};

struct Color
{
	Color(){}
	Color( uint8 r , uint8 g , uint8 b , uint8 a = 255 )
		:r(r),g(g),b(b),a(a){}
	uint8 r , g , b , a;
};


enum RenderPass
{
	RP_DIFFUSE ,
	RP_NORMAL  ,
	RP_GLOW    ,
	NUM_RENDER_PASS ,
};

enum
{
	COL_PLAYER    = BIT(0) ,
	COL_SOILD     = BIT(1) ,
	COL_FLY_SOILD = BIT(2) ,
	COL_TRIGGER   = BIT(3) ,
	COL_BULLET    = BIT(4) ,
	COL_ITEM      = BIT(5) ,

	COL_TERRAIN   = BIT(12) ,
	COL_RENDER    = BIT(13) ,
	COL_VIEW      = BIT(14) , //For terrain

	COL_OBJECT    = COL_PLAYER | COL_SOILD | COL_FLY_SOILD | COL_TRIGGER | COL_BULLET | COL_ITEM ,
	COL_ALL       = 0xffffffff ,
};

#endif // Base_h__
