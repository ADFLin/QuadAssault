#include "RenderEngine.h"

#include "Shader.h"
#include "Level.h"
#include "Light.h"
#include "Block.h"

#include "DataPath.h"

bool RenderEngine::init( int width , int height )
{
	mAmbientLight=Vec3(0.1f, 0.1f, 0.1f);

	mFrameWidth  = width;
	mFrameHeight = height;

	if ( !setupFBO( width , height ) )
		return false;

	mShaderLighting = createShader( "Light.vp", "Light.fp" );
	mShaderScene[ RM_ALL ] = createShader( "RenderScene.vp", "RenderScene.fp" );
	mShaderScene[ RM_GEOMETRY  ] = createShader( "RenderScene.vp", "RenderSceneGeometry.fp" );
	mShaderScene[ RM_LINGHTING ] = createShader( "RenderScene.vp", "RenderSceneLighting.fp" );


	return true;
}

bool RenderEngine::setupFBO( int width , int height )
{
	glGenFramebuffers(1,&mFBOColor);

	glGenTextures(1,&mTexLightmap);
	glBindTexture(GL_TEXTURE_2D,mTexLightmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width , height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1,&mTexNormalMap);
	glBindTexture(GL_TEXTURE_2D,mTexNormalMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width ,height ,
		0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1,&mTexGeometry);
	glBindTexture(GL_TEXTURE_2D,mTexGeometry);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width ,height ,
		0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenRenderbuffers(1, &mFBOStencil );  
	glBindRenderbuffer(GL_RENDERBUFFER, mFBOStencil );  
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8 , width , height);  
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width , height );

	return true;
}

void RenderEngine::cleanup()
{
	for(int i=0; i<mShaders.size(); i++)
		delete mShaders[i];

	mShaderLighting = NULL;
	for( int i = 0 ; i < NumMode ; ++i )
		mShaderScene[i] = NULL;
	mShaders.clear();	

	glDeleteFramebuffers(1,&mFBOColor);
	glDeleteTextures(1,&mTexLightmap);
	glDeleteTextures(1,&mTexNormalMap);
	glDeleteTextures(1,&mTexGeometry);
}

Shader* RenderEngine::createShader( char const* vsName , char const* fsName )
{
	string vsPath = SHADER_DIR;
	vsPath += vsName;
	string fsPath = SHADER_DIR;
	fsPath += fsName;
	Shader* shader = new Shader( vsPath.c_str() , fsPath.c_str() );
	mShaders.push_back( shader );
	return shader;
}


void RenderEngine::renderScene( RenderParam& param )
{

	param.renderWidth  = mFrameWidth * param.scaleFactor;
	param.renderHeight = mFrameHeight * param.scaleFactor;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0 , param.renderWidth , param.renderHeight  ,0,0,1);

	glMatrixMode( GL_MODELVIEW );
	

	TileRange& renderRange = param.terrainRange;

	renderRange.xMin = int( param.camera->getRenderPos().x/BLOCK_SIZE )-1;
	renderRange.yMin = int( param.camera->getRenderPos().y/BLOCK_SIZE )-1;
	renderRange.xMax = renderRange.xMin + int( param.renderWidth / BLOCK_SIZE ) + 3;
	renderRange.yMax = renderRange.yMin + int( param.renderHeight / BLOCK_SIZE ) + 3;

	TileMap& terrain = param.level->getTerrain();

	renderRange.xMin = Math::clamp( renderRange.xMin , 0 , terrain.getSizeX() );
	renderRange.xMax = Math::clamp( renderRange.xMax , 0 , terrain.getSizeX() );
	renderRange.yMin = Math::clamp( renderRange.yMin , 0 , terrain.getSizeY() );
	renderRange.yMax = Math::clamp( renderRange.yMax , 0 , terrain.getSizeY() );

	switch( param.mode )
	{
	case RM_ALL:
		renderGeometryFBO( param );
		renderLightFBO( param );	
		break;
	case RM_GEOMETRY:
		renderGeometryFBO( param );
		break;
	case RM_LINGHTING:
		renderLightFBO( param );
		break;
	case RM_NORMAL_MAP:
		renderNormalFBO( param );

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );

		glEnable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexNormalMap );

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f(0.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex2f( mFrameWidth , 0.0);
		glTexCoord2f(1.0, 0.0); glVertex2f( mFrameWidth , mFrameHeight );
		glTexCoord2f(0.0, 0.0); glVertex2f(0.0 , mFrameHeight );
		glEnd();

		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
		return;
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );

	renderSceneFinal( param );
}

void RenderEngine::renderSceneFinal( RenderParam& param )
{
	Shader* shader = mShaderScene[ param.mode ];

	shader->bind();
	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexGeometry);
	shader->setParam( "texGeometry" , 0 );

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTexLightmap);
	shader->setParam( "texLightmap" , 1 );
	shader->setParam( "ambientLight" , mAmbientLight );

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex2f(0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex2f( mFrameWidth , 0.0);
	glTexCoord2f(1.0, 0.0); glVertex2f( mFrameWidth , mFrameHeight );
	glTexCoord2f(0.0, 0.0); glVertex2f(0.0 , mFrameHeight );
	glEnd();

	shader->unbind();

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
}

void RenderEngine::renderTerrain( Level* level , TileRange const& range )
{
	TileMap& terrain = level->getTerrain();
	for(int i = range.xMin; i < range.xMax ; ++i )
	for(int j = range.yMin; j < range.yMax; ++j )
	{
		Tile const& tile = terrain.getData( i , j );
		Block::FromType( tile.type )->render( tile );
	}
}

void RenderEngine::renderTerrainNormal( Level* level , TileRange const& range )
{
	TileMap& terrain = level->getTerrain();
	for(int i = range.xMin; i < range.xMax ; ++i )
	for(int j = range.yMin; j < range.yMax; ++j )
	{		
		Tile const& tile = terrain.getData( i , j );
		Block::FromType( tile.type )->renderNormal( tile );
	}
}

void RenderEngine::renderTerrainGlow( Level* level , TileRange const& range )
{
	TileMap& terrain = level->getTerrain();
	for(int i = range.xMin; i < range.xMax ; ++i )
	for(int j = range.yMin; j < range.yMax; ++j )
	{
		Tile const& tile = terrain.getData( i , j );
		Block::FromType( tile.type )->renderGlow( tile );
	}
}


void RenderEngine::renderLighting( RenderParam& param , Light* light )
{
	mShaderLighting->bind();

	Vec2f posLight = light->getRenderPos() - param.camera->getRenderPos();

	mShaderLighting->setParam( "posLight" , posLight );

	//mShaderLighting->setParam( "frameHeight", mFrameHeight );
	//mShaderLighting->setParam( "scaleFactor" , param.scaleFactor );
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexNormalMap );
	mShaderLighting->setParam( "texNormalMap" , 0 );	
	
	light->setupShader( mShaderLighting );

	glColor3f(1,1,1);	

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex2f( 0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex2f( param.renderWidth , 0.0);
	glTexCoord2f(1.0, 0.0); glVertex2f( param.renderWidth , param.renderHeight );
	glTexCoord2f(0.0, 0.0); glVertex2f( 0.0 , param.renderHeight );
	glEnd();	

	mShaderLighting->unbind();	
	glActiveTexture(GL_TEXTURE0);
}

void RenderEngine::renderGeometryFBO( RenderParam& param )
{
	glBindFramebuffer(GL_FRAMEBUFFER ,mFBOColor);		
	glFramebufferTexture2D(GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , 
		GL_TEXTURE_2D, mTexGeometry, 0); 	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glLoadIdentity();	

	glPushMatrix();
	//Sprite(Vec2(0,0),Vec2(igra->DajRW()->getSize().x, igra->DajRW()->getSize().y),mt->DajTexturu(1)->id);
	glTranslatef( - param.camera->getRenderPos().x, - param.camera->getRenderPos().y, 0);			

	renderTerrain( param.level , param.terrainRange );
	param.level->renderObjects(RP_DIFFUSE);	

	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER ,0);
}

void RenderEngine::renderNormalFBO( RenderParam& param )
{
	glBindFramebuffer( GL_FRAMEBUFFER ,mFBOColor);		
	glFramebufferTexture2D(GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , 
		GL_TEXTURE_2D, mTexNormalMap, 0); 	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glLoadIdentity();	

	glPushMatrix();	

	glTranslatef( - param.camera->getRenderPos().x, - param.camera->getRenderPos().y, 0);			

	//Sprite(Vec2(0,0),Vec2(igra->DajRW()->getSize().x, igra->DajRW()->getSize().y),mt->DajTexturu(1)->id);
	renderTerrainNormal( param.level , param.terrainRange );		
	param.level->renderObjects( RP_NORMAL );

	glPopMatrix();
	glBindFramebufferEXT(GL_FRAMEBUFFER ,0);
}



void RenderEngine::renderLightFBO( RenderParam& param )
{
	renderNormalFBO( param );

	glBindFramebuffer(GL_FRAMEBUFFER ,mFBOColor);		
	glFramebufferTexture2D(GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, mTexLightmap, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mFBOStencil ); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mFBOStencil ); 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor( mAmbientLight.x , mAmbientLight.y , mAmbientLight.z , 1.0f);
	
	glLoadIdentity();	

	glEnable(GL_BLEND);	
	glBlendFunc(GL_ONE,GL_ONE);

	//RENDERIRA SE SVE GLOW

	Object* camera = param.camera;


	glPushMatrix();
	glTranslatef(-camera->getRenderPos().x, -camera->getRenderPos().y, 0);
	renderTerrainGlow( param.level , param.terrainRange );
	param.level->renderObjects(RP_GLOW);
	glPopMatrix();

	float w = param.renderWidth;
	float h = param.renderHeight;

	Shader* shader = mShaderLighting;

	LightList& lights = param.level->getLights();
	TileMap& terrain = param.level->getTerrain();

	for( LightList::iterator iter = lights.begin() , itEnd = lights.end();
		iter != itEnd ; ++iter )
	{		
		Light* light = *iter;

		if( light->getRenderPos().x + light->radius < camera->getRenderPos().x ||
			light->getRenderPos().x - light->radius > camera->getRenderPos().x + w ||
			light->getRenderPos().y + light->radius < camera->getRenderPos().y || 
			light->getRenderPos().y - light->radius > camera->getRenderPos().y + h )
			continue;

		if ( light->drawShadow )
		{
			glEnable( GL_STENCIL_TEST );
			glClear(GL_STENCIL_BUFFER_BIT);

#if 1
			glColorMask(false, false, false, false);
#else
			glColor3f( 0.1 , 0.1 , 0.1 );
#endif
			glStencilFunc(GL_ALWAYS, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			
			glPushMatrix();
			glTranslatef(-camera->getRenderPos().x, -camera->getRenderPos().y, 0);

			TileRange range = param.terrainRange;

			int tx = int( light->getRenderPos().x / BLOCK_SIZE );
			int ty = int( light->getRenderPos().y / BLOCK_SIZE );

			if ( tx < range.xMin )
				range.xMin = tx;
			else if ( tx > range.xMax )
				range.yMax = tx + 1;

			if ( ty < range.yMin )
				range.yMin = ty;
			else if ( ty > range.yMax )
				range.yMax = ty + 1;

			range.xMin = Math::clamp( range.xMin , 0 , terrain.getSizeX() );
			range.xMax = Math::clamp( range.xMax , 0 , terrain.getSizeX() );
			range.yMin = Math::clamp( range.yMin , 0 , terrain.getSizeY() );
			range.yMax = Math::clamp( range.yMax , 0 , terrain.getSizeY() );

			renderTerrainShadow( param.level , light , range );

#if 1
			glStencilFunc(GL_ALWAYS, 0, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			for(int i = param.terrainRange.xMin; i < param.terrainRange.xMax; ++i )
			for(int j = param.terrainRange.yMin; j < param.terrainRange.yMax; ++j )
			{
				Tile const& tile = terrain.getData( i , j );
				Block* block = Block::FromType( tile.type );
				if ( block->checkFlag( BF_CAST_SHADOW ) )
					block->renderNoTexture( tile );
			}
#endif

			glPopMatrix();
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glStencilFunc(GL_EQUAL, 0, 1);
			glColorMask(true, true, true, true);
		}
		else
		{
			glDisable( GL_STENCIL_TEST );
		}

		renderLighting( param , light);
	}

	glDisable( GL_STENCIL_TEST );
	glDisable(GL_BLEND);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0 ); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0 ); 
	glBindFramebuffer( GL_FRAMEBUFFER ,0);
}

static int const offsetX[4] = {-1,0,1,0};
static int const offsetY[4] = { 0,-1,0,1};
static Vec2f const tileVertex[4] = { Vec2f(0,0) , Vec2f(BLOCK_SIZE,0) , Vec2f(BLOCK_SIZE,BLOCK_SIZE ) , Vec2f(0,BLOCK_SIZE)   };
static Vec2f const tileNormal[4] = { Vec2f(-1,0) , Vec2f(0,-1) , Vec2f(1,0) , Vec2f(0,1) };

void RenderEngine::renderTerrainShadow( Level* level , Light* light , TileRange const& range )
{
	TileMap& terrain = level->getTerrain();


	for(int i = range.xMin; i < range.xMax ; ++i )
	{
		for(int j = range.yMin; j < range.yMax; ++j )
		{
			Tile const& tile = terrain.getData( i , j );
			Block* block = Block::FromType( tile.type );

			if ( !block->checkFlag( BF_CAST_SHADOW ) )
				continue;

			Vec2f tileOffset = tile.pos - light->getRenderPos();

			for( int idxCur = 0 , idxPrev = 3; idxCur < 4; idxPrev = idxCur , ++idxCur )
			{
				int nx = i + offsetX[idxCur];
				int ny = j + offsetY[idxCur];

#if 1
				if ( terrain.isVaildRange( nx , ny ) && 
					Block::FromType( terrain.getData( nx , ny ).type )->checkFlag( BF_CAST_SHADOW )  )
					continue;
#endif
				Vec2f offsetCur  = tileVertex[ idxCur ]  + tileOffset;
				
				if ( offsetCur.dot( tileNormal[ idxCur ] ) < 0 )
				{
					Vec2f offsetPrev = tileVertex[ idxPrev ] + tileOffset;

					Vec2f const& cur  = tile.pos + tileVertex[ idxCur ];
					Vec2f const& prev = tile.pos + tileVertex[ idxPrev ];

					Vec2f v1 = light->getRenderPos() + 5000 * offsetPrev;
					Vec2f v2 = light->getRenderPos() + 5000 * offsetCur;

					glBegin( GL_QUADS );
					glVertex2f( prev.x , prev.y );
					glVertex2f( v1.x , v1.y );
					glVertex2f( v2.x , v2.y  );
					glVertex2f( cur.x , cur.y );
					glEnd();
				}
			}
		}
	}
}
