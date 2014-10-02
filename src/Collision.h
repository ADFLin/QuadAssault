#ifndef Collision_h__
#define Collision_h__

#include "Base.h"
#include "ColBody.h"
#include "Block.h"

#include "IntrList.h"

struct ColInfo
{
	bool  isTerrain;
	union
	{
		Tile*    tile;
		ColBody* body;
	};
};

class CollisionManager
{
public:
	CollisionManager( TileMap& terrain ):mTerrain( &terrain ){}
	void setup( float width , float height  , float cellLength );

	void  addBody( LevelObject& obj , ColBody& body );
	void  removeBody( ColBody& body );

	void  update();
	bool  testCollision( ColInfo& info , Vec2f const& offset , ColBody& body , unsigned maskCheckReplace = 0 );
	Tile* rayTerrainTest( Vec2f const& from , Vec2f const& to , unsigned colMask );
	Tile* testTerrainCollision( Rect const& bBox , unsigned colMask );

private:

	bool checkCollision( ColBody& body );
	void updateBody( ColBody& body );
	void calcCellPos( Vec2f const& pos , int& cx , int& cy );


	Tile* rayBlockTest( Vec2i const& tPos , Vec2f const& from , Vec2f const& to , unsigned colMask );

	
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
