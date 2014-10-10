#ifndef Light_h___
#define Light_h___

#include "Object.h"


class Light : public LevelObject
{
	typedef LevelObject BaseClass;
public:

	Light();
	~Light();

	void init( Vec2f poz, float radius );

	virtual ObjectType getType(){ return OT_LIGHT; }
	virtual void enumProp( IPropEditor& editor );
	void tick();

	void setColorParam( Vec3f const& color , float intensity );
	void SetExplozija(bool explozija);
	void PostavkeKuta(Vec2f const& dir, float angle);

	



	float  radius;
	float  intensity;
	Vec3f  color;
	Vec2f  dir; //normal u kojem dir je light okrenuto (baterija)
	float  angle; //od 0.0 do 1.0, odredjuje angle djelovanja baterije

	bool  isStatic;
	bool  isExplosion;
	bool  drawShadow;

	friend class RenderEngine;

	
};

#endif // Light_h__