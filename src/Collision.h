#ifndef Collision_h__
#define Collision_h__

#include "Base.h"
#include "Block.h"

#include "IntrList.h"

class LevelObject;


enum 
{
	COL_LAND    = BIT(1) ,
	COL_FLY     = BIT(0) ,
	COL_TRIGGER = BIT(2) ,
	COL_TERRAIN = BIT(3) ,
	COL_ALL     = 0xffffffff ,
};

class ColBody
{
public:
	ColBody();

	void setMask( unsigned mask ){ colMask = mask; }
	void setSize( Vec2f const& size ){ halfSize = size / 2; bBBoxDirty = true; }
	void setOffset( Vec2f const& offset ){ mOffset = offset; bBBoxDirty = true; }

	Vec2f const& getOffset(){  return mOffset;  }
	
private:
	
	void updateBoundBox();
	Vec2f        mOffset;
	
	friend class CollisionManager;

	bool         bBBoxDirty;
	Vec2f        halfSize;
	unsigned     colMask;
	Rect         boundBox;
	int          idxCell;
	LevelObject* object;

	HookNode     cellHook;
	HookNode     managerHook;
};

class CollisionManager
{
public:
	void init( float length , float width , float cellLength );

	void update();
	bool testCollision( Vec2f const& pos , ColBody& body , unsigned maskReplace = 0 );
	bool checkCollision( ColBody& body );
	void addBody( LevelObject& obj , ColBody& body );
	void removeBody( ColBody& body );
	void updateBody( ColBody& body );


	void calcCellPos( Vec2f const& pos , int& cx , int& cy );
	
	typedef IntrList< ColBody , MemberHook< ColBody , &ColBody::cellHook > >    CellBodyList;
	typedef IntrList< ColBody , MemberHook< ColBody , &ColBody::managerHook > > BodyList;

	struct Cell
	{
		CellBodyList bodies;
	};
	float            mCellLength;
	TGrid2D< Cell >  mCellMap;
	BodyList         mBodies;
	TileMap*         mTerrain;

};

#endif // Collision_h__
