#ifndef RenderEngine_h__
#define RenderEngine_h__

#include "Base.h"
#include "Dependence.h"

class Level;
class Object;
class Shader;
class IRenderer;

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
	bool       updateObjects;
};


struct RenderGroup
{
	int          order;
	IRenderer*   renderer;
	LevelObject* objectLists;
};

class RenderEngine
{
public:
	bool         init( int width , int height );
	void         cleanup();
	Shader*      createShader( char const* vsName , char const* fsName );
	void         renderScene( RenderParam& param );
	Vec3f const& getAmbientLight() const { return mAmbientLight; }
	void         setAmbientLight( Vec3f const& color ) { mAmbientLight = color; }

	void         pushRenderState();
	void         popRenderState();

	void         prevRender();
	void         postRender();
private:

	void   renderLightFBO( RenderParam& param );
	void   renderNormalFBO( RenderParam& param );
	void   renderGeometryFBO( RenderParam& param );
	void   renderSceneFinal( RenderParam& param );

	void   renderTerrain( Level* level , TileRange const& range );
	void   renderTerrainNormal( Level* level , TileRange const& range );
	void   renderTerrainGlow( Level* level , TileRange const& range );
	void   renderTerrainShadow( Level* level , Light* light , TileRange const& range );

	void   renderLighting( RenderParam& param , Light* light );
	bool   setupFBO( int width , int height );
	void   setupLightShaderParam( Shader* shader , Light* light );

	struct GroupCompFun
	{
		bool operator()( RenderGroup* a , RenderGroup* b ) const 
		{
			return a->order > b->order;
		}
	};
	void   updateRenderGroup( RenderParam& param );


	void   renderObjects( RenderPass pass , Level* level );

	std::vector<Shader*> mShaders;

	typedef std::vector< RenderGroup* > RenderGroupVec;
	RenderGroupVec mRenderGroups;

	float   mFrameWidth;
	float   mFrameHeight;

	Shader* mShaderLighting;
	static int const NumMode = 3;
	Shader* mShaderScene[NumMode];

	GLuint mFBOColor;	
	GLuint mFBOStencil;
	GLuint mTexLightmap;
	GLuint mTexNormalMap;
	GLuint mTexGeometry;

	Vec3f   mAmbientLight;

};

#endif // RenderEngine_h__
