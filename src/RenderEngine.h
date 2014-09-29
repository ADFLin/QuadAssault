#ifndef RenderEngine_h__
#define RenderEngine_h__

#include "Base.h"
#include "Dependence.h"

class Level;
class Object;
class Shader;

struct TileRange
{
	int xMin;
	int xMax;
	int yMin;
	int yMax;
};

enum RenderMode
{
	RM_ALL = 0,
	RM_GEOMETRY  ,
	RM_LINGHTING ,
	RM_NORMAL_MAP ,
	
	
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


class RenderEngine
{
public:
	bool        init( int width , int height );
	void        cleanup();
	Shader*     createShader( char const* vsName , char const* fsName );
	void        renderScene( RenderParam& param );
	Vec3 const& getAmbientLight() const { return mAmbientLight; }
	void        setAmbientLight( Vec3 const& color ) { mAmbientLight = color; }

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

	std::vector<Shader*> mShaders;

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

	Vec3   mAmbientLight;

};

#endif // RenderEngine_h__
