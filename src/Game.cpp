#include "Game.h"

#include "GUISystem.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "RenderEngine.h"
#include "RenderSystem.h"

#include "DataPath.h"
#include "Platform.h"

#include "FixString.h"

#include <cassert>
#include <iostream>


static Game* gGame = NULL;
IGame* getGame(){ return gGame; }

IGame::~IGame()
{

}

Game::Game()
{
	assert( gGame == NULL );
	gGame = this;

	mFPS = 0;
	mStageAdd = NULL;

	mMouseState = 0;
}

bool Game::init( char const* pathConfig )
{
	using std::cout;
	using std::endl;

	 // This falls under the config ***
	int width=800;
	int height=600;
	//width=1024;
	//height=800;

	cout << "----QuadAssault----" << endl;
	cout << "*******************" << endl;		

	cout << "Setting Window..." << endl;

	mScreenSize.x = width;
	mScreenSize.y = height;

	char const* tile ="QuadAssault";
	mWindow.reset( Platform::createWindow( tile , mScreenSize , 32 , false ) );
	if ( !mWindow  )
	{
		std::cerr << "ERROR: Can't create window !" << endl;
		return false;
	}
	mWindow->setSystemListener( *this );
	mWindow->showCursor( false );

	cout << "Build Render System..." << endl;
	mRenderSystem.reset( new RenderSystem );
	if ( !mRenderSystem->init( *mWindow ) )
	{
		return false;
	}

	IFont* font = IFont::loadFont( DATA_DIR"DialogueFont.TTF" );
	//IFont* font = NULL;
	mFonts.push_back( font );

	mSoundMgr.reset( new SoundManager );
	mRenderEngine.reset( new RenderEngine );

	cout << "Initialize Render Engine..." << endl;
	mRenderEngine->init( width , height );
		
	mNeedEnd=false;
	srand(time(NULL));
		
	cout << "Setting OpenGL..." << endl;
	glViewport(0,0,(GLsizei)width,(GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,width,height,0,0,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );	
	

	cout << "Game Init!" << endl;
	return true;
}

void Game::exit()
{
	using std::cout;
	using std::endl;

	for(int i= mStageStack.size() - 1 ; i > 0 ; --i )
	{
		mStageStack[i]->onExit();
		delete mStageStack[i];
	}
	mStageStack.clear();

	for( int i = 0 ; i < mFonts.size() ; ++i )
	{
		delete mFonts[i];
	}
	mFonts.clear();	
	
	mSoundMgr->cleanup();
	mRenderEngine->cleanup();
	mRenderSystem->cleanup();

	mWindow.clear();

	cout << "Game End !!" << endl;
	cout << "*******************" << endl;	

}

void Game::run()
{
	using std::cout;
	using std::endl;

	int prevTime = Platform::getTickCount();
	int64 timeFrame = Platform::getTickCount();
	int frameCount = 0;

	IText* text = IText::create( mFonts[0] , 18 , Color(255,255,25) );

	static int const NUM_FPS_SAMPLES = 12;
	float fpsSamples[NUM_FPS_SAMPLES];
	int   NumFramePerSample = 10;
	std::fill_n( fpsSamples , NUM_FPS_SAMPLES , 60.0f );
	int  idxSample = 0;

	while( !mNeedEnd )
	{
		int64 curTime = Platform::getTickCount();
		float deltaT = ( curTime - prevTime ) / 1000.0f;
		prevTime = curTime;

		while ( mStageAdd )
		{
			if( mbRemovePrevStage )
			{
				mStageStack.back()->onExit();
				delete mStageStack.back();
				mStageStack.pop_back();
				cout << "Old stage deleted." << endl;
			}

			GameStage* stage = mStageAdd;
			mStageAdd = NULL;
			mStageStack.push_back( stage );
			cout << "Setup new state..." << endl;
			if ( !stage->onInit() )
			{
				cout << "Stage Can't Init !" << endl;
			}
			cout << "Stage Init !" << endl;
		}

		GameStage* stage = mStageStack.back();

		mSoundMgr->update( deltaT );
		stage->onUpdate( deltaT );

		if ( mRenderSystem->prevRender() )
		{
			stage->onRender();

			++frameCount;
			if ( frameCount > NumFramePerSample )
			{
				int64 temp = Platform::getTickCount();
				fpsSamples[idxSample] = 1000.0f * ( frameCount ) / (  temp - timeFrame );
				timeFrame = temp;
				frameCount = 0;

				++idxSample;
				if ( idxSample == NUM_FPS_SAMPLES )
					idxSample = 0;

				mFPS = 0;
				for (int i = 0; i < NUM_FPS_SAMPLES; i++)
					mFPS += fpsSamples[i];

				mFPS /= NUM_FPS_SAMPLES;
#if 0
				std::cout << "FPS =" << mFPS << std::endl;
#endif
			}

			FixString< 256 > str;
			str.format( "FPS = %f" , mFPS );
			text->setString( str.c_str() );

			mRenderSystem->drawText( text , 
				Vec2i( getGame()->getScreenSize().x - 100 , 10 ) , 
				TEXT_SIDE_LEFT | TEXT_SIDE_TOP );

			mRenderSystem->postRender();
		}

		if( stage->needStop() )
		{
			mStageStack.pop_back();
			stage->onExit();
			delete stage;
			cout << "Stage Exit !" << endl;
		}
		if( mStageStack.empty() )		
			mNeedEnd=true;
	}

	text->release();
}

void Game::addStage( GameStage* stage, bool removePrev )
{	
	using std::cout;
	using std::endl;

	if ( mStageAdd )
	{
		delete stage;
		std::cerr << "Add Stage Error" << std::endl;
		return;
	}
	if ( mStageAdd )
		delete mStageAdd;

	mStageAdd = stage;
	mbRemovePrevStage = removePrev;

}

void Game::procWidgetEvent( int event , int id , GWidget* sender )
{
	mStageStack.back()->onWidgetEvent( event , id , sender );
}

void Game::procSystemEvent()
{
	mWindow->procSystemMessage();
}

bool Game::onMouse( MouseMsg const& msg )
{
	mMousePos = msg.getPos();

	if ( !GUISystem::getInstance().mManager.procMouseMsg( msg ) )
	{
		if ( !msg.onMoving() )
			return true;
	}
	mStageStack.back()->onMouse( msg );
	return true;
}

bool Game::onKey( unsigned key , bool isDown )
{
	if ( !GUISystem::getInstance().mManager.procKeyMsg( key , isDown ) )
		return true;

	mStageStack.back()->onKey( key , isDown );
	return true;
}

bool Game::onChar( unsigned code )
{
	if ( !GUISystem::getInstance().mManager.procCharMsg( code ) )
		return true;

	return true;
}

bool Game::onSystemEvent( SystemEvent event )
{
	switch( event )
	{
	case SYS_WINDOW_CLOSE:
		getGame()->stopPlay();
		return false;
	}

	return true;

}

