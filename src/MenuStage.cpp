#include "MenuStage.h"

#include "GameInterface.h"
#include "TextureManager.h"
#include "GUISystem.h"
#include "RenderSystem.h"

#include "GlobalVariable.h"
#include "DataPath.h"
#include "RenderUtility.h"

#include "LevelStage.h"
#include "DevStage.h"

#include "Dependence.h"
#include "FixString.h"

#include <fstream>
#include <sstream>

MenuStage::MenuStage()
{
	mState = MS_NONE;
}

bool MenuStage::onInit()
{
	texCursor = getGame()->getTextureMgr()->getTexture("cursor.tga");
	texBG     = getGame()->getTextureMgr()->getTexture("Menu1.tga");
	texBG2    = getGame()->getTextureMgr()->getTexture("MenuLoading1.tga");		

	IFont* font = getGame()->getFont( 0 );

	GUISystem::getInstance().cleanupWidget();

	Vec2f poz;
	poz.x = getGame()->getScreenSize().x/2 - 224;
	poz.y = getGame()->getScreenSize().y/2 + getGame()->getScreenSize().y/8;

	GTextButton* button;
	button = new GTextButton( UI_START , Vec2i( poz.x , poz.y ) , Vec2i(128, 64) , NULL );
	button->text->setFont( font );
	button->text->setString( "Start" );
	button->show( false );
	GUISystem::getInstance().addWidget( button );

	button = new GTextButton( UI_ABOUT , Vec2i( poz.x +128+32 , poz.y ) , Vec2i(128, 64) , NULL );
	button->text->setFont( font );
	button->text->setString( "About" );
	button->show( false );
	GUISystem::getInstance().addWidget( button );

	button = new GTextButton( UI_DEV_TEST , Vec2i( poz.x +128+32 , poz.y + 80 ) , Vec2i(128, 64) , NULL );
	button->text->setFont( font );
	button->text->setString( "Dev Test" );
	button->show( false );
	GUISystem::getInstance().addWidget( button );

	button = new GTextButton( UI_EXIT , Vec2i( poz.x +256+64, poz.y ) , Vec2i(128, 64) , NULL );
	button->text->setFont( font );
	button->text->setString( "Exit" );
	button->show( false );
	GUISystem::getInstance().addWidget( button );

	button = new GTextButton( UI_BACK , Vec2i( 32 , getGame()->getScreenSize().y-96 ) , Vec2i(128, 64) , NULL );
	button->text->setFont( font );
	button->text->setString( "Back" );
	button->show( false );
	GUISystem::getInstance().addWidget( button );

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
	mTextAbout = IText::create( font , 22 , Color(50,255,25) );
	mTextAbout->setString( text );
	

	std::ifstream file( LEVEL_DIR "LevelList.gdf" , std::ios::in );
	std::string linija;
	while(getline(file,linija))
	{
		std::istringstream stream(linija, std::ios::in);
		std::string token;
		while(getline(stream,token,' '))
		{
			if(token=="[LEVEL]")
			{				
				mLevels.push_back(LevelInfo());
				mLevels.back().index = mLevels.size() - 1;
			}
			else if(token=="level_file")
			{
				getline(stream,token,' ');
				mLevels.back().levelFile = token;
			}
			else if(token=="map_file")
			{
				getline(stream,token,' ');
				mLevels.back().mapFile = token;
			}
		}
	}
	file.close();


	ifstream in( LEVEL_DIR LEVEL_LOCK_FILE );	
	for(int i=0; i<MAX_LEVEL_NUM; i++)
	{
		in >> gLevelEnabled[i];	
	}
	in.close();
	for(int i=0; i<mLevels.size(); i++)
	{		

		FixString< 256 > str;
		str.format( "Level %d" , i + 1 );

		button = new GTextButton( UI_LEVEL , Vec2i( getGame()->getScreenSize().x/2-64, 64+i*96 ) , Vec2i(128, 64) , NULL );
		button->text->setFont( font );
		button->text->setString( str );
		button->setUserData( &mLevels[i] );
		button->show( false );
		GUISystem::getInstance().addWidget( button );

	
		mLevels[i].button = button;
	}


	mState = MS_NONE;
	changeState( MS_SELECT_MENU );


	if(odabir_levela_odmah==NULL)
		odabir_levela_odmah=false;
	if(odabir_levela_odmah==true)
		changeState( MS_SELECT_LEVEL );

	mScreenFade.setColor( 1 );
	mScreenFade.fadeIn();

	return true;
}

void MenuStage::onExit()
{	
	mTextAbout->release();
	mLevels.clear();	
}

void MenuStage::onUpdate(float deltaT)
{
	getGame()->procSystemEvent();
	mScreenFade.update( deltaT );
}


void MenuStage::onWidgetEvent( int event , int id , GWidget* sender )
{
	switch( id )
	{
	case UI_LEVEL:
		{
			LevelInfo* info = static_cast< LevelInfo* >( sender->getUserData() );
			gLevelFileName   = info->levelFile;
			gMapFileName     = info->mapFile;
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
		mScreenFade.fadeOut( std::tr1::bind( &GameStage::stop , this ) );
		break;
	case UI_DEV_TEST:
		{
			getGame()->addStage( new DevStage , true );
		}
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
		GUISystem::getInstance().findTopWidget( UI_DEV_TEST )->show( beShow );
		break;
	}
}

void MenuStage::onRender()
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
		getRenderSystem()->drawText( mTextAbout , Vec2i( 32 , 32 ) , TEXT_SIDE_LEFT | TEXT_SIDE_TOP );
		break;
	}

	GUISystem::getInstance().render();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	drawSprite( Vec2f( getGame()->getMousePos() )-Vec2f(16,16),Vec2f(32,32), texCursor );
	glDisable(GL_BLEND);

	mScreenFade.render();
}

bool MenuStage::onKey( unsigned key , bool isDown )
{
	if ( !isDown )
		return true;
	switch( key )
	{
	case Keyboard::eESCAPE:
		if( mState == MS_SELECT_MENU )
			stop();
		else
			changeState( MS_SELECT_MENU );
		break;
	}
	return false;
}

bool MenuStage::onMouse( MouseMsg const& msg )
{
	return true;
}
