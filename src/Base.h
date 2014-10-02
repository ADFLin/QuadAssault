#ifndef Base_h__
#define Base_h__

#include "MathCore.h"
#include <cmath>

#ifndef BIT
#define BIT( n ) ( 1 << ( n ) )
#endif

typedef TVector2< int > Vec2i; 

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
};

enum RenderPass
{
	RP_DIFFUSE ,
	RP_NORMAL  ,
	RP_GLOW    ,
	NUM_RENDER_PASS ,
};



#endif // Base_h__
