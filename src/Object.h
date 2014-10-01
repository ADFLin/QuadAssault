#ifndef Object_h__
#define Object_h__

#include "Base.h"
#include "IntrList.h"

class Object
{
public:
	Object();
	virtual ~Object(){}
	
	Vec2f const& getPos() const { return mPos; }
	void         setPos(Vec2f const& pos){ mPos = pos; }

protected:
	Vec2f mPos;
		
};

class Level;
class PropEditor;

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


class IRenderer;
class ColBody;

class LevelObject : public Object
{
public:
	LevelObject();
	
	virtual ObjectType getType() = 0;

	virtual void onSpawn(){}
	virtual void onDestroy(){}
	virtual void tick(){}
	virtual void postTick(){}
	virtual void updateRender( float dt ){}
	
	virtual void render( RenderPass pass ){}
	virtual void enumProp( PropEditor& editor ){}
	virtual void onCollision( ColBody& self , ColBody& other ){}

	virtual IRenderer* getRenderer(){ return NULL; }

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

	Vec2f mSize;
	friend class Level;

	HookNode baseHook;
	HookNode typeHook;

	Level*   mLevel;
	bool     mNeedDestroy;
};

class IRenderer
{
public:
	IRenderer();
	virtual void init() = 0;
	virtual void render( RenderPass pass , LevelObject* object ) = 0;
	virtual void renderDev( LevelObject* object ){}

	static void cleanup();
	static void initialize();

	IRenderer* mLink;
};

#define DEFINE_RENDERER( CLASS , RENDERER )\
	static RENDERER g##CLASS##Renderer;\
	IRenderer* CLASS::getRenderer(){  return &g##CLASS##Renderer;  }

#endif // Object_h__

