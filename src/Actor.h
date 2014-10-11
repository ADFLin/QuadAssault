#ifndef Actor_h__
#define Actor_h__

#include "Object.h"

class Actor : public LevelObject
{
	typedef LevelObject BaseClass;
public:

	Actor()
	{
		rotation = 0.0f;
	}

	float getRotation() const { return rotation; }
	void  setRotation( float theta )
	{ 
		rotation = theta;
		while(rotation > 2 * PI )
			rotation -= 2 * PI;
		while(rotation < 2 * PI)
			rotation += 2 * PI;	
	}
	void  rotate(float theta){ setRotation( rotation + theta ); }

	virtual void enumProp( IPropEditor& editor )
	{
		BaseClass::enumProp( editor );
		editor.addProp( "Rotation" , rotation );
	}

protected:

	float rotation;	

};

#endif // Actor_h__
