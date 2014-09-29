#ifndef MineParticle_h__
#define MineParticle_h__

#include "Particle.h"

class MineParticle : public Particle
{
	typedef Particle BaseClass;
public:
	virtual void Init(Vec2f poz);
	virtual void tick();
	virtual void onSpawn();

	virtual IRenderer* getRenderer();

};


#endif // MineParticle_h__