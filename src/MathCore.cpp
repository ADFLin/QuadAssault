#include "MathCore.h"

#include <cmath>

float Math::normalize( Vec2f& v )
{
	float d = sqrt( v.length2() );

	if(d==0)
		return 0.0f;

	v.x/=d;			
	v.y/=d;
	return d;
}
