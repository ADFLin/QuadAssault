#ifndef Collision_h__
#define Collision_h__

class LevelObject;

class ColBody
{
protected:

	friend class CollisionManager;
	
	Rect     mBoundBox;
	HookNode cellHook;
	Vec2f    halfSize;
};

class CollisionManager
{
public:
	
	void init( float length , float width , float cellLength )
	{
		int cx = int ( length / cellLength - 1 ) + 1;
		int cy = int ( width / cellLength - 1 ) + 1;
		mCellMap.resize( cx , cy );
		mCellLength = cellLength;
	}

	void updateBody( ColBody& body )
	{



	}

	typedef IntrList< ColBody , MemberHook< ColBody , ColBody::cellHook > , PointerType > CellBodyList;
	struct Cell
	{
		CellBodyList objects;
	};
	float            mCellLength;
	TGrid2D< Cell >  mCellMap;


};

#endif // Collision_h__
