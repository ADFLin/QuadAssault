#ifndef MinigunBullet_h__
#define MinigunBullet_h__

#include "Bullet.h"
#include "Light.h"

class MinigunBullet : public Bullet
{
	DECLARE_OBJECT_CLASS( MinigunBullet , Bullet )
public:	
	virtual void init();
	virtual void tick();
	virtual void onDestroy( unsigned flag );
	virtual void onSpawn( unsigned flag );
	virtual IObjectRenderer* getRenderer();

private:
	Light light;

	BEGIN_CLASS_PROP()
	END_CLASS_PROP()
};

#endif // MinigunBullet_h__