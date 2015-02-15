#ifndef ObjectRenderer_h__
#define ObjectRenderer_h__

#include "Renderer.h"
#include "Object.h"

class IObjectRenderer : public IRenderer
{
public:
	IObjectRenderer();

	virtual void render( RenderPass pass , LevelObject* object ) = 0;
	virtual void renderGroup( RenderPass pass , int numObj , LevelObject* object );

	virtual void renderMRT( LevelObject* object ){}
	virtual void renderGroupMRT( int numObj , LevelObject* object );

	int    getOrder(){ return mRenderOrder; }
	static LevelObject* nextObject( LevelObject* obj ){ return obj->renderLink; }

protected:
	int mRenderOrder;

};

#define DEF_OBJ_RENDERER( CLASS , RENDERER )\
	static RENDERER g##CLASS##Renderer;\
	IObjectRenderer* CLASS::getRenderer(){  return &g##CLASS##Renderer;  }

#endif // ObjectRenderer_h__
