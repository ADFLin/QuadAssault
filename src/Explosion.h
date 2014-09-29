#ifndef Explosion_h__
#define Explosion_h__

#include "Object.h"

class Light;

class Explosion : public LevelObject
{
	typedef LevelObject BaseClass;
protected:
	
	float  radius;
	Light* light;

	bool umire; //ako umire, intensity se smanjuje, ako ne, onda se povecava

	float speedRasta; //brzina kojom se povezava intensity
	float speedUmiranja; //brzina kojom se smanjuje intensity
	float intensity, maxIntenzitet;

	Vec3 color; //boja svjetla


public:
	Explosion();
	~Explosion();

	void Init(Vec2f poz, float radius );

	ObjectType getType(){ return OT_EXPLOSION; }

	void onSpawn();
	void onDestroy();
	
	void setParam(float intensity, float brzinaRasta, float brzinaUmiranja);
	void setColor( Vec3 const& c );
	void tick();
	void render( RenderPass pass );	

};


#endif // Explosion_h__