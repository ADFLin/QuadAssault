#ifndef RenderEngine_h__
#define RenderEngine_h__

#include "Base.h"
#include "Dependence.h"
#include "FrameAllocator.h"
#include "GBuffer.h"

class ColBody;
typedef std::vector< ColBody* > ColBodyVec;

class Level;
class Object;
class Shader;
class Light;
class IObjectRenderer;

struct TileRange
{
	int xMin;
	int xMax;
	int yMin;
	int yMax;
};

enum RenderMode
{
	RM_ALL = 0 ,
	RM_GEOMETRY  ,
	RM_LINGHTING ,
	RM_NORMAL_MAP ,

	NUM_RENDER_MODE ,
};



struct RenderParam
{
	Level*     level;
	Object*    camera;
	float      scaleFactor;
	RenderMode mode;
	////////////////////////
	TileRange  terrainRange;
	float      renderWidth;
	float      renderHeight;
};


struct RenderGroup
{
	int          order;
	IObjectRenderer*   renderer;
	int          numObject;
	LevelObject* objectLists;
};

class RenderEngine
{
public:
	RenderEngine();
	bool         init( int width , int height );
	void         cleanup();

	void         renderScene( RenderParam& param );
	Vec3f const& getAmbientLight() const { return mAmbientLight; }
	void         setAmbientLight( Vec3f const& color ) { mAmbientLight = color; }

	void         pushRenderState();
	void         popRenderState();

	void         prevRender();
	void         postRender();
private:

	void   renderLightingFBO( RenderParam& param );
	void   renderNormalFBO( RenderParam& param );
	void   renderGeometryFBO( RenderParam& param );
	void   renderSceneFinal( RenderParam& param );

	void   renderTerrain( Level* level , TileRange const& range );
	void   renderTerrainNormal( Level* level , TileRange const& range );
	void   renderTerrainGlow( Level* level , TileRange const& range );
	void   renderTerrainShadow( Level* level , Vec2f const& lightPos , Light* light , TileRange const& range );
	void   renderLight( RenderParam& param , Vec2f const& lightPos , Light* light );
	bool   setupFBO( int width , int height );
	void   setupLightShaderParam( Shader* shader , Light* light );

	void   updateRenderGroup( RenderParam& param );


	void   renderObjects( RenderPass pass , Level* level );

	
	typedef std::vector< RenderGroup* > RenderGroupVec;
	RenderGroupVec mRenderGroups;
	FrameAllocator mAllocator;
	ColBodyVec     mBodyList;

	GBuffer        mGBuffer;
	float   mFrameWidth;
	float   mFrameHeight;

	Shader* mShaderLighting;
	static int const NumMode = 3;
	Shader* mShaderScene[NumMode];

	GLuint mFBOColor;	
	GLuint mFBODepth;
	GLuint mTexLightmap;
	GLuint mTexNormalMap;
	GLuint mTexGeometry;
	Vec3f  mAmbientLight;

};

#endif // RenderEngine_h__
