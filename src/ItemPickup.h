#ifndef ItemPickup_h__
#define ItemPickup_h__

#include "Object.h"

class LevelStage;
class Player;

class ItemPickup : public LevelObject
{
	typedef LevelObject BaseClass;
protected:
	GLuint tex, texN, texG;
public:
	void Init(Vec2f poz);

	virtual ObjectType getType(){ return OT_ITEM; }
	virtual void tick();
	virtual void onDestroy();
	virtual void onPick(Player* igrac);

};


#endif // ItemPickup_h__
