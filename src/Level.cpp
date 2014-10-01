#include "Level.h"

#include "GameInterface.h"
#include "SoundManager.h"

#include "Block.h"

#include "Light.h"
#include "ItemPickup.h"
#include "Bullet.h"
#include "Mob.h"
#include "Player.h"
#include "Particle.h"
#include "Explosion.h"

#include "LaserMob.h"
#include "MinigunMob.h"
#include "PlasmaMob.h"


void Level::init()
{

}

template< class C , class T >
static void remove( C& c , T obj )
{
	c.erase( std::find( c.begin() , c.end() , obj ) );
}

void Level::tick()
{
	for( ObjectList::iterator iter = mObjects.begin() , itEnd = mObjects.end(); 
		  iter != itEnd; ++iter )
	{
		LevelObject* obj = *iter;
		obj->tick();
	}

	
	for( ObjectList::iterator iter = mObjects.begin() , itEnd = mObjects.end(); 
		  iter != itEnd; )
	{
		LevelObject* obj = *iter;
		++iter;

		if( obj->mNeedDestroy )
		{
			obj->onDestroy();
			delete obj;
		}
		else
		{
			obj->postTick();
		}
	}
}

void Level::addOjectInternal( LevelObject* obj )
{
	assert( obj );
	mObjects.push_front( obj );

	obj->mLevel = this;
	obj->onSpawn();
}

Explosion* Level::createExplosion( Vec2f const& pos , float raidus )
{
	Explosion* e = new Explosion();
	e->Init( pos , raidus );
	addOjectInternal( e );
	return e;
}

Light* Level::createLight( Vec2f const& pos ,float radius , bool bStatic )
{
	Light* light = new Light();
	light->init( pos , radius );
	light->isStatic = bStatic;
	mLights.push_back( light );
	addOjectInternal( light );
	return light;
}

Bullet* Level::addBullet( Bullet* bullet )
{
	mBullets.push_back( bullet );
	addOjectInternal( bullet );
	return bullet;
}

ItemPickup* Level::addItem( ItemPickup* item )
{
	mItems.push_back( item );
	addOjectInternal( item );
	return item;
}

Mob* Level::addMob( Mob* mob )
{
	mMobs.push_back( mob );
	addOjectInternal( mob );
	return mob;
}

Particle* Level::addParticle( Particle* particle )
{
	mParticles.push_back( particle );
	addOjectInternal( particle );
	return particle;
}

void Level::cleanup()
{
	for( ObjectList::iterator iter = mObjects.begin() , itEnd = mObjects.end();
		iter != itEnd;  )
	{
		LevelObject* obj = *iter;
		++iter;
		obj->onDestroy();
		delete obj;
	}
	mObjects.clear();

	mLights.clear();
	mBullets.clear();
	mMobs.clear();	
	mItems.clear();
	mParticles.clear();

	mPlayer = NULL;

	//FIXME
	Block::cleanup();
}

int Level::random( int i1, int i2 )
{
	return ::rand()%i2+i1;
}

Mob* Level::spawnMobByName( string const& name , Vec2f const& pos )
{
	Mob* mob = NULL;
	if(name=="Mob.Laser")				
		mob = new LaserMob();	
	else if(name=="Mob.Plasma")				
		mob = new PlasmaMob();	
	else if(name=="Mob.Minigun")		
		mob = new MinigunMob();

	if ( mob )
	{
		mob->init( pos );
		addMob( mob );
	}
	return mob;
}

void Level::renderObjects( RenderPass pass )
{
	for( ItemList::iterator iter = mItems.begin() , itEnd = mItems.end(); 
		iter != itEnd ; ++iter )
	{
		LevelObject* obj = *iter;
		IRenderer* renderer =  obj->getRenderer();
		renderer->render( pass , obj );
	}

	for( MobList::iterator iter = mMobs.begin() , itEnd = mMobs.end(); 
		iter != itEnd ; ++iter )
	{
		LevelObject* obj = *iter;
		IRenderer* renderer =  obj->getRenderer();
		renderer->render( pass , obj );
	}

	for( BulletList::iterator iter = mBullets.begin() , itEnd = mBullets.end(); 
		iter != itEnd ; ++iter )
	{
		LevelObject* obj = *iter;
		IRenderer* renderer =  obj->getRenderer();
		renderer->render( pass , obj );
	}

	for( ParticleList::iterator iter = mParticles.begin() , itEnd = mParticles.end(); 
		iter != itEnd ; ++iter )
	{
		LevelObject* obj = *iter;
		IRenderer* renderer =  obj->getRenderer();
		renderer->render( pass , obj );
	}

	{
		IRenderer* renderer = mPlayer->getRenderer();
		renderer->render( pass , mPlayer );
	}
}

Sound* Level::playSound( char const* name , bool canRepeat /*= false */ )
{
	Sound* sound = getGame()->getSoundMgr()->addSound( name , canRepeat );
	if ( sound )
		sound->play();

	return sound;
}

void Level::updateRender( float dt )
{
	for( ObjectList::iterator iter = mObjects.begin() ; iter != mObjects.end(); ++iter )
	{
		LevelObject* obj = *iter;
		obj->updateRender( dt );
	}
}

void Level::destroyObject( LevelObject* object )
{
	object->onDestroy();
	delete object;
}

void Level::changeState( State state )
{
	if ( mState = state )
		return;

	mState = state;
}

Tile* Level::rayBlockTest( Vec2i const& tPos , Vec2f const& from , Vec2f const& to , unsigned skipFlag )
{
	if ( !mTerrain.checkRange( tPos.x , tPos.y ) )
		return NULL;

	Tile& tile = mTerrain.getData( tPos.x , tPos.y );
	Block* block = Block::FromType( tile.type );

	if ( block->checkFlag( skipFlag ) )
		return NULL;

	if ( block->checkFlag( BF_NONSIMPLE ) )
	{
		if ( !block->rayTest( tile , from , to ) )
			return NULL;
	}

	return &tile;
}

Tile* Level::rayTerrainTest( Vec2f const& from , Vec2f const& to , unsigned skipFlag )
{
	Vec2i tpFrom = Vec2i( Math::floor( from.x / BLOCK_SIZE ) , Math::floor( from.y / BLOCK_SIZE ) );


	Tile* out;
	
	if ( out = rayBlockTest( tpFrom , from , to , skipFlag ) )
		return out;

	Vec2i tpCur  = tpFrom;
	Vec2i tpTo   = Vec2i( Math::floor( to.x / BLOCK_SIZE ) , Math::floor( to.y / BLOCK_SIZE ) );
	Vec2i tpDif  = tpTo - tpFrom;

	if ( tpDif.x == 0 )
	{
		int delta = tpDif.y > 0 ? 1 : -1;
		while( tpCur.y != tpTo.y )
		{
			tpCur.y += delta;
			if ( out = rayBlockTest( tpCur , from , to , skipFlag ) )
				return out;	
		}
	}
	else if ( tpDif.y == 0 )
	{
		int delta = tpDif.x > 0 ? 1 : -1;
		while( tpCur.x != tpTo.x )
		{
			tpCur.x += delta;
			if ( out = rayBlockTest( tpCur , from , to , skipFlag ) )
				return out;	
		}
	}
	else
	{
		Vec2f flac = from / float( BLOCK_SIZE ) - Vec2f( tpFrom );
		Vec2f dif = to - from;
		float slopeFactor = dif.y / dif.x;
		if ( slopeFactor < 0 )
			slopeFactor = -slopeFactor;

        int deltaX = -1;
		if ( tpDif.x > 0 )
		{
			flac.x = 1 - flac.x;
			deltaX = 1;
		}

		int deltaY = -1;
		if ( tpDif.y > 0 )
		{
			flac.y = 1 - flac.y;
			deltaY = 1;
		}

		for(;;)
		{
			float yOff = flac.x * slopeFactor;

			if ( flac.y > yOff )
			{
				flac.y -= yOff;
				flac.x = 1;
				tpCur.x += deltaX;
			}
			else
			{
				flac.x -= flac.y / slopeFactor;
				flac.y = 1;
				tpCur.y += deltaY;
			}

			if ( out = rayBlockTest( tpCur , from , to , skipFlag ) )
				return out;

			if ( tpCur.x == tpTo.x && tpCur.y == tpTo.y )
				break;
		}
	}
	return NULL;
}

Tile* Level::testTerrainCollision( Rect const& bBox , unsigned skipFlag )
{
	TileMap& terrain = getTerrain();

	int xMin = Math::clamp( Math::floor( bBox.min.x / BLOCK_SIZE ) , 0 , terrain.getSizeX() - 1 );
	int xMax = Math::clamp( Math::floor( bBox.max.x / BLOCK_SIZE ) , 0 , terrain.getSizeX() - 1 );
	int yMin = Math::clamp( Math::floor( bBox.min.y / BLOCK_SIZE ) , 0 , terrain.getSizeY() - 1 );
	int yMax = Math::clamp( Math::floor( bBox.max.y / BLOCK_SIZE ) , 0 , terrain.getSizeY() - 1 );

	for( int x = xMin; x <= xMax ; ++x )
	{
		for(int y = yMin; y <= yMax; ++y  )
		{
			Tile& tile = mTerrain.getData( x , y );
			Block* block = Block::FromType( tile.type );

			if ( block->checkFlag( skipFlag ) )
				continue;

			Rect bBoxOther;
			bBoxOther.min=Vec2f(x*BLOCK_SIZE,y*BLOCK_SIZE);
			bBoxOther.max=Vec2f(x*BLOCK_SIZE,y*BLOCK_SIZE)+Vec2f(BLOCK_SIZE,BLOCK_SIZE);

			if( !bBox.intersect(bBoxOther) )
				continue;

			if ( block->checkFlag( BF_NONSIMPLE ) )
			{
				if ( !block->testIntersect( tile , bBox ) )
					continue;
			}

			return &tile;
		}
	}
	return NULL;
}
