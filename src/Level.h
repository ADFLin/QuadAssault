#ifndef Level_h__
#define Level_h__

#include "Collision.h"
#include "ObjectFactory.h"
#include "Block.h"
#include "Object.h"
#include "Light.h"

#include "TGrid2D.h"
#include "IntrList.h"

#include <list>
#include <vector>

class LevelObject;
class Mob;
class LightObject;
class ItemPickup;
class Particle;
class Bullet;
class Explosion;
class Player;
class Sound;
class Message;

class TextureManager;

struct LevelEvent
{
	enum EventId
	{
		ePLAYER_DEAD ,
		eCHANGE_STATE ,
	};

	EventId id;
	union
	{
		int intVal;
	};
};

class Level
{
public:

	Level();

	void              setupTerrain( int w , int h );
	void              init( ObjectCreator& creator );
	void              tick();
	void              updateRender( float dt );
	void              cleanup();

	enum State
	{
		eRUNNING  ,
		eFINISH   ,
		eFREEZE   ,
	};

	class EventListener
	{
	public:
		virtual ~EventListener(){}
		virtual void onLevelEvent( LevelEvent const& event ){}
	};
	State             getState(){ return mState; }
	void              changeState( State state );

	

	TileMap&          getTerrain(){ return mTerrain; }
	CollisionManager& getColManager(){  return mColManager; }

	void              destroyAllObject( bool skipPlayer );

	Player*           createPlayer();
	Player*           getPlayer( int id = 0 ){  return mPlayers[id];  }

	Explosion*        createExplosion( Vec2f const& pos , float raidus );

	// Create Light Object
	LightObject*      createLight( Vec2f const& pos ,float radius );
	void              addLight( Light& light );

	Bullet*           addBullet( Bullet* bullet );
	ItemPickup*       addItem( ItemPickup* item );
	Mob*              addMob( Mob* mob );
	Particle*         addParticle(Particle* particle );
	void              addObject( LevelObject* object );
	LevelObject*      spawnObjectByName( char const* name , Vec2f const& pos );
	void              destroyObject( LevelObject* object );

	Sound*            playSound( char const* name , bool canRepeat = false );
	Message*          addMessage(Message* msg );

	Message*          getTopMessage(){ return mTopMessage; }
	
	void              renderObjects( RenderPass pass );
	void              renderDev( DevDrawMode mode );

	void              addListerner( EventListener& listener );
	void              sendEvent( LevelEvent const& event );


	int               random(int i1, int i2);

	Tile*             getTile( Vec2f const& pos );
	LevelObject*      hitObjectTest( Vec2f const& pos );

	unsigned          setSpwanDestroyFlag( unsigned flag );
	unsigned          getSpawnDestroyFlag(){ return mSpwanDestroyFlag; }

	
public:

	

	typedef MemberHook< LevelObject , &LevelObject::baseHook > ObjHook;
	typedef MemberHook< LevelObject , &LevelObject::typeHook > TypeHook;

	typedef IntrList< LightObject  , TypeHook , PointerType > LightList;
	typedef IntrList< Bullet , TypeHook , PointerType > BulletList;
	typedef IntrList< Mob    , TypeHook , PointerType > MobList;

	typedef IntrList< Light , MemberHook< Light , &Light::mHook > , PointerType > RenderLightList;

	MobList&         getMobs()   { return mMobs; }
	BulletList&      getBullets(){ return mBullets; }
	LightList&       getLights() { return mLights; }
	RenderLightList& getRenderLights(){ return mRenderLights; }

protected:
	typedef IntrList< LevelObject , ObjHook , PointerType > ObjectList;
	typedef IntrList< ItemPickup  , TypeHook , PointerType > ItemList;
	typedef IntrList< Particle , TypeHook , PointerType > ParticleList;

	void       addObjectInternal( LevelObject* obj );
	void       destroyAllObjectImpl();

	RenderLightList mRenderLights;

	typedef std::vector< Message* > MessageVec;

	ObjectCreator*   mObjectCreator;
	MessageVec       mMsgQueue;
	Message*         mTopMessage;
	State            mState;

	ObjectList       mObjects;
	MobList          mMobs;
	BulletList       mBullets;
	ItemList         mItems;
	LightList        mLights;
	ParticleList     mParticles;
	
	typedef std::vector< Player* > PlayerVec;
	PlayerVec        mPlayers;
	TileMap          mTerrain;
	CollisionManager mColManager;

	unsigned         mSpwanDestroyFlag;


	typedef std::vector< EventListener* > ListenerList;
	ListenerList     mListeners;
};

typedef Level::RenderLightList RenderLightList;
typedef Level::LightList LightList;


#endif // Level_h__
