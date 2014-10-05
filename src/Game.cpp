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

Game::Game()
{
	assert( gGame == NULL );
	gGame = this;

	mFPS = 0;

	mMouseState = 0;
}

bool Game::init(char* configFile)
{
	using std::cout;
	using std::endl;

	 // This falls under the config ***
	int width=800;
	int height=600;
	//width=1024;
	//height=800;
	char const* tile ="QuadAssault";
	int style = sf::Style::Close;		
	//int style = sf::Style::Fullscreen;
	//***************************

	cout << "----QuadAssault----" << endl;
	cout << "*******************" << endl;		

	cout << "Setting Window..." << endl;

	mScreenSize.x = width;
	mScreenSize.y = height;

	mWindow = Platform::createWindow( tile , mScreenSize , 32 , false );
	if ( !mWindow  )
	{
		std::cerr << "ERROR: Can't create window !" << endl;
		return false;
	}
	mWindow->setSystemListener( *this );
	mWindow->showCursor( false );

	mRenderSystem = new RenderSystem;
	mRenderSystem->init( *mWindow );

	IFont* font = IFont::loadFont( DATA_DIR"DialogueFont.TTF" );
	mFonts.push_back( font );

	cout << "Initilize glew..." << endl;
	GLenum result = glewInit();
	if(result != GLEW_OK )
	{
		std::cerr << "ERROR: Impossible to initialize Glew. Your graphics card probably does not support Shader Model 2.0." << endl;
		return false;
	}

	mSoundMgr     = new SoundManager;
	mTextureMgr   = new TextureManager;
	mRenderEngine = new RenderEngine;

	cout << "Initilize Render Engine..." << endl;
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
	
	mTextureMgr->cleanup();	
	delete mTextureMgr;

	mSoundMgr->cleanup();
	delete mSoundMgr;

	mRenderEngine->cleanup();
	delete mRenderEngine;

	delete mRenderSystem;

	mWindow->close();
	mWindow->release();


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

	while( !mNeedEnd )
	{
		int64 curTime = Platform::getTickCount();
		float deltaT = ( curTime - prevTime ) / 1000.0f;
		prevTime = curTime;

		GameStage* stage = mStageStack.back();

		mSoundMgr->update( deltaT );
		mStageStack.back()->onUpdate( deltaT );

		mRenderSystem->prevRender();

		mStageStack.back()->onRender();

		++frameCount;

		if ( frameCount > 50 )
		{
			int64 temp = Platform::getTickCount();
			mFPS = 1000.0f * ( frameCount ) / (  temp - timeFrame );
			timeFrame = temp;
			frameCount = 0;
		}

		FixString< 256 > str;
		str.format( "FPS = %f" , mFPS );
		text->setString( str.c_str() );

		mRenderSystem->drawText( text , 
			Vec2i( getGame()->getScreenSize().x - 100 , 10 ) , 
			TEXT_SIDE_LEFT | TEXT_SIDE_TOP );
		
		mRenderSystem->postRender();

		if( mStageStack.back()->needStop() )
		{
			mStageStack.back()->onExit();
			delete mStageStack.back();
			mStageStack.pop_back();
			cout << "Stage Exit !" << endl;
		}
		if(mStageStack.size()==0)		
			mNeedEnd=true;
	}

	text->release();
}

void Game::addStage( GameStage* stage, bool removePrev )
{	
	using std::cout;
	using std::endl;

	if( removePrev )
	{
		mStageStack.back()->onExit();
		delete mStageStack.back();
		mStageStack.pop_back();
		
		cout << "Old stage deleted." << endl;
	}

	mStageStack.push_back(stage);

	cout << "Setup new state..." << endl;
	mStageStack.back()->onInit();
	cout << "Stage Init !" << endl;
}

void Game::procWidgetEvent( int event , int id , GWidget* sender )
{
	mStageStack.back()->onWidgetEvent( event , id , sender );
}


void Game::procSystemEvent()
{
#if !USE_SFML_WIN
	Platform::procSystemMsg();
#else
	sf::Event event;
	while( mWindow->mImpl.pollEvent( event ) )
	{
		bool needSend = true;
		switch( event.type )
		{
		case sf::Event::Closed:
			getGame()->stopPlay();
			break;
		case sf::Event::TextEntered:
			event.text.unicode;
			break;
		case sf::Event::KeyPressed:
			{
				if ( !GUISystem::getInstance().mManager.procKeyMsg( event.key.code , true ) )
				{
					needSend = false;
				}
			}
			break;
		case sf::Event::KeyReleased:
			{
				if ( !GUISystem::getInstance().mManager.procKeyMsg( event.key.code , false ) )
				{
					needSend = false;
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
		case sf::Event::MouseButtonPressed:
			{
				unsigned msg = 0;

				switch( event.mouseButton.button )
				{
				case sf::Mouse::Button::Left:   msg |= MBS_LEFT; break;
				case sf::Mouse::Button::Right:  msg |= MBS_RIGHT; break;
				case sf::Mouse::Button::Middle: msg |= MBS_MIDDLE; break;
				}

				if ( event.type == sf::Event::MouseButtonPressed )
				{
					mMouseState |= msg;
					msg |= MBS_DOWN;
				}
				else
				{
					mMouseState &= ~msg;
				}

				if ( !GUISystem::getInstance().mManager.procMouseMsg( 
					 MouseMsg( event.mouseButton.x , event.mouseButton.y , msg , mMouseState ) ) )
				{
					needSend = false;
				}
			}
			break;
		case sf::Event::MouseMoved:
			{
				mMousePos.x=event.mouseMove.x;
				mMousePos.y=event.mouseMove.y;

				GUISystem::getInstance().mManager.procMouseMsg( 
					MouseMsg( event.mouseMove.x , event.mouseMove.y , MBS_MOVING , mMouseState ) );
			}
			break;
		}
		if ( needSend )
			mStageStack.back()->onSystemEvent( event );
	}
#endif
}

bool Game::onMouse( MouseMsg const& msg )
{
	if ( msg.onMoving() )
	{
		mMousePos = msg.getPos();
	}

	if ( !GUISystem::getInstance().mManager.procMouseMsg( msg ) )
	{
		if ( !msg.onMoving() )
			return true;
	}
	return true;
}

bool Game::onKey( char key , bool beDown )
{
	if ( !GUISystem::getInstance().mManager.procKeyMsg( key , beDown ) )
		return true;

	return true;
}

bool Game::onChar( char c )
{
	if ( !GUISystem::getInstance().mManager.procCharMsg( c ) )
		return true;

	return true;
}

