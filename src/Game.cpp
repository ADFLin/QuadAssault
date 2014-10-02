#include "Game.h"

#include "GUISystem.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "RenderEngine.h"

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

	mWindow.create(sf::VideoMode(width,height,32), tile, style);	
	//mWindow.SetFramerateLimit(120);	
	mWindow.setMouseCursorVisible(false);

	sf::Font* font = new sf::Font;
	font->loadFromFile( DATA_DIR"DialogueFont.TTF");
	mFonts.push_back( font );

	cout << "Initilize glew..." << endl;
	GLenum greska=glewInit();

	if(greska!=GLEW_OK)
	{
		std::cerr << "ERROR: Impossible to initialize Glew. Your graphics card probably does not support Shader Model 2.0.\n";
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
		mStageStack[i]->exit();
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

	mWindow.close();

	cout << "Game End !!" << endl;
	cout << "*******************" << endl;	

}

void Game::run()
{
	int prevTime = Platform::getTickCount();
	int64 timeFrame = Platform::getTickCount();
	int frameCount = 0;

	sf::Text text;
	text.setFont( *mFonts[0] );		
	text.setColor(sf::Color(50,255,25));
	text.setCharacterSize(18);
		
	text.setPosition( getGame()->getScreenSize().x - 100 , 10 );	

	while( !mNeedEnd )
	{
		int64 curTime = Platform::getTickCount();
		float deltaT = ( curTime - prevTime ) / 1000.0f;
		prevTime = curTime;

		GameStage* stage = mStageStack.back();

		mSoundMgr->update( deltaT );
		mStageStack.back()->update( deltaT );

		mRenderEngine->prevRender();
		mStageStack.back()->render();

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
		text.setString( str.c_str() );
		mWindow.pushGLStates();
		mWindow.draw( text );
		mWindow.popGLStates();
		
		mRenderEngine->postRender();
		mWindow.display();	

		if( mStageStack.back()->isEnd() )
		{
			mStageStack.back()->exit();
			delete mStageStack.back();
			mStageStack.pop_back();			
		}
		if(mStageStack.size()==0)		
			mNeedEnd=true;
	}
}

void Game::addStage( GameStage* stage, bool removePrev )
{	
	using std::cout;
	using std::endl;

	if( removePrev )
	{
		mStageStack.back()->exit();
		delete mStageStack.back();
		mStageStack.pop_back();
		
		cout << "Old stage deleted." << endl;
	}

	mStageStack.push_back(stage);

	cout << "Setup new state..." << endl;
	mStageStack.back()->init();
	cout << "Stage Init !" << endl;
}

sf::RenderWindow* Game::getWindow()
{
	return &mWindow;
}

void Game::procWidgetEvent( int event , int id , GWidget* sender )
{
	mStageStack.back()->onWidgetEvent( event , id , sender );
}

void Game::procSystemEvent()
{
	sf::Event event;
	while( getWindow()->pollEvent(event) )
	{
		bool needSend = true;
		switch( event.type )
		{
		case sf::Event::TextEntered:
			{
				
			}
			break;
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			{
				
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
}
