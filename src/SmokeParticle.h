#ifndef SmokeParticle_h__
#define SmokeParticle_h__

#include "Particle.h"

class SmokeParticle : public Particle
{
	typedef Particle BaseClass;
public:
	SmokeParticle( Vec2f const& pos );
	virtual void init();
	virtual void tick();
	virtual void onSpawn( unsigned flag );
	virtual IRenderer* getRenderer();

};

#endif // SmokeParticle_h__