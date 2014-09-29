#ifndef Light_h___
#define Light_h___

#include "Object.h"

class Shader;

class Light : public LevelObject
{
	typedef LevelObject BaseClass;
public:

	float radius;
	float intensity;
	Vec3  color;
	Vec2f  dir; //normal u kojem dir je light okrenuto (baterija)
	float angle; //od 0.0 do 1.0, odredjuje angle djelovanja baterije

	bool  isStatic;
	bool  explozija;
	bool  drawShadow;

	Light();
	~Light();

	
	void init( Vec2f poz, float radius );

	ObjectType getType(){ return OT_LIGHT; }
	void tick();

	void setColorParam( Vec3 const& color , float intensity );
	void SetExplozija(bool explozija);
	void PostavkeKuta(Vec2f dir, float angle);
	
	void setupShader(Shader* s );

	
};

#endif // Light_h__