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
	OT_ACTOR ,
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

class ObjectClass
{
public:
	ObjectClass( ObjectClass* inParent , char const* inName , ObjectType inType )
		:parent( inParent ) ,name( inName ),type( inType )
	{
		typeBits = BIT( inType );
		if ( parent )
			typeBits |= parent->typeBits ;
	}

	char const* getName(){ return name; }
	ObjectType  getType(){ return type; }

private:
	friend class LevelObject;
	ObjectClass* parent;
	char const*  name;
	ObjectType   type;
	unsigned     typeBits;
	
};


#define DECLARE_OBJECT_CLASS( CLASS , BASE )\
private:\
	typedef CLASS ThisClass;\
	typedef BASE  BaseClass;\
public:\
	static ObjectClass* StaticClass();\
	virtual ObjectClass* getClass(){ return ThisClass::StaticClass(); }

#define IMPL_OBJECT_CLASS( CLASS , TYPE , NAME )\
	ObjectClass* CLASS::StaticClass()\
	{\
		static ObjectClass myClass( BaseClass::StaticClass() , NAME , TYPE );\
		return &myClass;\
	}


#define BEGIN_CLASS_PROP_NOBASE()\
public:\
	void enumProp( IPropEditor& editor ){

#define BEGIN_CLASS_PROP()\
	BEGIN_CLASS_PROP_NOBASE()\
	BaseClass::enumProp( editor );

#define MEMBER_PROP( NAME , MEMBER )\
	editor.addProp( NAME , MEMBER );

#define MEMBER_PROP_F( NAME , MEMBER , FLAG )\
	editor.addProp( NAME , MEMBER , FLAG );

#define MENBER_ENUM_PROP( NAME , MEMBER , NUM , VALUE_SET , STR_SET )\
	editor.addEnumProp( NAME , MEMBER , NUM , VALUE_SET , STR_SET );

#define FUN_PROP( NAME , TYPE , SET , GET )

#define END_CLASS_PROP()\
	}


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

	static  ObjectClass* StaticClass();
	ObjectType   getType(){ return getClass()->getType(); }
	virtual ObjectClass* getClass(){ return StaticClass(); }
	
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
	T* tryCast()
	{ 
		if ( getClass()->typeBits & BIT( T::StaticClass()->getType() ) )
			return static_cast< T* >( this );
		return NULL;
	}
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
	MEMBER_PROP( "Pos" , mPos )
	END_CLASS_PROP()
};


template < class T >
class ClassEditHelperT : public IEditable
{





};

#endif // Object_h__

