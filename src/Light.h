#ifndef Light_h__
#define Light_h__

#include "IntrList.h"

enum LightingModel
{
	LIGHT_POINT ,
	LIGHT_DIRECTIONAL ,

	LIGHT_EXPLOSION ,
	LIGHT_HEAD ,
};

class Light
{
public:

	Light()
	{
		host = NULL;
		offset = Vec2f( 0 , 0 );
		dir   = Vec2f(0.0, 0.0);
		angle = 0.0;
		drawShadow = false;
		isExplosion = false;
	}

	float  radius;
	float  intensity;
	Vec3f  color;
	Vec2f  dir;   //normal u kojem dir je light okrenuto (baterija)
	float  angle; //od 0.0 do 1.0, odredjuje angle djelovanja baterije

	bool  isExplosion;
	bool  drawShadow;

	Vec2f   offset;
	Object* host;

	void setColorParam( Vec3f const& inColor , float inIntensity )
	{
		color    = inColor;
		intensity= inIntensity;
	}

	void   remove()
	{
		if ( mHook.isLinked() )
			mHook.unlink();
	}
private:
	Vec2f    cachePos;
	HookNode mHook;
	friend class Level;
	friend class RenderEngine;	
};


#endif // Light_h__
