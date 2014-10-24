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
	unsigned     typeBit;
	ObjectClass* parent;
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



#define BEGIN_CLASS_PROP_NOBASE()\
public:\
	void enumProp( IPropEditor& editor ){

#define BEGIN_CLASS_PROP()\
public:\
	void enumProp( IPropEditor& editor ){\
	BaseClass::enumProp( editor );

#define MEMBER_PROP( NAME , MEMBER )\
	editor.addProp( NAME , MEMBER );

#define MEMBER_PROP_F( NAME , MEMBER , FLAG )\
	editor.addProp( NAME , MEMBER , FLAG );

#define MENBER_ENUM_PROP( NAME , MEMBER , NUM , VALUE_SET , STR_SET )\
	editor.addEnumProp( NAME , MEMBER , NUM , VALUE_SET , STR_SET );

#define END_CLASS_PROP() }

class LevelObject : public Object
{
	typedef Object BaseClass;
public:
	LevelObject();
	LevelObject( Vec2f const& pos );
	
	//virtual ObjectClass* getClass(){}
	virtual ObjectType   getType() = 0;
	virtual void init(){}
	virtual void onSpawn( unsigned flag ){}
	virtual void onDestroy( unsigned flag ){}
	virtual void tick(){}
	virtual void postTick(){}
	virtual void updateRender( float dt ){}
	
	virtual void renderDev( DevDrawMode mode ){}
	

	virtual void onTileCollision( ColBody& self , Tile& tile ){}
	virtual void onBodyCollision( ColBody& self , ColBody& other ){}

	virtual IObjectRenderer* getRenderer(){ return NULL; }

	Level* getLevel(){ return mLevel; }
	void   destroy(){ mNeedDestroy = true; }

	template< class T >
	T* cast()
	{ 
		assert( dynamic_cast< T* >( this ) );
		return static_cast< T* >( this );
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

	BEGIN_CLASS_PROP_NOBASE()
	MEMBER_PROP( "Pos" , mPos );
	END_CLASS_PROP()
};

#define DECLARE_OBJECT_CLASS( CLASS , BASE )\
private:\
	typedef CLASS ThisClass;\
	typedef BASE  BaseClass;\



#define DEFINE_OBJECT_TYPE( CLASS , TYPE , NAME )

template < class T >
class ClassEditHelperT : public IEditable
{





};

#endif // Object_h__

