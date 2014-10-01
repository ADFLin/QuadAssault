#ifndef Collision_h__
#define Collision_h__

#include "Base.h"

class LevelObject;


enum 
{
	COL_BULLET = BIT(0) ,
	COL_MOB    = BIT(1) ,
	COL_ALL    = 0xffffffff ,
};

class ColBody
{
	ColBody();
	void setMask( unsigned mask ){ colMask = mask; }
	void setSize( Vec2f const& size ){ halfSize = size / 2; }

private:
	friend class CollisionManager;

	unsigned     colMask;
	Vec2f        halfSize;
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
	bool testCollision( Vec2f const& pos , ColBody& body );
	bool checkCollision( ColBody& body );
	void addBody( LevelObject& obj , ColBody& body );
	void removeBody( ColBody& body );
	void updateBody( ColBody& body );
	
	typedef IntrList< ColBody , MemberHook< ColBody , ColBody::cellHook > >    CellBodyList;
	typedef IntrList< ColBody , MemberHook< ColBody , ColBody::managerHook > > BodyList;

	struct Cell
	{
		CellBodyList bodies;
	};
	float            mCellLength;
	TGrid2D< Cell >  mCellMap;
	BodyList         mBodies;

};

#endif // Collision_h__
