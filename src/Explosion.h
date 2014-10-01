#ifndef Explosion_h__
#define Explosion_h__

#include "Object.h"

class Light;

class Explosion : public LevelObject
{
	typedef LevelObject BaseClass;

public:
	Explosion();
	~Explosion();

	void Init( Vec2f poz , float radius );

	virtual ObjectType getType(){ return OT_EXPLOSION; }
	virtual void tick();
	virtual void onSpawn();
	virtual void onDestroy();
	
	void setParam(float intensity, float brzinaRasta, float brzinaUmiranja);
	void setColor( Vec3 const& c );
	

protected:

	float  radius;
	Light* light;

	bool umire; //ako umire, intensity se smanjuje, ako ne, onda se povecava

	float speedRasta; //brzina kojom se povezava intensity
	float speedUmiranja; //brzina kojom se smanjuje intensity
	float intensity, maxIntenzitet;

	Vec3  color; //boja svjetla


};


#endif // Explosion_h__