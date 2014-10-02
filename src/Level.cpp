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


Level::Level() 
{
	mTopMessage = NULL;
}


void Level::init()
{
	Block::initialize( this );
	mColManager.setTerrain( mTerrain );

	mPlayer = new Player();
	mPlayer->init();
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

	mTopMessage = NULL;
	for(int i=0; i<mMsgQueue.size(); i++)
		delete mMsgQueue[i];
	mMsgQueue.clear();

	//FIXME
	Block::cleanup();
}

void Level::tick()
{
	for( ObjectList::iterator iter = mObjects.begin() , itEnd = mObjects.end(); 
		  iter != itEnd; ++iter )
	{
		LevelObject* obj = *iter;
		obj->tick();
	}

	mColManager.update();
	
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

	if ( mTopMessage == NULL )
	{
		if ( !mMsgQueue.empty() )
		{
			mTopMessage = mMsgQueue.front();
			mTopMessage->nodifyShow();
		}
	}
	if ( mTopMessage )
	{
		mTopMessage->tick();
		if ( mTopMessage->unisten )
		{
			delete mTopMessage;
			mMsgQueue.erase(mMsgQueue.begin());
			mTopMessage = NULL;
		}
	}
}

void Level:: setupTerrain( int w , int h )
{
	mColManager.setup( w * BLOCK_SIZE ,  h * BLOCK_SIZE ,  10 * BLOCK_SIZE );

	mTerrain.resize( w ,  h );
	for(int i=0; i< w ; i++)
	{
		for(int j=0; j< h; j++)
		{		
			Tile& tile = mTerrain.getData( i , j );
			tile.pos  = Vec2f( BLOCK_SIZE * i , BLOCK_SIZE * j );
			tile.type = TID_FLAT;
			tile.meta = 0;
			if(i==0 || j==0 || i== w-1 || j== h-1)
				tile.type = TID_WALL;
		}	
	}
	addOjectInternal( mPlayer );
}


void Level::updateRender( float dt )
{
	for( ObjectList::iterator iter = mObjects.begin() ; iter != mObjects.end(); ++iter )
	{
		LevelObject* obj = *iter;
		obj->updateRender( dt );
	}

	if ( mTopMessage )
		mTopMessage->updateRender( dt );
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

Message* Level::addMessage( Message* msg )
{
	mMsgQueue.push_back( msg );
	return mMsgQueue.back();
}
