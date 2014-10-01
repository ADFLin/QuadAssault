#ifndef Level_h__
#define Level_h__

#include "TGrid2D.h"
#include "IntrList.h"

#include "Block.h"
#include "Object.h"
#include "Light.h"

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

typedef TGrid2D< Tile , FastMapping > TileMap;

namespace sf
{
	class Font;
}



class Level
{
public:


	void              init();
	void              tick();
	void              updateRender( float dt );
	void              cleanup();

	void              restart()
	{





	}


	enum State
	{
		eRunning ,
		eFinish   ,
		eFreeze  ,
	};


	class Listener
	{
	public:
		//virtual ~EventListener(){}
		virtual void onChangeState( State state ){}
	};
	State             getState(){ return mState; }
	void              changeState( State state );

	TileMap&          getTerrain(){ return mTerrain; }


	Player*           getPlayer(){  return mPlayer;  }

	Explosion*        createExplosion( Vec2f const& pos , float raidus );
	Light*            createLight( Vec2f const& pos , float radius , bool bStatic );

	Bullet*           addBullet( Bullet* bullet );
	ItemPickup*       addItem( ItemPickup* item );
	Mob*              addMob( Mob* mob );
	Particle*         addParticle(Particle* particle );

	Mob*              spawnMobByName(string const& name , Vec2f const& pos );

	Sound*            playSound( char const* name , bool canRepeat = false );
	virtual Message*  addMessage( Message* p ) = 0;
	

	int               random(int i1, int i2);

	void              renderObjects( RenderPass pass );

	Tile*            rayTerrainTest( Vec2f const& from , Vec2f const& to , unsigned skipFlag );
	Tile*            rayBlockTest( Vec2i const& tPos , Vec2f const& from , Vec2f const& to , unsigned skipFlag );
	Tile*            testTerrainCollision( Rect const& bBox , unsigned skipFlag );
	
public:
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
	void       addOjectInternal( LevelObject* obj );



	Listener*    mListener;
	ObjectList   mObjects;
	MobList      mMobs;
	BulletList   mBullets;
	ItemList     mItems;
	LightList    mLights;
	ParticleList mParticles;

	State           mState;
	Player*         mPlayer;
	TileMap         mTerrain;
};

typedef Level::MobList    MobList;
typedef Level::BulletList BulletList;
typedef Level::LightList  LightList;


#endif // Level_h__
