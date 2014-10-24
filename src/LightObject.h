#ifndef LightObject_h__
#define LightObject_h__

#include "Object.h"
#include "Light.h"


class LightObject : public LevelObject
	              , public Light
{
	DECLARE_OBJECT_CLASS( LightObject , LevelObject )
public:

	LightObject();
	~LightObject();

	void init();
	virtual void onSpawn( unsigned flag );
	virtual void onDestroy( unsigned flag );
	virtual void tick();
	virtual void setupDefault();
	virtual ObjectType getType(){ return OT_LIGHT; }


	BEGIN_CLASS_PROP()
	MEMBER_PROP( "Radius" , radius )
	MEMBER_PROP( "Color" , color )
	MEMBER_PROP( "Intensity" , intensity )
	MEMBER_PROP( "DrawShadow" , drawShadow )
	END_CLASS_PROP()

};





#endif // LightObject_h__