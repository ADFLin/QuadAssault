#include "LevelStage.h"

#include "GameInterface.h"
#include "SoundManager.h"
#include "GUISystem.h"
#include "TextureManager.h"
#include "Block.h"

#include "Player.h"
#include "Explosion.h"
#include "Mob.h"
#include "Light.h"
#include "Trigger.h"

#include "Message.h"
#include "MenuStage.h"

#include "WeaponPickup.h"

#include "LaserMob.h"
#include "PlasmaMob.h"
#include "MinigunMob.h"

#include "GlobalVariable.h"
#include "DataPath.h"

#include "EasingFun.h"

bool LevelStage::init()
{
	int screenWidth   = getGame()->getScreenSize().x;
	int screenHeight  = getGame()->getScreenSize().y;


	mEditTileMeta = 0;
	mEditTileType = TID_FLAT;

	mNeedExit = false;

	postavljaLight = false;
	mStepEdit=0;
	sr=1.0; sg=10; sb=1.0; si=8.0; srad=128.0;

	mTexCursor = getGame()->getTextureMgr()->getTexture("kurzor.tga");	
	getGame()->getTextureMgr()->loadTexture("pozadinaSvemir.tga");


	Level::init();


	GUISystem::getInstance().cleanupWidget();

	{
		GFrame* frame = new GFrame( UI_MAP_TOOL , Vec2i(32,32), Vec2i(320, 240) , NULL );
		frame->show( false );
		//"Tools"
		GUISystem::getInstance().addWidget( frame );

		GImageButton* button;

		button = new GImageButton( UI_CREATE_LIGHT , Vec2i(16,32),Vec2i(32,32) , frame );
		button->texImag = getGame()->getTextureMgr()->getTexture("gumb_svjetlo.tga");

		button = new GImageButton( UI_CREATE_TRIGGER ,Vec2i(64,32),Vec2i(32,32) , frame );
		button->texImag = getGame()->getTextureMgr()->getTexture("gumb_svjetlo.tga");

		button = new GImageButton( UI_EMPTY_MAP  ,Vec2i(16,72),Vec2i(32,32) , frame );
		button->texImag = getGame()->getTextureMgr()->getTexture("gumb_gen.tga");

		button = new GImageButton( UI_SAVE_MAP ,Vec2i(64,72),Vec2i(32,32) , frame );
		button->texImag = getGame()->getTextureMgr()->getTexture("gumb_spremi.tga");
	}

	{


		Vec2i panelSize( 150 , 200 );

		Vec2i panelPos;
		panelPos.x = ( screenWidth - panelSize.x ) / 2;
		panelPos.y = ( screenHeight - panelSize.y ) / 2;
		Vec2i buttonSize( 128 , 32 );
		GPanel* panel = new GPanel( UI_MENU_PANEL , panelPos , panelSize , NULL );
		panel->show( false );

		GUISystem::getInstance().addWidget( panel );

		sf::Font* font = getGame()->getFont( 0 );

		Vec2i pos;
		pos.x = ( panelSize.x - buttonSize.x ) / 2;
		pos.y = 10;
		GTextButton* button;
		button = new GTextButton( UI_BACK_GAME , pos , buttonSize , panel );
		button->text.setFont( *font );
		button->text.setCharacterSize( 24 );
		button->text.setString( "Back" );

		pos.y += 40;
		button = new GTextButton( UI_EXIT_GAME , pos , buttonSize , panel );
		button->text.setFont( *font );
		button->text.setCharacterSize( 24 );
		button->text.setString( "Exit Game" );

		pos.y += 40;
		button = new GTextButton( UI_GO_MENU , pos , buttonSize , panel );
		button->text.setFont( *font );
		button->text.setCharacterSize( 24 );
		button->text.setString( "Menu" );

	}

	DEVMODE=false;
	//FIXME
	Block::initMap( this );
	IRenderer::initialize();

	mCamera = new Object();
	mCamera->setPos(Vec2f(0,0));
	mWorldScaleFactor = 1.0f;


	LoadLevel();

	tranzicija=ST_FADEIN;
	tBoja=0.0f;
	brzinaFadeanja=1;

	gameOverTimer=3;
	tickTimer = 0.0f;
	mPause = false;

	return true;
}


void LevelStage::exit()
{
	Level::cleanup();


	for(int i=0; i<mMsgQueue.size(); i++)
		delete mMsgQueue[i];
	mMsgQueue.clear();
	

	mMusic.stop();	

	delete mCamera;

}


void LevelStage::update(float deltaT)
{	
	if( DEVMODE )
	{
		UpdateDev(deltaT);
		return;
	}
	
	tickTimer += deltaT;
	int numFrame = tickTimer / TICK_TIME;

	if ( numFrame )
	{
		tickTimer -= numFrame * TICK_TIME;
		int nFrame = 0;
		for ( ; nFrame < numFrame ; ++nFrame )
		{
			getGame()->procSystemEvent();
			if ( mPause )
				break;

			tick();
		}
		updateRender( nFrame * TICK_TIME );
	}
	else
	{
		getGame()->procSystemEvent();
	}

	if( tranzicija == ST_FADEIN )
	{
		tBoja+=brzinaFadeanja* deltaT;
		if(tBoja>1.0f)
		{
			tranzicija=ST_NONE;
			tBoja=1.0f;
		}
	}

	if( tranzicija==ST_FADEOUT )
	{
		tBoja -= brzinaFadeanja*deltaT;
		if( tBoja < 0.0f )
		{
			tBoja = 0.0f;
			if( !mNeedExit )
			{
				if( Level::getState() == Level::eFinish || mPlayer->isDead() )
					odabir_levela_odmah = true;
				else
					odabir_levela_odmah = false;		
				mNeedExit=true;
			}
		}
	}


}

void LevelStage::tick()
{

	Vec2f wPosMouse = convertToWorldPos( getGame()->getMousePos() );

	float rotateSpeed = Math::toRad( 150 );
	float moveAcc = 1;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		mPlayer->rotate(-rotateSpeed*TICK_TIME);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		mPlayer->rotate( rotateSpeed*TICK_TIME);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		mPlayer->DodajMoment( moveAcc);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		mPlayer->DodajMoment(-moveAcc);
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		mPlayer->shoot( wPosMouse );

	mPlayer->update( wPosMouse );

	Level::tick();
	UpdateajNizove(TICK_TIME);

	//mCamera->changePos( mPlayer->getCenterPos() - ( 0.5f * mWorldScaleFactor ) * Vec2f( getGame()->getScreenSize() ) );

	if ( Level::getState() == Level::eFinish )
	{





	}

	if( Level::getState() == Level::eFinish || mPlayer->isDead() )
		gameOverTimer -= TICK_TIME;

	if(gameOverTimer<=0.0)
		tranzicija = ST_FADEOUT;
}


void LevelStage::UpdateajNizove(float deltaT)
{

	if( !mMsgQueue.empty() )
	{
		Message* message = mMsgQueue.front();

		message->Update( deltaT );
		if( message->unisten == true )
		{
			delete message;
			mMsgQueue.erase(mMsgQueue.begin());
		}
	}
}


void LevelStage::updateRender( float dt )
{
	if ( DEVMODE )
	{
		mTweener.update( dt );
		return;
	}

	Level::updateRender( dt );
	mTweener.update( dt );

	mCamera->setPos( mPlayer->getPos() - ( 0.5f * mWorldScaleFactor ) * Vec2f( getGame()->getScreenSize() ) );

}


void RenderBar( float len , float h , float flac , float alpha , float colorAT[] , float colorAD[] , float colorBT[] , float colorBD[] )
{
	glEnable(GL_BLEND);

	if ( flac > 0 )
	{
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		float temp = len * flac;
		glBegin(GL_QUADS);
		glColor4f(colorAT[0],colorAT[1],colorAT[2],alpha); 
		glVertex2f( 0.0 , 0.0); glVertex2f( temp , 0.0);
		glColor4f(colorAD[0],colorAD[1],colorAD[2],alpha);
		glVertex2f( temp, h );  glVertex2f( 0.0  , h );
		glEnd();	
	}

	glBlendFunc(GL_ONE, GL_ONE);

	glBegin(GL_LINE_LOOP);
	glColor3fv(colorBT); glVertex2f( 0.0 , 0.0 );glVertex2f( len , 0.0 );
	glColor3fv(colorBD); glVertex2f( len , h );  glVertex2f( 0.0 , h );
	glEnd();

	glDisable(GL_BLEND);
	glColor3f(1.0, 1.0, 1.0);
}



void LevelStage::render()
{
	RenderEngine* renderEngine = getGame()->getRenderEenine();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//mWorldScaleFactor = 0.5f;

	mRenderParam.camera      = mCamera;
	mRenderParam.level       = this;
	mRenderParam.scaleFactor = mWorldScaleFactor;
	mRenderParam.mode        = RM_ALL;

	renderEngine->renderScene( mRenderParam );

	if( !mMsgQueue.empty() )
	{
		mMsgQueue[0]->RenderOkvir();
		mMsgQueue[0]->Render();	
	}

	glLoadIdentity();

	glPushMatrix();
	glTranslatef(32, getGame()->getScreenSize().y - 32 , 0);
	{
		float colorAT[] = { 0.0, 0.5, 0.0 };
		float colorAD[] = { 0.0, 0.1, 0.0 };
		float colorBT[] = { 0.0, 0.75, 0.0 };
		float colorBD[] = {	0.0, 0.25, 0.0 };
		float flac = mPlayer->getHP() / mPlayer->getMaxHP();
		RenderBar( 200 , 16 , flac , 0.75 , colorAT , colorAD , colorBT , colorBD );
	}
	glPopMatrix();	

	glPushMatrix();
	glTranslatef( getGame()->getScreenSize().x -232, getGame()->getScreenSize().y - 32, 0);
	{
		float colorAT[] = { 0.2, 0.2, 0.5 };
		float colorAD[] = { 0.05, 0.05, 0.1 };
		float colorBT[] = { 0.2, 0.3, 0.75 };
		float colorBD[] = {	0.1, 0.1, 0.25 };
		float flac = mPlayer->getEnergy() / mPlayer->getMaxEnergy();
		RenderBar( 200 , 16 , flac , 0.75 , colorAT , colorAD , colorBT , colorBD );
	}
	glPopMatrix();

	if ( mPause )
	{
		glEnable(GL_BLEND);
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glColor4f( 0 , 0 , 0 , 0.8 );
		drawRect( Vec2f(0.0, 0.0) , Vec2f( getGame()->getScreenSize() ) );
		glDisable(GL_BLEND);
	}

	GUISystem::getInstance().render();


	glEnable(GL_BLEND);
	glBlendFunc( GL_DST_COLOR, GL_ZERO );
	glColor3f( tBoja, tBoja , tBoja );
	drawRect( Vec2f(0.0, 0.0) , Vec2f( getGame()->getScreenSize() ) );
	glDisable(GL_BLEND);	


	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	drawSprite( Vec2f( getGame()->getMousePos() ) - Vec2f(16,16) ,Vec2f(32,32), mTexCursor );
	glDisable(GL_BLEND);


	sf::Text t;
	t.setFont( *getGame()->getFont( 0 ) );	
	t.setColor(sf::Color(50,255,50));
	t.setCharacterSize(18);
	char buf[256];
	::sprintf( buf ,"x = %f , y = %f " , mPlayer->getPos().x , mPlayer->getPos().y );
	t.setString( buf );
	t.setPosition( 10 , t.getLocalBounds().height/2);

	getGame()->getWindow()->pushGLStates();
	getGame()->getWindow()->draw(t);
	getGame()->getWindow()->popGLStates();

}

void LevelStage::onSystemEvent( sf::Event const& event )
{
	if ( DEVMODE )
	{
		onSystemEventDev( event );
		return;
	}
	switch( event.type )
	{
	case sf::Event::Closed:
		mNeedExit=true;
		break;
	case sf::Event::MouseButtonPressed:
		break;
	case sf::Event::MouseButtonReleased:
		break;
	case sf::Event::KeyPressed:	
		switch( event.key.code )
		{
		case sf::Keyboard::Key::F1:
			DEVMODE=true;
			GUISystem::getInstance().findTopWidget( UI_MAP_TOOL )->show( true );
			break;
		case sf::Keyboard::Key::F2:
			break;
		case sf::Keyboard::Key::Escape:

			GUISystem::getInstance().findTopWidget( UI_MENU_PANEL )->show( true );
			mPause = true;
			break;

			//if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			//	tranzicija=ST_FADEOUT;
		case sf::Keyboard::Key::Q:
			mTweener.tweenValue< Easing::OQuad >( mWorldScaleFactor , mWorldScaleFactor , std::max( mWorldScaleFactor + 0.25f , 0.25f ) , 1.0f );
			break;
		case sf::Keyboard::Key::E:
			mTweener.tweenValue< Easing::OQuad >( mWorldScaleFactor , mWorldScaleFactor , mWorldScaleFactor - 0.25f , 1.0f );
			break;
		}
		break;
	}
}



void LevelStage::onWidgetEvent( int event , int id , GWidget* sender )
{
	switch( id )
	{
	case UI_SAVE_MAP:
		{
			string path = LEVEL_DIR;
			path += gMapFileName;
			saveMap( path.c_str() );
		}
		break;
	case UI_EMPTY_MAP:
		{
			generateEmptyLevel();
		}
		break;
	case UI_CREATE_TRIGGER:
		if( mStepEdit == 0 )
		{		
			mEditTrigger = new AreaTrigger;
			mStepEdit    = 1;
		}
		break;
	case UI_CREATE_LIGHT:
		if(postavljaLight==false)
		{
			postavljaLight = true;
			mEditLight = createLight( getGame()->getMousePos() , 128 , true);
			mEditLight->setColorParam(Vec3(1.0, 1.0, 1.0), 8);
		}
		break;
	case UI_BACK_GAME:
		GUISystem::getInstance().findTopWidget( UI_MENU_PANEL )->show( false );
		mPause = false;
		break;
	case UI_EXIT_GAME:
		tranzicija = ST_FADEOUT;
		break;
	}
}

Message* LevelStage::addMessage(Message* p)
{
	mMsgQueue.push_back(p);
	return mMsgQueue.back();
}
