#include "Collision.h"

#include "Object.h"


ColBody::ColBody()
	:mOffset( 0,0 )
	,halfSize( 0,0 )
{
	idxCell = -1;
	bBBoxDirty = true;
	colMask = COL_ALL;
}

void ColBody::updateBoundBox()
{
	if ( bBBoxDirty )
	{
		boundBox.min = mOffset - halfSize;
		boundBox.max = mOffset + halfSize;
		bBBoxDirty = false;
	}
}

void CollisionManager::init( float length , float width , float cellLength )
{
	int cx = int ( length / cellLength - 1 ) + 1;
	int cy = int ( width / cellLength - 1 ) + 1;
	mCellMap.resize( cx , cy );
	mCellLength = cellLength;
}

bool CollisionManager::testCollision( Vec2f const& pos , ColBody& body , unsigned maskReplace )
{
	unsigned mask = ( maskReplace ) ? maskReplace : body.colMask;

	Vec2f const& posBody = pos + body.getOffset(); 

	int cx , cy;
	calcCellPos( posBody , cx , cy );

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

				if ( &bodyTest == &body )
					continue;

				unsigned mask = body.colMask & bodyTest.colMask;

				if ( mask == 0 )
					continue;

				Vec2f offset = bodyTest.object->getPos() -  body.object->getPos();

				if ( !body.boundBox.intersect( bodyTest.boundBox , offset ) )
					continue;

				return true;
			}
		}
	}

	return false;
}
bool CollisionManager::checkCollision( ColBody& body )
{
	Vec2f posBody = body.object->getPos() + body.getOffset();

	int cx , cy;
	calcCellPos( posBody , cx , cy );

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

				if ( &bodyTest == &body )
					continue;

				unsigned mask = body.colMask & bodyTest.colMask;

				if ( mask == 0 )
					continue;

				Vec2f offset = bodyTest.object->getPos() -  body.object->getPos();

				if ( !body.boundBox.intersect( bodyTest.boundBox , offset ) )
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

	Vec2f posBody = body.object->getPos() + body.getOffset();

	int cx , cy;
	calcCellPos( posBody , cx , cy );
	int idxCell = mCellMap.toIndex( cx , cy );

	body.idxCell = idxCell;
	mCellMap[ idxCell ].bodies.push_back( body );

	mBodies.push_back( body );
}

void CollisionManager::removeBody( ColBody& body )
{
	body.idxCell = -1;
	body.cellHook.unlink();
	body.managerHook.unlink();
}

void CollisionManager::updateBody( ColBody& body )
{
	Vec2f posBody = body.object->getPos() + body.getOffset();

	body.updateBoundBox();
	int cx , cy;
	calcCellPos( posBody , cx , cy );


	int idxCell = mCellMap.toIndex( cx , cy );
	if ( idxCell != body.idxCell )
	{
		body.cellHook.unlink();
		mCellMap[ idxCell ].bodies.push_back( body );
		body.idxCell = idxCell;
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

void CollisionManager::calcCellPos( Vec2f const& pos , int& cx , int& cy )
{
	cx = Math::clamp( int( pos.x / mCellLength ) , 0 , mCellMap.getSizeX() - 1 );
	cy = Math::clamp( int( pos.y / mCellLength ) , 0 , mCellMap.getSizeY() - 1 );
}
