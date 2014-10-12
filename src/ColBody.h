#ifndef ColBody_h__
#define ColBody_h__

#include "Base.h"
#include "IntrList.h"

class LevelObject;


class ColBody
{
public:
	ColBody();

	void setMask( unsigned mask ){ colMask = mask; }
	void setMaskCheck( unsigned mask ){ colMaskCheck = mask; }
	void setSize( Vec2f const& size ){ halfSize = size / 2; bUpdateSize = true; }
	void setOffset( Vec2f const& offset ){ mOffset = offset;  }

	Vec2f const& getOffset(){ return mOffset;  }
	LevelObject* getClient(){ return object; }

private:

	void updateCache();
	Vec2f        mOffset;

	friend class CollisionManager;

	bool         bUpdateSize;
	Vec2f        halfSize;
	unsigned     colMaskCheck;
	unsigned     colMask;
	Vec2f        cachePos;
	Rect         boundBox;
	int          idxCell;
	LevelObject* object;

	HookNode     cellHook;
	HookNode     managerHook;
};

#endif // ColBody_h__
