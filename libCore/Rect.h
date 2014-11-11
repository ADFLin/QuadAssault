#ifndef Rect_h__028D948C_C13C_4C3B_BADB_C6A4B8405647
#define Rect_h__028D948C_C13C_4C3B_BADB_C6A4B8405647

#include "TVector2.h"

template< class T >
struct TRect
{
	typedef TVector2< T > CoordType;
	CoordType min;
	CoordType max;

	CoordType getSize() const { return max - min; }

	TRect(){}

	TRect( CoordType const& min , CoordType const& max )
		:min(min),max(max){}

	bool intersect( TRect const& a ) const
	{
		if( max.x < a.min.x || max.y<a.min.y ||	
			min.x > a.max.x || min.y>a.max.y ) 
			return false;		
		return true;
	}

	bool intersect( TRect const& a , CoordType const& offset ) const
	{
		if( max.x < a.min.x + offset.x || max.y< a.min.y + offset.y ||	
			min.x > a.max.x + offset.x || min.y>a.max.y + offset.y ) 
			return false;

		return true;
	}

	bool hitTest( CoordType const& p ) const
	{
		return min.x < p.x && p.x < max.x &&
			   min.y < p.y && p.y < max.y ;
	}

	bool  overlap( TRect const& other )
	{
		T xMin = std::max( min.x, other.min.x );
		T yMin = std::max( min.y, other.min.y );
		T xMax = std::min( max.x , other.max.x );
		T yMax = std::min( max.y , other.max.y );

		if ( xMax  >= xMin && yMax >= yMin )
		{
			min.x = xMin;
			min.y = yMin;
			max.x = xMax;
			max.y = yMax;
			return true;
		}
		return false;
	}
};

#endif // Rect_h__028D948C_C13C_4C3B_BADB_C6A4B8405647
