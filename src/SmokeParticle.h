#ifndef SmokeParticle_h__
#define SmokeParticle_h__

#include "Particle.h"

class SmokeParticle : public Particle
{
	DECLARE_OBJECT_CLASS( SmokeParticle , Particle )
public:
	SmokeParticle( Vec2f const& pos );
	virtual void init();
	virtual void tick();
	virtual void onSpawn( unsigned flag );
	virtual IObjectRenderer* getRenderer();

	BEGIN_CLASS_PROP()
	END_CLASS_PROP()

};

#endif // SmokeParticle_h__