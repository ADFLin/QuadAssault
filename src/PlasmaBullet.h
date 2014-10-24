#ifndef PlasmaBullet_h__
#define PlasmaBullet_h__

#include "Bullet.h"

#include "Light.h"

class LevelStage;

class PlasmaBullet : public Bullet
{
	DECLARE_OBJECT_CLASS( PlasmaBullet , Bullet )
public:	
	virtual void init();
	virtual void tick();
	virtual void onDestroy( unsigned flag );
	virtual void onSpawn( unsigned flag );
	virtual IObjectRenderer* getRenderer();
	
private:
	Light mLight;
	float  dimTimer; //dok ostvari 10, ostavlja cesticu dima


	BEGIN_CLASS_PROP()
	END_CLASS_PROP()
};

#endif // PlasmaBullet_h__