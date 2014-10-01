#ifndef MathCore_h__
#define MathCore_h__

#include "TVector2.h"
#include "TVector3.h"

#include <cmath>

float const PI = 3.14159265359f;

typedef TVector2< float > Vec2f;
typedef TVector3< float > Vec3;

class Math
{
public:
	static float toRad( float degree ){ return degree * PI / 180.0f;}
	static float toDeg( float rad ){ return rad * 180.0f / PI; }

	static float atan2( float y , float x ){ return ::atan2( y , x );  }

	template< class T >
	static T clamp( T value , T min , T max )
	{
		if ( value < min )
			value = min;
		else if ( value > max )
			value = max;
		return value;
	}

	static int floor( float value ){ return (int)::floor( value ); }

	static float normalize( Vec2f& v );
};




#endif // MathCore_h__