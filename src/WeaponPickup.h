#ifndef WeaponPickup_h__
#define WeaponPickup_h__

#include "ItemPickup.h"

#include "Light.h"

enum WeaponId
{
	WEAPON_LASER ,
	WEAPON_PLAZMA ,
	WEAPON_MINIGUN ,

	NUM_WEAPON_ID ,
};

class WeaponPickup : public ItemPickup
{
	DECLARE_OBJECT_CLASS( WeaponPickup , ItemPickup )

public:
	WeaponPickup();

	WeaponPickup( Vec2f const& pos , int id );
	void init();

	virtual void tick();
	virtual void onPick(Player* player);
	virtual void onSpawn( unsigned flag );
	virtual void onDestroy( unsigned flag );

	virtual IObjectRenderer* getRenderer();

	virtual void setupDefault();
	virtual void updateEdit();


protected:
	float  mRotation;
	Light  mLight;
	int    mId;

	friend class WeaponPickupRenderer;

	BEGIN_CLASS_PROP()
	MEMBER_PROP( "WeaponId" , mId )
	END_CLASS_PROP()
};

#endif // WeaponPickup_h__