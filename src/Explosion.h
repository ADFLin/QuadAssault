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
	void setColor( Vec3f const& c );
	

protected:

	float  radius;
	Light* light;

	// if you die, the intensity is reduced, if not, then increases
	bool  mbDead;
	// the speed at which the intensity is increasing
	float growthRate; 
	// the speed at which the intensity is decreasing
	float deathRate;  
	float intensity, maxIntenzitet;
	// light color
	Vec3f  color;
};


#endif // Explosion_h__