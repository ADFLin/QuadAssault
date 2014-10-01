#include "Collision.h"

ColBody::ColBody()
{
	idxCell = -1;
}

void CollisionManager::init( float length , float width , float cellLength )
{
	int cx = int ( length / cellLength - 1 ) + 1;
	int cy = int ( width / cellLength - 1 ) + 1;
	mCellMap.resize( cx , cy );
	mCellLength = cellLength;
}


bool CollisionManager::testCollision( Vec2f const& pos , ColBody& body )
{
	Rect bBox;
	bBox.min = pos - body.halfSize;
	bBox.min = pos + body.halfSize;

	int cx = Math::clamp( pos.x / mCellLength , 0 , mCellMap.getSizeX() - 1 );
	int cy = Math::clamp( pos.y / mCellLength , 0 , mCellMap.getSizeX() - 1 );

	bool result = false;

	for( int oy = -1 ; oy <= 1 ; ++oy )
	{
		for( int ox = -1 ; ox <= 1 ; ++ox )
		{
			if ( !mCellMap.checkRange( cx + ox , cy + oy ) )
				continue;

			int idxCell = mCellMap.toIndex( cx + ox , cy + oy );

			Cell& cell = mCellMap[ idxCell ];


			for ( CellBodyList::iterator iter = cell.bodies.begin() , itEnd = cell.bodies.end();
				iter != itEnd ; ++iter )
			{
				ColBody& bodyTest = *iter;

				if ( bodyTest == &body )
					continue;

				unsigned mask = body.colMask & bodyTest.colMask;

				if ( mask == 0 )
					continue;

				if ( !body.boundBox.intersect( bodyTest.boundBox ) )
					continue;

				return true;
			}
		}
	}

	return false;
}
bool CollisionManager::checkCollision( ColBody& body )
{
	Vec2f pos = body.object->getPos();

	int cx = Math::clamp( pos.x / mCellLength , 0 , mCellMap.getSizeX() - 1 );
	int cy = Math::clamp( pos.y / mCellLength , 0 , mCellMap.getSizeX() - 1 );

	bool result = false;

	for( int oy = -1 ; oy <= 1 ; ++oy )
	{
		for( int ox = -1 ; ox <= 1 ; ++ox )
		{
			if ( !mCellMap.checkRange( cx + ox , cy + oy ) )
				continue;

			int idxCell = mCellMap.toIndex( cx + ox , cy + oy );

			Cell& cell = mCellMap[ idxCell ];


			for ( CellBodyList::iterator iter = cell.bodies.begin() , itEnd = cell.bodies.end();
				iter != itEnd ; ++iter )
			{
				ColBody& bodyTest = *iter;

				if ( bodyTest == &body )
					continue;

				unsigned mask = body.colMask & bodyTest.colMask;

				if ( mask == 0 )
					continue;

				if ( !body.boundBox.intersect( bodyTest.boundBox ) )
					continue;

				body.object->onCollision( body , bodyTest );
				result = true;
			}
		}
	}

	return result;
}

void CollisionManager::addBody( LevelObject& obj , ColBody& body )
{
	assert( body.idxCell = -1 );
	assert( body.halfSize.x < mCellLength / 2 && body.halfSize.y < mCellLength / 2 );
	body.object = &obj;

	Vec2f pos = body.object->getPos();

	int cx = Math::clamp( pos.x / mCellLength , 0 , mCellMap.getSizeX() - 1 );
	int cy = Math::clamp( pos.y / mCellLength , 0 , mCellMap.getSizeX() - 1 );
	int idxCell = mCellMap.toIndex( cx , cy );

	body.idxCell = idxCell;
	mCellMap[ idxCell ].bodies.push_back( &body );

	mBodies.push_back( &body );
}

void CollisionManager::removeBody( ColBody& body )
{
	body.idxCell = -1;
	body.cellHook.unlink();
	body.managerHook.unlink();
}

void CollisionManager::updateBody( ColBody& body )
{
	Vec2f pos = body.object->getPos();

	body.boundBox.min = pos - body.halfSize;
	body.boundBox.max = pos + body.halfSize;

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

void CollisionManager::update()
{
	for( BodyList::iterator iter = mBodies.begin() , itEnd = mBodies.end(); 
		iter != itEnd ; ++iter )
	{
		ColBody& body = *iter;
		updateBody( body );
	}

	for( BodyList::iterator iter = mBodies.begin() , itEnd = mBodies.end(); 
		iter != itEnd ; ++iter )
	{
		ColBody& body = *iter;
		checkCollision( body );
	}
}
