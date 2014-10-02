#include "MenuStage.h"

#include "GameInterface.h"
#include "TextureManager.h"
#include "LevelStage.h"

#include "GUISystem.h"

#include "GlobalVariable.h"
#include "DataPath.h"
#include "RenderUtility.h"

#include <fstream>
#include <sstream>

#include "Dependence.h"

bool MenuStage::init()
{
	mNeedExit=false;	

	texCursor = getGame()->getTextureMgr()->getTexture("kurzor.tga");
	texBG     = getGame()->getTextureMgr()->getTexture("Menu1.tga");
	texBG2    = getGame()->getTextureMgr()->getTexture("MenuLoading1.tga");		


	sf::Font* font = getGame()->getFont( 0 );

	Vec2f poz;
	poz.x = getGame()->getScreenSize().x/2 - 224;
	poz.y = getGame()->getScreenSize().y/2 + getGame()->getScreenSize().y/8;

	GTextButton* button;
	button = new GTextButton( UI_START , Vec2i( poz.x , poz.y ) , Vec2i(128, 64) , NULL );
	button->text.setFont( *font );
	button->text.setString( "Start" );
	button->show( false );
	GUISystem::getInstance().addWidget( button );

	button = new GTextButton( UI_ABOUT , Vec2i( poz.x +128+32 , poz.y ) , Vec2i(128, 64) , NULL );
	button->text.setFont( *font );
	button->text.setString( "About" );
	button->show( false );
	GUISystem::getInstance().addWidget( button );

	button = new GTextButton( UI_EXIT , Vec2i( poz.x +256+64, poz.y ) , Vec2i(128, 64) , NULL );
	button->text.setFont( *font );
	button->text.setString( "Exit" );
	button->show( false );
	GUISystem::getInstance().addWidget( button );

	button = new GTextButton( UI_BACK , Vec2i( 32 , getGame()->getScreenSize().y-96 ) , Vec2i(128, 64) , NULL );
	button->text.setFont( *font );
	button->text.setString( "Back" );
	button->show( false );
	GUISystem::getInstance().addWidget( button );


	mTransition = ST_FADEIN;
	mFadeColor  = 0.0f;
	mFadeSpeed  = 1;

	mState = MS_NONE;

	changeState( MS_SELECT_MENU );

	mAboutText.setFont( *font );		
	mAboutText.setColor(sf::Color(50,255,25));
	mAboutText.setCharacterSize(18);
	char const* text =
		"QuadAssault v1.0\n"
		"----------------\n"
		"You control a tank which in itself can handle various weapons.\n"
		"Movement: W or up arrow, down arrow or S\n"
		"Revolution: A or left arrow, right arrow or D\n"		
		"Shooting: Left mouse button\n"		
		"---------------------\n"
		"Made by Marko Stanic, 4.A-2\n"
		"Supervisor: Dario Jembrek, prof.\n"
		"Trade schools Koprivnica\n"
		"---------------------\n"
		"Contact :\n"
		"Mail/MSN : mstani19@gmail.com\n"
		"Blog : staniks.blogspot.com\n"
		"Youtube : www.youtube.com/geomancer120\n"
		"\n"
		"The game is written in C++, using Microsoft Visual Studio IDE-a.\n"
		"The game uses a library of functions and classes OpenGL and SFML, and shaders is\n"
		"koristen scripting language GLSL. Not used outside engine, but it was written\n"
		"own, special to the game.\n";


	mAboutText.setString(text);		
	mAboutText.setPosition(32, 32);	

	std::ifstream file( LEVEL_DIR "nivoLista.gdf" , std::ios::in );
	string linija;
	while(getline(file,linija))
	{
		std::istringstream stream(linija, std::ios::in);
		string token;
		while(getline(stream,token,' '))
		{
			if(token=="[NIVO]")
			{				
				mLevels.push_back(LevelInfo());
				mLevels.back().index = mLevels.size() - 1;
			}
			else if(token=="datoteka")
			{
				getline(stream,token,' ');
				mLevels.back().datoteka=token;
			}
			else if(token=="datoteka_g")
			{
				getline(stream,token,' ');
				mLevels.back().datoteka_g=token;
			}
		}
	}
	file.close();
	if(odabir_levela_odmah==NULL)
		odabir_levela_odmah=false;
	if(odabir_levela_odmah==true)
		changeState( MS_SELECT_LEVEL );

	ifstream in( LEVEL_DIR LEVEL_LOCK_FILE );	
	for(int i=0; i<BROJ_NIVOA; i++)
	{
		in >> gLevelEnabled[i];	
	}
	in.close();
	for(int i=0; i<mLevels.size(); i++)
	{		
		string text="Level ";
		char brojString[16];
		itoa(i+1,brojString,10);
		text+=brojString;

		button = new GTextButton( UI_LEVEL , Vec2i( getGame()->getScreenSize().x/2-64, 64+i*96 ) , Vec2i(128, 64) , NULL );
		button->text.setFont( *font );
		button->text.setString( text );
		button->setUserData( &mLevels[i] );
		button->show( false );
		GUISystem::getInstance().addWidget( button );

	
		mLevels[i].button = button;
	}

	return true;
}

void MenuStage::exit()
{	
	mLevels.clear();	
}

void MenuStage::update(float deltaT)
{
	getGame()->procSystemEvent();

	if(mTransition==ST_FADEIN)
	{
		mFadeColor+=mFadeSpeed*deltaT;
		if(mFadeColor>1.0f)
		{
			mTransition=ST_NONE;
			mFadeColor=1.0f;
		}
	}
	if(mTransition==ST_FADEOUT)
	{
		mFadeColor-=mFadeSpeed*deltaT;
		if(mFadeColor<0.0f)
		{
			mFadeColor=0.0f;
		}
	}	

	if(mTransition==ST_FADEOUT && mFadeColor==0.0 && !mNeedExit )
	{
		mNeedExit = true;
	}
}

void MenuStage::onSystemEvent( sf::Event const& event )
{
	switch( event.type )
	{
	case sf::Event::Closed:
		mNeedExit=true;
		break;

	case sf::Event::KeyPressed:
		{
			if(event.key.code==sf::Keyboard::Key::Escape)
			{
				if(mState==MS_SELECT_MENU)
					mNeedExit=true;
				else
					mState=MS_SELECT_MENU;
			}
		}
		break;
	case sf::Event::MouseButtonReleased:
		{

		}
		break;
	}
}

void MenuStage::onWidgetEvent( int event , int id , GWidget* sender )
{
	switch( id )
	{
	case UI_LEVEL:
		{
			LevelInfo* info = static_cast< LevelInfo* >( sender->getUserData() );
			renderLoading();
			gLevelFileName   = info->datoteka;
			gMapFileName     = info->datoteka_g;
			gIdxCurLevel     = info->index;
			getGame()->addStage(new LevelStage(), true);
		}

		break;
	case UI_START:
		changeState( MS_SELECT_LEVEL ); 
		break;
	case UI_ABOUT:
		changeState( MS_ABOUT );
		break;
	case UI_BACK:
		changeState( MS_SELECT_MENU );
		break;
	case UI_EXIT:
		mTransition = ST_FADEOUT;
		break;
	}
}

void MenuStage::changeState( State state )
{
	if ( mState == state )
		return;

	showStateWidget( mState , false );
	mState = state;
	showStateWidget( state , true );
}

void MenuStage::showStateWidget( State state , bool beShow )
{
	switch( state )
	{
	case MS_ABOUT:
		GUISystem::getInstance().findTopWidget( UI_BACK )->show( beShow );
		break;
	case MS_SELECT_LEVEL:
		GUISystem::getInstance().findTopWidget( UI_BACK )->show( beShow );
		for(int i=0; i<mLevels.size(); ++i)
			mLevels[i].button->show( beShow );
		break;
	case MS_SELECT_MENU:
		GUISystem::getInstance().findTopWidget( UI_START )->show( beShow );
		GUISystem::getInstance().findTopWidget( UI_ABOUT )->show( beShow );
		GUISystem::getInstance().findTopWidget( UI_EXIT  )->show( beShow );
		break;
	}
}

void MenuStage::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glLoadIdentity();	

	switch( mState )
	{
	case MS_SELECT_LEVEL:
		drawSprite(Vec2f(0.0, 0.0), Vec2f( getGame()->getScreenSize() ), texBG2 );
		break;
	case MS_SELECT_MENU:
		drawSprite(Vec2f(0.0, 0.0), Vec2f( getGame()->getScreenSize() ), texBG );	
		break;
	case MS_ABOUT:
		drawSprite(Vec2f(0.0, 0.0), Vec2f( getGame()->getScreenSize() ), texBG2);

		getGame()->getWindow()->pushGLStates();	
		getGame()->getWindow()->draw(mAboutText);
		getGame()->getWindow()->popGLStates();
		break;
	}

	GUISystem::getInstance().render();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	drawSprite( Vec2f( getGame()->getMousePos() )-Vec2f(16,16),Vec2f(32,32), texCursor );

	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	glColor3f(mFadeColor, mFadeColor, mFadeColor);
	drawRect( Vec2f(0.0, 0.0), Vec2f( getGame()->getScreenSize() ) );

	glDisable(GL_BLEND);

}

void MenuStage::renderLoading()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glLoadIdentity();
	drawSprite(Vec2f(0.0, 0.0), Vec2f(getGame()->getScreenSize().x, getGame()->getScreenSize().y), texBG2 );	

	sf::Text t;
	t.setFont( *getGame()->getFont(0) );	
	t.setColor(sf::Color(50,255,50));
	t.setCharacterSize(35);
	t.setString("Loading Data...");
	t.setPosition(getGame()->getScreenSize().x/2-t.getLocalBounds().width/2,getGame()->getScreenSize().y/2-t.getLocalBounds().height/2);


	getGame()->getWindow()->pushGLStates();
	getGame()->getWindow()->draw(t);
	getGame()->getWindow()->popGLStates();

	getGame()->getWindow()->display();
}