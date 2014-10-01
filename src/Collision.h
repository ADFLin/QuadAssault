#ifndef Collision_h__
#define Collision_h__

class LevelObject;

class ColBody
{
	ColBody()
	{
		idxCell = -1;
	}

	void setMask( unsigned mask ){ colMask = mask; }


private:
	friend class CollisionManager;

	unsigned     colMask;
	Vec2f        halfSize;
	Rect         boundBox;
	HookNode     cellHook;
	int          idxCell;
	LevelObject* object;
};

class CollisionManager
{
public:

	bool testCollision( ColBody& body , Cell& cell )
	{
		for ( CellBodyList::iterator iter = cell.bodies.begin() , itEnd = cell.bodies.end();
			 iter != itEnd ; ++iter )
		{
			ColBody* bodyTest = *iter;

			if ( bodyTest == &body )
				continue;

			unsigned mask = body.colMask & bodyTest->colMask;

			if ( mask == 0 )
				continue;

			if ( !body.boundBox.intersect( bodyTest->boundBox ) )
				continue;



			return true;
		}

		return false;
	}

	void addBody( LevelObject& object , ColBody& body )
	{
		body.object = &object;

		Vec2f pos = body.object->getPos();

		int cx = Math::clamp( pos.x / mCellLength , 0 , mCellMap.getSizeX() - 1 );
		int cy = Math::clamp( pos.y / mCellLength , 0 , mCellMap.getSizeX() - 1 );
		int idxCell = mCellMap.toIndex( cx , cy );

		body.idxCell = idxCell;
		mCellMap[ idxCell ].bodies.push_back( &body );
	}
	
	void init( float length , float width , float cellLength )
	{
		int cx = int ( length / cellLength - 1 ) + 1;
		int cy = int ( width / cellLength - 1 ) + 1;
		mCellMap.resize( cx , cy );
		mCellLength = cellLength;
	}

	void updateBody( ColBody& body )
	{
		Vec2f pos = body.object->getPos();
		
		int cx = Math::clamp( pos.x / mCellLength , 0 , mCellMap.getSizeX() - 1 );
		int cy = Math::clamp( pos.y / mCellLength , 0 , mCellMap.getSizeX() - 1 );
		int idxCell = mCellMap.toIndex( cx , cy );
		if ( idxCell != body.idxCell )
		{
			body.cellHook.unlink();
			body.idxCell = idxCell;
			mCellMap[ idxCell ].bodies.push_back( &body );
		}
	}

	typedef IntrList< ColBody , MemberHook< ColBody , ColBody::cellHook > , PointerType > CellBodyList;
	struct Cell
	{
		CellBodyList bodies;
	};
	float            mCellLength;
	TGrid2D< Cell >  mCellMap;


};

#endif // Collision_h__
