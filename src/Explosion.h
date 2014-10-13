#ifndef Explosion_h__
#define Explosion_h__

#include "Object.h"

#include "Light.h"

class Explosion : public LevelObject
{
	typedef LevelObject BaseClass;

public:
	Explosion();
	Explosion( Vec2f const& pos , float radius );
	~Explosion();

	virtual void init();
	virtual ObjectType getType(){ return OT_EXPLOSION; }
	virtual void tick();
	virtual void onSpawn( unsigned flag );
	virtual void onDestroy( unsigned flag );
	
	void setParam(float intensity, float brzinaRasta, float brzinaUmiranja);
	void setColor( Vec3f const& c );
	

protected:

	float radius;
	Light mLight;

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