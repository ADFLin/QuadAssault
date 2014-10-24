#ifndef Actor_h__
#define Actor_h__

#include "Object.h"

class Actor : public LevelObject
{
	DECLARE_OBJECT_CLASS( Actor , LevelObject )
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

protected:

	float rotation;

	BEGIN_CLASS_PROP()
	MEMBER_PROP( "Rotation" , rotation  )
	END_CLASS_PROP()

};

#endif // Actor_h__
