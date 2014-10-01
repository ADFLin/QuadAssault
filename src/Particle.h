#ifndef Particle_h__
#define Particle_h__

#include "Object.h"

class Particle : public LevelObject
{
	typedef LevelObject BaseClass;
public:
	virtual ObjectType getType(){ return OT_PARTICLE; }
	virtual void Init(Vec2f poz);
	virtual void tick();
	virtual void onSpawn();

protected:
	float zivot; //smanjuje se postepeno	
	float maxZivot; //za rendering

	friend class SmokeRenderer;
	friend class MineParticleRenderer;
};


#endif // Particle_h__