#ifndef Particle_h__
#define Particle_h__

#include "Object.h"

class Particle : public LevelObject
{
	DECLARE_OBJECT_CLASS( Particle , LevelObject )
public:
	Particle( Vec2f const& pos );

	virtual ObjectType getType(){ return OT_PARTICLE; }
	virtual void init();
	virtual void tick();
	virtual void onSpawn( unsigned flag );

protected:
	float zivot; //smanjuje se postepeno	
	float maxZivot; //za rendering

	friend class SmokeRenderer;
	friend class DebrisParticleRenderer;

	BEGIN_CLASS_PROP()
	END_CLASS_PROP()
};


#endif // Particle_h__