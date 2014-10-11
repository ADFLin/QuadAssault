#ifndef LightObject_h__
#define LightObject_h__

#include "Object.h"
#include "Light.h"


class LightObject : public LevelObject
	              , public Light
{
	typedef LevelObject BaseClass;
public:

	LightObject();
	~LightObject();

	void init();

	virtual ObjectType getType(){ return OT_LIGHT; }
	virtual void enumProp( IPropEditor& editor );
	virtual void onSpawn();
	virtual void onDestroy();

	void tick();
};

#endif // LightObject_h__