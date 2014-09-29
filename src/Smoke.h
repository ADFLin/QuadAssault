#ifndef Smoke_h__
#define Smoke_h__

#include "Particle.h"

class Smoke : public Particle
{
	typedef Particle BaseClass;
public:
	virtual void Init(Vec2f poz);
	virtual void tick();
	virtual void onSpawn();
	virtual IRenderer* getRenderer();

};

#endif // Smoke_h__