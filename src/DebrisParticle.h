#ifndef DebrisParticle_h__
#define DebrisParticle_h__

#include "Particle.h"

class DebrisParticle : public Particle
{
	typedef Particle BaseClass;
public:
	DebrisParticle( Vec2f const& pos );
	virtual void init();
	virtual void tick();
	virtual void onSpawn( unsigned flag );

	virtual IRenderer* getRenderer();

};

#endif // DebrisParticle_h__