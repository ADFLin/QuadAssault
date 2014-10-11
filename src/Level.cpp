#include "Level.h"

#include "GameInterface.h"
#include "SoundManager.h"

#include "ObjectFactory.h"

#include "Block.h"

#include "LightObject.h"
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


void Level::init( ObjectCreator& creator )
{
	mColManager.setTerrain( mTerrain );
	mObjectCreator = &creator;
}

void Level::cleanup()
{
	destroyAllObjectImpl();

	mPlayers.clear();
	mObjects.clear();

	mLights.clear();
	mBullets.clear();
	mMobs.clear();	
	mItems.clear();
	mParticles.clear();

	mTopMessage = NULL;
	for(int i=0; i<mMsgQueue.size(); i++)
		delete mMsgQueue[i];
	mMsgQueue.clear();

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

void Level::setupTerrain( int w , int h )
{
	mColManager.setup( w * BLOCK_SIZE ,  h * BLOCK_SIZE ,  10 * BLOCK_SIZE );

	mTerrain.resize( w ,  h );
	for(int i=0; i< w ; i++)
	{
		for(int j=0; j< h; j++)
		{		
			Tile& tile = mTerrain.getData( i , j );
			tile.pos  = Vec2f( BLOCK_SIZE * i , BLOCK_SIZE * j );
			tile.type = BID_FLAT;
			tile.meta = 0;
			if(i==0 || j==0 || i== w-1 || j== h-1)
				tile.type = BID_WALL;
		}	
	}
}


void Level::updateRender( float dt )
{
	for( ObjectList::iterator iter = mObjects.begin() ; iter != mObjects.end(); ++iter )
	{
		LevelObject* obj = *iter;
		obj->updateRender( dt );
	}

	RenderLightList& lights = getRenderLights();
	for( RenderLightList::iterator iter = lights.begin() , itEnd = lights.end();
		iter != itEnd ; ++iter )
	{		
		Light* light = *iter;
		light->cachePos = light->offset;
		if ( light->host )
			light->cachePos += light->host->getPos();
	}

	if ( mTopMessage )
		mTopMessage->updateRender( dt );
}

void Level::addOjectInternal( LevelObject* obj )
{
	assert( obj && obj->mLevel == NULL );
	mObjects.push_front( obj );

	obj->mLevel = this;
	obj->onSpawn();
}

Player* Level::createPlayer()
{
	Player* player = new Player;
	player->init();
	player->mPlayerId = mPlayers.size();
	mPlayers.push_back( player );
	addOjectInternal( player );

	return player;
}

Explosion* Level::createExplosion( Vec2f const& pos , float raidus )
{
	Explosion* e = new Explosion();
	e->Init( pos , raidus );
	addOjectInternal( e );
	return e;
}

LightObject* Level::createLight( Vec2f const& pos ,float radius )
{
	LightObject* light = new LightObject();
	light->init();
	light->setPos( pos );
	light->radius = radius;
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

	

	for( PlayerVec::iterator iter = mPlayers.begin() , itEnd = mPlayers.end();
		 iter != itEnd ; ++iter )
	{
		Player* player = *iter;
		IRenderer* renderer = player->getRenderer();
		renderer->render( pass , player );
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

	LevelEvent event;
	event.id     = LevelEvent::eCHANGE_STATE;
	event.intVal = state;
	sendEvent( event );
}

Message* Level::addMessage( Message* msg )
{
	mMsgQueue.push_back( msg );
	return mMsgQueue.back();
}

void Level::sendEvent( LevelEvent const& event )
{
	for( ListenerList::iterator iter = mListeners.begin() , itEnd = mListeners.end();
		iter != itEnd ; ++iter )
	{
		(*iter)->onLevelEvent( event );
	}
}

void Level::addListerner( EventListener& listener )
{
	mListeners.push_back( &listener );
}

Tile* Level::getTile( Vec2f const& pos )
{
	int tx = int( pos.x / BLOCK_SIZE );
	int ty = int( pos.y / BLOCK_SIZE );

	if ( !mTerrain.checkRange( tx , ty ) )
		return NULL;
	return &mTerrain.getData( tx , ty );
}

void Level::addLight( Light& light )
{
	mRenderLights.push_back( &light );
}

void Level::destroyAllObjectImpl()
{
	for( ObjectList::iterator iter = mObjects.begin() , itEnd = mObjects.end();
		iter != itEnd;  )
	{
		LevelObject* obj = *iter;
		++iter;
		delete obj;
	}
}

void Level::destroyAllObject( bool bPlayerIncluded )
{
	if ( !bPlayerIncluded )
	{
		for( PlayerVec::iterator iter = mPlayers.begin();
			iter != mPlayers.end() ; ++iter )
		{
			Player* player = *iter;
			player->baseHook.unlink();
		}
	}

	destroyAllObjectImpl();

	if ( bPlayerIncluded )
	{
		mPlayers.clear();
	}
	else
	{
		for( PlayerVec::iterator iter = mPlayers.begin();
			iter != mPlayers.end() ; ++iter )
		{
			Player* player = *iter;
			addOjectInternal( player );
		}
	}
}

void Level::addObject( LevelObject* object )
{
	assert( object );
	switch( object->getType() )
	{
	case OT_MOB:    mMobs.push_back( object->cast< Mob >() ); break;
	case OT_BULLET: mBullets.push_back( object->cast< Bullet >() ); break;
	case OT_LIGHT:  mLights.push_back( object->cast< LightObject >() ); break;
	case OT_PARTICLE:  mParticles.push_back( object->cast< Particle >() ); break;
	case OT_PLAYER: 
		{

			return;
		}
		break;
	}
	addOjectInternal( object );
	
}

LevelObject* Level::spawnObjectByName( char const* name , Vec2f const& pos )
{
	LevelObject* obj = mObjectCreator->createObject( name );
	if ( !obj )
		return NULL;
	obj->setPos( pos );
	addObject( obj );
	return obj;
}
