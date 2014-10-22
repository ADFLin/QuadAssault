#ifndef Object_h__
#define Object_h__

#include "Base.h"
#include "IntrList.h"
#include "GameEdit.h"

class Object : public IEditable
{
public:
	Object();
	Object( Vec2f const& pos );
	virtual ~Object(){}

	Vec2f const&  getPos() const { return mPos; }
	void          setPos(Vec2f const& pos){ mPos = pos; }

protected:
	Vec2f mPos;
		
};

class Level;

enum ObjectType
{
	OT_OBJECT ,
	OT_BULLET ,
	OT_EXPLOSION ,
	OT_LIGHT ,
	OT_MOB ,
	OT_PLAYER ,
	OT_ITEM ,
	OT_PARTICLE ,
	OT_TRIGGER ,
};

class IObjectRenderer;
struct Tile;
class ColBody;

//TODO: Contruct Object RTTI System
class ObjectClass
{
	char const*  name;
	ObjectType   type;
	ObjectClass* parent;
	IObjectRenderer*   renderer;
};

enum DevDrawMode
{
	DDM_EDIT ,
	DDM_COLLISION ,
};

enum SpawnDestroyFlag
{
	SDF_SETUP_DEFAULT = BIT(0),
	SDF_CAST_EFFECT   = BIT(1),
	SDF_LOAD_LEVEL    = BIT(2),
};

class LevelObject : public Object
{
	typedef Object BaseClass;
public:
	LevelObject();
	LevelObject( Vec2f const& pos );
	
	virtual ObjectType getType() = 0;
	virtual void init(){}
	virtual void onSpawn( unsigned flag ){}
	virtual void onDestroy( unsigned flag ){}
	virtual void tick(){}
	virtual void postTick(){}
	virtual void updateRender( float dt ){}
	
	virtual void renderDev( DevDrawMode mode ){}
	virtual void enumProp( IPropEditor& editor );

	virtual void onTileCollision( ColBody& self , Tile& tile ){}
	virtual void onBodyCollision( ColBody& self , ColBody& other ){}

	virtual IObjectRenderer* getRenderer(){ return NULL; }

	Level* getLevel(){ return mLevel; }
	void   destroy(){ mNeedDestroy = true; }

	template< class T >
	T* cast()
	{ 
#if _DEBUG
		return dynamic_cast< T* >( this );
#else
		return static_cast< T* >( this );
#endif
	}

	Vec2f        getRenderPos() const { return mPos - mSize / 2; }
	Vec2f const& getSize() const { return mSize; }
	void         setSize(Vec2f const& size ){ mSize = size; }

	void         calcBoundBox( Rect& bBox );

protected:
	Vec2f    mSize;
	Level*   mLevel;
	bool     mNeedDestroy;

private:
	friend class Level;
	HookNode baseHook;
	HookNode typeHook;

private:
	friend class RenderEngine;
	friend class IObjectRenderer;
	LevelObject* renderLink;
};


#endif // Object_h__

