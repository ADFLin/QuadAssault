#ifndef Level_h__
#define Level_h__

#include "Collision.h"
#include "Block.h"
#include "Object.h"
#include "Light.h"

#include "TGrid2D.h"
#include "IntrList.h"

#include <list>
#include <vector>

class LevelObject;
class Mob;
class Light;
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

	void              init();
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

	void              setupTerrain( int w , int h );

	TileMap&          getTerrain(){ return mTerrain; }
	CollisionManager& getColManager(){  return mColManager; }


	Player*           getPlayer(){  return mPlayer;  }

	Explosion*        createExplosion( Vec2f const& pos , float raidus );
	Light*            createLight( Vec2f const& pos , float radius , bool bStatic );

	Bullet*           addBullet( Bullet* bullet );
	ItemPickup*       addItem( ItemPickup* item );
	Mob*              addMob( Mob* mob );
	Particle*         addParticle(Particle* particle );

	Mob*              spawnMobByName(string const& name , Vec2f const& pos );

	Sound*            playSound( char const* name , bool canRepeat = false );
	Message*          addMessage(Message* msg );

	Message*          getTopMessage(){ return mTopMessage; }
	
	void              renderObjects( RenderPass pass );

	void              addListerner( EventListener& listener );
	void              sendEvent( LevelEvent const& event );


	int               random(int i1, int i2);

	
public:

	void       addOjectInternal( LevelObject* obj );

	typedef MemberHook< LevelObject , &LevelObject::baseHook > ObjHook;
	typedef MemberHook< LevelObject , &LevelObject::typeHook > TypeHook;

	typedef IntrList< Light  , TypeHook , PointerType > LightList;
	typedef IntrList< Bullet , TypeHook , PointerType > BulletList;
	typedef IntrList< Mob    , TypeHook , PointerType > MobList;

	MobList&    getMobs()   { return mMobs; }
	BulletList& getBullets(){ return mBullets; }
	LightList&  getLights() { return mLights; }

protected:
	typedef IntrList< LevelObject , ObjHook , PointerType > ObjectList;
	typedef IntrList< ItemPickup  , TypeHook , PointerType > ItemList;
	typedef IntrList< Particle , TypeHook , PointerType > ParticleList;

	void       destroyObject( LevelObject* object );
	

	typedef std::vector< Message* > MessageVec;

	MessageVec       mMsgQueue;
	Message*         mTopMessage;
	State            mState;

	ObjectList       mObjects;
	MobList          mMobs;
	BulletList       mBullets;
	ItemList         mItems;
	LightList        mLights;
	ParticleList     mParticles;
	
	Player*          mPlayer;
	TileMap          mTerrain;
	CollisionManager mColManager;


	typedef std::vector< EventListener* > ListenerList;
	ListenerList     mListeners;
};

typedef Level::LightList  LightList;


#endif // Level_h__
