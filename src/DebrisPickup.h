#ifndef DebrisPickup_h__
#define DebrisPickup_h__

#include "ItemPickup.h"
#include "Light.h"

class DebrisPickup : public ItemPickup
{
	DECLARE_OBJECT_CLASS( DebrisPickup , ItemPickup )
public:
	DebrisPickup( Vec2f const& pos );

	virtual void init();
	virtual void onSpawn( unsigned flag );
	virtual void onDestroy( unsigned flag );
	virtual void tick();
	virtual IObjectRenderer* getRenderer();

	void onPick(Player* player);
	bool testCollision( Vec2f const& offset );
	
protected:	
	Vec2f dir;
	float brzina;
	float cesticaTimer; //dok dosegne 0, spawna cesticu
	Light mLight;

	BEGIN_CLASS_PROP()
	END_CLASS_PROP()
};


#endif // DebrisPickup_h__