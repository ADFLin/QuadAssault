#include "DevStage.h"

#include "GameInterface.h"
#include "TextureManager.h"

#include "EditorWidget.h"

#include "GBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Renderer.h"
#include "RenderUtility.h"

#include "MenuStage.h"

class WidgetTest : public TestBase
{

	virtual bool onInit()
	{
		PropFrame* frame = new PropFrame( UI_ANY , Vec2i( 10 , 10 ) , NULL );
		val = 1;
		val2 = 2;

		frame->addProp( "a1" , val );
		frame->addProp( "a2" , val2 );
		GUISystem::getInstance().addWidget( frame );

		GTextButton* button;
		button = new GTextButton( UI_ANY , Vec2i( 100 , 100 ) , Vec2i(128, 64) , NULL );
		button->text->setFont( getGame()->getFont(0) );
		button->text->setString( "Start" );
		GUISystem::getInstance().addWidget( button );

		GChoice* chioce = new GChoice( UI_ANY , Vec2i( 200 , 200 ) , Vec2i( 200 , 30 ) , NULL );
		chioce->appendItem( "Hello" );
		chioce->appendItem( "Good" );
		chioce->appendItem( "Test" );
		GUISystem::getInstance().addWidget( chioce );

		GListCtrl* listCtrl = new GListCtrl( UI_ANY , Vec2i( 500 , 200 ) , Vec2i( 100 , 200 ) , NULL );
		listCtrl->appendItem( "Hello" );
		listCtrl->appendItem( "Good" );
		listCtrl->appendItem( "Test" );
		GUISystem::getInstance().addWidget( listCtrl );

		frame->inputData();
		return true;
	}

	int       val;
	float     val2;
};

class RenderTest : public TestBase
{
public:

	struct Material
	{
		float ka;
		float kd;
		float ks;
		float power;

		Material(){}
		Material( float inKa , float inKd , float inKs , float inPower )
			:ka( inKa ) , kd( inKd ) , ks( inKs ) , power( inPower ){}
	};

	struct Light
	{
		Vec2f pos;
		Vec3f color;
		float ambIntensity;
		float difIntensity;
		float speIntensity;
		float radius;
	};

	class LightingShaderParam
	{
	public:
		void init( Shader* shader )
		{
			mShader = shader;
			locTexMaterial  = mShader->getParamLoc( "texMaterial" );
			locTexBaseColor = mShader->getParamLoc( "texBaseColor" );
			locTexNormal = mShader->getParamLoc( "texNormal" );
			locPos = mShader->getParamLoc( "gLight.pos" );
			locColor = mShader->getParamLoc( "gLight.color" );
			locRadius = mShader->getParamLoc( "gLight.radius"  );
			locAmbIntensity = mShader->getParamLoc( "gLight.ambIntensity" );
			locDifIntensity = mShader->getParamLoc( "gLight.difIntensity" );
			locSpeIntensity = mShader->getParamLoc( "gLight.speIntensity" );
		}

		void setTexture( GBuffer* buffer , GLuint texMat )
		{
			mShader->setTexture1D( locTexMaterial , texMat , 0 );
			mShader->setTexture2D( locTexBaseColor , buffer->getTexture( GBuffer::eBASE_COLOR ) ,1 );
			mShader->setTexture2D( locTexNormal , buffer->getTexture( GBuffer::eNORMAL ) , 2 );
		}

		void setLight( Light& light )
		{
			mShader->setParam( locPos , light.pos );
			mShader->setParam( locColor , light.color );
			mShader->setParam( locRadius , light.radius );
			mShader->setParam( locAmbIntensity , light.ambIntensity );
			mShader->setParam( locDifIntensity , light.difIntensity );
			mShader->setParam( locSpeIntensity , light.speIntensity );
		}

		Shader* mShader;
		int locTexMaterial;
		int locTexBaseColor;
		int locTexNormal;
		int locPos;
		int locColor;
		int locRadius;
		int locAmbIntensity;
		int locDifIntensity;
		int locSpeIntensity;

	};

	static int const MaxRegMaterialNum = 512;
	static float MatId2TexCoord( int id ){ return float( id ) / MaxRegMaterialNum; }

	void registerDefaultMaterial()
	{
		float c = 1.0;
		registerMaterial( Material( c / 2 , c / 2 , c , 5 ) );
		registerMaterial( Material( c , 0 , 0 , 0 ) );
		registerMaterial( Material( 0 , c , 0 , 0 ) );
		registerMaterial( Material( 0 , 0 , c , 5 ) );
	}
	int registerMaterial( Material const& mat )
	{
		assert( mRegMatList.size() < MaxRegMaterialNum );
		mRegMatList.push_back( mat );
		return mRegMatList.size() - 1;
	}

	inline uint32 pow2roundup ( uint32 x )
	{
		if ( x == 0 )
			return 2;

		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return x+1;
	}

	GLuint createMaterialTexture()
	{
		GLuint tex = 0;
		glGenTextures( 1 , &tex );
		glBindTexture( GL_TEXTURE_1D , tex );
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE );

		std::vector< float > buf( 4 * MaxRegMaterialNum , 0.0f );

		float* ptr = &buf[0];
		for ( MaterialVec::iterator iter = mRegMatList.begin() , itEnd = mRegMatList.end();
			iter != itEnd ; ++iter )
		{
			Material& mat = *iter;
			ptr[0] = mat.ka;
			ptr[1] = mat.kd;
			ptr[2] = mat.ks;
			ptr[3] = mat.power;
			ptr += 4;
		}
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F , MaxRegMaterialNum , 0 , GL_RGBA , GL_FLOAT , &buf[0] );
		return tex;
	}
	virtual bool onInit()
	{
		RenderSystem* renderSys = getRenderSystem();

		mGeomShader = renderSys->createShader( "GeomDefaultVS.glsl" , "GeomDefaultFS.glsl" );
		mLightingGlowShader = renderSys->createShader( "LightingGlowVS.glsl" , "LightingGlowFS.glsl" );
		mLightingShader = renderSys->createShader( "LightingPointVS.glsl" , "LightingPointFS.glsl" );
		mLightingParam.init( mLightingShader );

		TextureManager* texMgr = getRenderSystem()->getTextureMgr();

		mTexBlock[0] = texMgr->getTexture( "Block.tga" );
		mTexBlock[1] = texMgr->getTexture( "CircleN.tga" );

		mTexBlock[0] = texMgr->getTexture( "Block.tga" );
		mTexBlock[1] = texMgr->getTexture( "SqureN.tga" );

		mTexObject[ RP_DIFFUSE ] = texMgr->getTexture("mob1Diffuse.tga");
		mTexObject[ RP_NORMAL  ] = texMgr->getTexture("mob1Normal.tga");
		//mTexObject[ RP_NORMAL  ] = texMgr->getEmptyTexture();
		mTexObject[ RP_GLOW    ] = texMgr->getTexture("mob1Glow.tga");

		Vec2i screenSize = getGame()->getScreenSize();
		mGBuffer.reset( new GBuffer );
		if ( !mGBuffer->create( screenSize.x , screenSize.y ) )
			return false;

		mLights[0].pos = Vec2f( 30 , 30 );
		mLights[0].color = Vec3f( 1 , 1 , 1 );
		mLights[0].ambIntensity = 100;
		mLights[0].difIntensity = 25;
		mLights[0].speIntensity = 50;
		mLights[0].radius = 512;

		mLights[1].pos = Vec2f( 130 , 30 );
		mLights[1].color = Vec3f( 0 ,1 , 0 );
		mLights[1].ambIntensity = 25;
		mLights[1].difIntensity = 25;
		mLights[1].speIntensity = 50;
		mLights[1].radius = 256;

		mLights[2].pos = Vec2f( 30 , 130 );
		mLights[2].color = Vec3f( 0 , 0 , 1 );
		mLights[2].ambIntensity = 30;
		mLights[2].difIntensity = 20;
		mLights[2].speIntensity = 150;
		mLights[2].radius = 256;


		registerDefaultMaterial();
		mTexMaterial = createMaterialTexture();

		return true;
	}

	virtual void setDevMsg( FString& str )
	{
		str.format( "%f %f" , mLights[0].pos.x , mLights[0].pos.y );

	}

	bool onMouse(MouseMsg const& msg)
	{
		mLights[0].pos.x = msg.getPos().x;
		mLights[0].pos.y = msg.getPos().y;
		return true; 
	}

	virtual void onUpdate( float dt )
	{
		static float theta = 0.0f;
		theta += 2 * dt;
		Vec2f dir = Vec2f( Math::cos( theta ) , Math::sin( theta ) );
		mLights[2].pos = Vec2f( 330 , 130 ) + 100 * dir;
		mLights[1].pos = Vec2f( 230 , 230 ) + 200 * dir;


	}

	virtual void onRender()
	{
		mGeomShader->bind();

		mGBuffer->bind();

		//glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		mGeomShader->setTexture2D( "texDiffuse" , mTexBlock[0]->id , 0 );
		mGeomShader->setTexture2D( "texNormal" , mTexBlock[1]->id , 1 );
		mGeomShader->setTexture2D( "texGlow" , 0 , 2 );

		for( int j = 0 ; j < 10 ; ++j )
			for( int i = 0 ; i < 20 ; ++i )
			{
				mGeomShader->setParam( "matId" , MatId2TexCoord(( i ) % 4 ) );
				Vec2f pos( 10 + BLOCK_SIZE * i , 10 + BLOCK_SIZE * j );
				Vec2f size( BLOCK_SIZE , BLOCK_SIZE );
				glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(pos.x, pos.y);
				glTexCoord2f(1.0, 0.0); glVertex2f(pos.x+size.x, pos.y);
				glTexCoord2f(1.0, 1.0); glVertex2f(pos.x+size.x, pos.y+size.y);
				glTexCoord2f(0.0, 1.0); glVertex2f(pos.x, pos.y+size.y);
				glEnd();
			}

			mGeomShader->setTexture2D( "texDiffuse" , mTexObject[0]->id , 0 );
			mGeomShader->setTexture2D( "texNormal" , mTexObject[1]->id , 1 );
			mGeomShader->setTexture2D( "texGlow" , mTexObject[2]->id , 2 );


			mGeomShader->setParam( "matId" , MatId2TexCoord(( 0 ) % 4 ) );
			drawSprite( Vec2f( 200 + 64 * 1 , 100 ) , Vec2f( 64 , 64 ) , 0.0f );	
			drawSprite( Vec2f( 300 + 64 * 2 , 210 ) , Vec2f( 64 , 64 ) , 0.0f );	


			mGBuffer->unbind();
			mGeomShader->unbind();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			glEnable( GL_BLEND );
			glBlendFunc( GL_ONE , GL_ONE );

			Vec2i size = getGame()->getScreenSize();

			mLightingGlowShader->bind();
			mLightingGlowShader->setTexture2D( "texBaseColor" , mGBuffer->getTexture( GBuffer::eBASE_COLOR ) , 0 );
			mLightingGlowShader->setTexture2D( "texGlow" , mGBuffer->getTexture( GBuffer::eLIGHTING ) , 1 );

			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0); glVertex2f(0, 0);
			glTexCoord2f(1.0, 1.0); glVertex2f(size.x, 0);
			glTexCoord2f(1.0, 0.0); glVertex2f(size.x, size.y);
			glTexCoord2f(0.0, 0.0); glVertex2f(0, size.y);
			glEnd();
			mLightingGlowShader->unbind();

			mLightingShader->bind();
			mLightingParam.setTexture( mGBuffer , mTexMaterial );

			for( int i = 0 ; i < ARRAY_SIZE( mLights ) ; ++i )
			{
				Light& light = mLights[i];

				mLightingParam.setLight( light );

				Vec2f halfRange =  Vec2f( light.radius , light.radius ); 

				Vec2f minRender = light.pos - halfRange;
				Vec2f maxRender = light.pos + halfRange;

				Vec2f minTex , maxTex;
				minTex.x = minRender.x / size.x;
				maxTex.x = maxRender.x / size.x;
				minTex.y = 1 - minRender.y / size.y;
				maxTex.y = 1 - maxRender.y / size.y;

				glColor3f(1,1,1);

				glBegin(GL_QUADS);
				glTexCoord2f(minTex.x,minTex.y); glVertex2f( minRender.x , minRender.y );
				glTexCoord2f(maxTex.x,minTex.y); glVertex2f( maxRender.x , minRender.y );
				glTexCoord2f(maxTex.x,maxTex.y); glVertex2f( maxRender.x , maxRender.y );
				glTexCoord2f(minTex.x,maxTex.y); glVertex2f( minRender.x , maxRender.y );
				glEnd();	

			}


			mLightingShader->unbind();

			glDisable( GL_BLEND );


#if 0
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable( GL_TEXTURE_2D );

			{
				Vec2i pos( 50 , 50 );
				Vec2i size( 200 , 100 );
				glBindTexture( GL_TEXTURE_2D , mGBuffer->getBuffer( GBuffer::eDIFFUSE ) );
				glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(pos.x, pos.y);
				glTexCoord2f(1.0, 0.0); glVertex2f(pos.x+size.x, pos.y);
				glTexCoord2f(1.0, 1.0); glVertex2f(pos.x+size.x, pos.y+size.y);
				glTexCoord2f(0.0, 1.0); glVertex2f(pos.x, pos.y+size.y);
				glEnd();
			}


			{
				Vec2i pos( 50 + 200 , 50 );
				Vec2i size( 200 , 100 );
				glEnable( GL_TEXTURE_2D );
				glBindTexture( GL_TEXTURE_2D , mGBuffer->getBuffer( GBuffer::eNORMAL ) );
				glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(pos.x, pos.y);
				glTexCoord2f(1.0, 0.0); glVertex2f(pos.x+size.x, pos.y);
				glTexCoord2f(1.0, 1.0); glVertex2f(pos.x+size.x, pos.y+size.y);
				glTexCoord2f(0.0, 1.0); glVertex2f(pos.x, pos.y+size.y);
				glEnd();
			}
#endif



	}


	typedef std::vector< Material > MaterialVec;
	MaterialVec mRegMatList;

	Texture*  mTexBlock[3];
	Texture*  mTexObject[3];
	GLuint    mTexMaterial;
	Light     mLights[ 3 ];

	FPtr< GBuffer >  mGBuffer;
	Shader*   mGeomShader;
	Shader*   mLightingGlowShader;
	Shader*   mLightingShader;
	LightingShaderParam mLightingParam;
};

DevStage::DevStage()
{
	mTest.reset( new RenderTest );
}

bool DevStage::onInit()
{
	mTexCursor = getRenderSystem()->getTextureMgr()->getTexture("cursor.tga");
	mDevMsg.reset( IText::create( getGame()->getFont( 0 ) , 18 , Color(50,255,50) ) );

	GUISystem::getInstance().cleanupWidget();

	if ( !mTest->onInit() )
		return false;

	return true;
}

void DevStage::onExit()
{
	mDevMsg.clear();
}

void DevStage::onUpdate(float deltaT)
{
	getGame()->procSystemEvent();
	mTest->onUpdate( deltaT );
}

void DevStage::onRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	mTest->onRender();

	GUISystem::getInstance().render();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	drawSprite( Vec2f( getGame()->getMousePos() ) - Vec2f(16,16) ,Vec2f(32,32), mTexCursor );
	glDisable(GL_BLEND);

	FString str;
	mTest->setDevMsg( str );
	mDevMsg->setString( str );
	getRenderSystem()->drawText( mDevMsg , Vec2f( 5 ,5 ) , TEXT_SIDE_LEFT | TEXT_SIDE_TOP );
}

bool DevStage::onKey(unsigned key , bool isDown)
{
	if ( !isDown )
		return false;

	switch( key )
	{
	case Keyboard::eESCAPE:
		getGame()->addStage( new MenuStage , true );
		break;
	}
	return false;
}

bool DevStage::onMouse(MouseMsg const& msg)
{
	return mTest->onMouse( msg );
}

DevStage::~DevStage()
{

}
