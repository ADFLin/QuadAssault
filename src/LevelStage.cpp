#include "LevelStage.h"

#include "GameInterface.h"
#include "SoundManager.h"
#include "GUISystem.h"
#include "RenderSystem.h"
#include "TextureManager.h"

#include "MenuStage.h"
#include "LevelEditStage.h"

#include "GlobalVariable.h"
#include "DataPath.h"
#include "RenderUtility.h"
#include "Block.h"

#include "Player.h"
#include "Explosion.h"
#include "Mob.h"
#include "LightObject.h"
#include "Trigger.h"
#include "Message.h"

#include "KeyPickup.h"
#include "WeaponPickup.h"

#include "LaserMob.h"
#include "PlasmaMob.h"
#include "MinigunMob.h"

#include "Laser.h"
#include "Plasma.h"
#include "Minigun.h"

#include "EasingFun.h"
#include "FixString.h"


#include "GameInterface.h"
#include "SoundManager.h"

#include <fstream>
#include <sstream>


bool LevelStageBase::onInit()
{
	mPause    = false;
	mTexCursor = getGame()->getTextureMgr()->getTexture("cursor.tga");

	mDevMsg = IText::create( getGame()->getFont( 0 ) , 18 , Color(50,255,50) );

	return true;
}

void LevelStageBase::onExit()
{
	mDevMsg->release();
}

void LevelStageBase::onWidgetEvent( int event , int id , GWidget* sender )
{

}


bool LevelStageBase::onMouse( MouseMsg const& msg )
{
	return true;
}

bool LevelStageBase::onKey( unsigned key , bool isDown )
{
	if ( !isDown )
		return false;

	switch( key )
	{
	case Keyboard::eF2:
		break;
	case Keyboard::eESCAPE:
		GUISystem::getInstance().findTopWidget( UI_MENU_PANEL )->show( true );
		mPause = true;
		break;
	}
	return false;
}

bool LevelStage::onInit()
{
	if ( !BaseClass::onInit() )
		return false;

	int screenWidth   = getGame()->getScreenSize().x;
	int screenHeight  = getGame()->getScreenSize().y;

	getGame()->getTextureMgr()->loadTexture("backgroundUniverse.tga");

	GUISystem::getInstance().cleanupWidget();

	{
		Vec2i panelSize( 150 , 200 );

		Vec2i panelPos;
		panelPos.x = ( screenWidth - panelSize.x ) / 2;
		panelPos.y = ( screenHeight - panelSize.y ) / 2;
		Vec2i buttonSize( 128 , 32 );
		GPanel* panel = new GPanel( UI_MENU_PANEL , panelPos , panelSize , NULL );
		panel->show( false );

		GUISystem::getInstance().addWidget( panel );

		IFont* font = getGame()->getFont( 0 );

		Vec2i pos;
		pos.x = ( panelSize.x - buttonSize.x ) / 2;
		pos.y = 10;
		GTextButton* button;
		button = new GTextButton( UI_BACK_GAME , pos , buttonSize , panel );
		button->text->setFont( font );
		button->text->setCharSize( 24 );
		button->text->setString( "Back" );

		pos.y += 40;
		button = new GTextButton( UI_EXIT_GAME , pos , buttonSize , panel );
		button->text->setFont( font );
		button->text->setCharSize( 24 );
		button->text->setString( "Exit Game" );

		pos.y += 40;
		button = new GTextButton( UI_GO_MENU , pos , buttonSize , panel );
		button->text->setFont( font );
		button->text->setCharSize( 24 );
		button->text->setString( "Menu" );

	}


	Block::initialize();
	IRenderer::initialize();


	mObjectCreator = new ObjectCreator;
	reigsterObject();

	mLevel = new Level;
	mLevel->init( *mObjectCreator );
	mLevel->addListerner( *this );

	mCamera = new Object();
	mCamera->setPos(Vec2f(0,0));
	mWorldScaleFactor = 1.0f;

	loadLevel();

	Player* player = mLevel->getPlayer();
	//player->addWeapon(new Plasma());
	//player->addWeapon(new Laser());
	//player->addWeapon(new Laser());
	//player->addWeapon(new Plasma());
	player->addWeapon(new Minigun());
	player->addWeapon(new Minigun());

	for ( int i = 0 ; i < 20 ; ++i )
	{
		mLevel->spawnObjectByName( "Mob.Laser" , Vec2f( 300 + i * 100 , 1000 ) );
		mLevel->spawnObjectByName( "Mob.Laser" , Vec2f( 300 + i * 100 , 1200 ) );
	}


	mScreenFade.setColor( 0 );
	mScreenFade.fadeIn();

	mGameOverTimer =3;
	mTickTimer     = 0.0f;


	return true;
}


void LevelStage::onExit()
{
	mLevel->cleanup();
	mMusic.stop();

	delete mLevel;
	delete mCamera;

	//FIXME
	IRenderer::cleanup();
	Block::cleanup();
	
}


void LevelStage::onUpdate(float deltaT)
{	
	
	mTickTimer += deltaT;
	int numFrame = mTickTimer / TICK_TIME;

	if ( numFrame )
	{
		mTickTimer -= numFrame * TICK_TIME;
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

	mScreenFade.update( deltaT );
}

void LevelStage::changeMenuStage()
{
	if(  mLevel->getState() == Level::eFINISH || mLevel->getPlayer()->isDead() )
		odabir_levela_odmah = true;
	else
		odabir_levela_odmah = false;

	MenuStage* stage = new MenuStage;
	getGame()->addStage( stage , true );
}

void LevelStage::tick()
{

	Vec2f wPosMouse = convertToWorldPos( getGame()->getMousePos() );

	Player* player = mLevel->getPlayer();

	float rotateSpeed = Math::toRad( 150 );
	float moveAcc = 1;
	if(Platform::isKeyPressed(Keyboard::eLEFT) || Platform::isKeyPressed(Keyboard::eA))
		player->rotate(-rotateSpeed*TICK_TIME);
	if(Platform::isKeyPressed(Keyboard::eRIGHT) || Platform::isKeyPressed(Keyboard::eD))
		player->rotate( rotateSpeed*TICK_TIME);
	if(Platform::isKeyPressed(Keyboard::eUP) || Platform::isKeyPressed(Keyboard::eW))
		player->addMoment( moveAcc);
	if(Platform::isKeyPressed(Keyboard::eDOWN) || Platform::isKeyPressed(Keyboard::eS))
		player->addMoment(-moveAcc);
	if(Platform::isButtonPressed( Mouse::eLBUTTON ) )
		player->shoot( wPosMouse );

	player->update( wPosMouse );

	mLevel->tick();

	if( mLevel->getState() == Level::eFINISH || player->isDead() )
		mGameOverTimer -= TICK_TIME;

	if( mGameOverTimer <= 0.0 )
		mScreenFade.fadeOut( std::tr1::bind( &LevelStage::changeMenuStage , this ) );

}


void LevelStage::updateRender( float dt )
{
	mLevel->updateRender( dt );
	mTweener.update( dt );
	mCamera->setPos( mLevel->getPlayer()->getPos() - ( 0.5f * mWorldScaleFactor ) * Vec2f( getGame()->getScreenSize() ) );
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



void LevelStage::onRender()
{
	RenderEngine* renderEngine = getGame()->getRenderEenine();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//mWorldScaleFactor = 0.5f;

	mRenderParam.camera      = mCamera;
	mRenderParam.level       = mLevel;
	mRenderParam.scaleFactor = mWorldScaleFactor;
	mRenderParam.mode        = RM_ALL;

	renderEngine->renderScene( mRenderParam );

	Message* msg = mLevel->getTopMessage();
	if ( msg )
		msg->render();

	Player* player = mLevel->getPlayer();
	glLoadIdentity();

	//Render HP Bar
	glPushMatrix();
	glTranslatef(32, getGame()->getScreenSize().y - 32 , 0);
	{
		float colorAT[] = { 0.0, 0.5, 0.0 };
		float colorAD[] = { 0.0, 0.1, 0.0 };
		float colorBT[] = { 0.0, 0.75, 0.0 };
		float colorBD[] = {	0.0, 0.25, 0.0 };
		float flac = player->getHP() / player->getMaxHP();
		RenderBar( 200 , 16 , flac , 0.75 , colorAT , colorAD , colorBT , colorBD );
	}
	glPopMatrix();	

	//Render Energy Bar
	glPushMatrix();
	glTranslatef( getGame()->getScreenSize().x -232, getGame()->getScreenSize().y - 32, 0);
	{
		float colorAT[] = { 0.2, 0.2, 0.5 };
		float colorAD[] = { 0.05, 0.05, 0.1 };
		float colorBT[] = { 0.2, 0.3, 0.75 };
		float colorBD[] = {	0.1, 0.1, 0.25 };
		float flac = player->getEnergy() / player->getMaxEnergy();
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


	mScreenFade.render();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	drawSprite( Vec2f( getGame()->getMousePos() ) - Vec2f(16,16) ,Vec2f(32,32), mTexCursor );
	glDisable(GL_BLEND);


	FixString< 256 > str;
	str.format( "x = %f , y = %f " , player->getPos().x , player->getPos().y );
	mDevMsg->setString( str );
	getRenderSystem()->drawText( mDevMsg , Vec2i( 10 , 10 ) , TEXT_SIDE_LEFT | TEXT_SIDE_RIGHT );
}

bool LevelStage::onMouse( MouseMsg const& msg )
{
	return true;
}

bool LevelStage::onKey( unsigned key , bool isDown )
{
	if ( !isDown )
		return false;

	switch( key )
	{
	case Keyboard::eF1:
		{
			LevelEditStage* stage = new LevelEditStage;
			stage->mLevel  = mLevel;
			stage->mCamera = mCamera;
			stage->mWorldScaleFactor = mWorldScaleFactor;
			stage->mObjectCreator = mObjectCreator;
			getGame()->addStage( stage , false );
		}
		break;
	case Keyboard::eF2:
		break;
	case Keyboard::eQ:
		mTweener.tweenValue< Easing::OQuad >( mWorldScaleFactor , mWorldScaleFactor , std::max( mWorldScaleFactor + 0.25f , 0.25f ) , 1.0f );
		break;
	case Keyboard::eE:
		mTweener.tweenValue< Easing::OQuad >( mWorldScaleFactor , mWorldScaleFactor , mWorldScaleFactor - 0.25f , 1.0f );
		break;
	}

	return BaseClass::onKey( key , isDown );
}


void LevelStage::onWidgetEvent( int event , int id , GWidget* sender )
{
	switch( id )
	{
	case UI_BACK_GAME:
		GUISystem::getInstance().findTopWidget( UI_MENU_PANEL )->show( false );
		mPause = false;
		break;
	case UI_GO_MENU:
		getGame()->addStage( new MenuStage , true );
		break;
	case UI_EXIT_GAME:
		mScreenFade.fadeOut( std::tr1::bind( &IGame::stopPlay , getGame() ) );
		break;
	}
	BaseClass::onWidgetEvent( event , id , sender );
}


void LevelStage::onLevelEvent( LevelEvent const& event )
{
	switch( event.id )
	{
	case LevelEvent::ePLAYER_DEAD:
		break;
	case LevelEvent::eCHANGE_STATE:
		if ( event.intVal == Level::eFINISH )
		{
			//SPREMANJE INFORMACIJA O OTKLJUCAVANJU				
			if(gIdxCurLevel<MAX_LEVEL_NUM-1)
				gLevelEnabled[gIdxCurLevel+1]=true;

			std::ofstream of( LEVEL_DIR LEVEL_LOCK_FILE );	
			for(int i=0; i<MAX_LEVEL_NUM; i++)
			{
				of << gLevelEnabled[i] << " ";	
			}
			of.close();
		}
		break;
	}
}

void LevelStage::loadLevel()
{
	int mapWidth  = 128;
	int mapHeight = 128;

	string mapPath = LEVEL_DIR;
	mapPath += gMapFileName;

	std::ifstream mapFS( mapPath.c_str() ,ios::in);

	if ( mapFS.is_open() )
	{
		mapFS >> mapWidth;
		mapFS >> mapHeight;
	}

	mLevel->setupTerrain( mapWidth , mapHeight );

	Player* player = mLevel->createPlayer();

	TileMap& terrain = mLevel->getTerrain();

	if ( mapFS.good() )
	{
		string linija_;
		while(getline(mapFS,linija_))
		{
			std::istringstream lstring(linija_,std::ios::in);
			string token;
			while( getline(lstring,token,' ') )
			{
				if(token=="block")
				{
					getline(lstring,token,' ');
					int x = atoi(token.c_str());
					getline(lstring,token,' ');
					int y = atoi(token.c_str());
					getline(lstring,token,' ');
					int type = atoi(token.c_str());
					getline(lstring,token,' ');
					int meta = atoi(token.c_str());

					if ( terrain.checkRange( x , y ) )
					{
						Tile& tile = terrain.getData( x , y ); 
						tile.type = type;
						tile.meta = meta;
					}
				}
				else if(token=="light")
				{
					Vec2f pos;
					Vec3f color;

					getline(lstring,token,' ');
					pos.x = atof(token.c_str());
					getline(lstring,token,' ');
					pos.y = atof(token.c_str());
					getline(lstring,token,' ');
					float radius=atof(token.c_str());
					getline(lstring,token,' ');
					float intensity=atof(token.c_str());
					getline(lstring,token,' ');
					color.x = atof(token.c_str());
					getline(lstring,token,' ');
					color.y = atof(token.c_str());
					getline(lstring,token,' ');
					color.z = atof(token.c_str());

					LightObject* light = mLevel->createLight( pos , radius );
					light->setColorParam( color ,intensity);
					//light->drawShadow = true;
				}
			}
		}
		mapFS.close();
	}


	Vec2f posPlayer = Vec2f(0,0);

	string levelPath = LEVEL_DIR;
	levelPath += gLevelFileName;

	ifstream levelFS( levelPath.c_str() ,ios::in);
	string linija;
	while(getline(levelFS,linija))
	{
		istringstream lstring(linija,ios::in);
		string token;
		while(getline(lstring,token,' '))
		{			
			if(token=="player")
			{
				getline(lstring,token,' ');
				float x=atof(token.c_str());
				getline(lstring,token,' ');
				float y=atof(token.c_str());
				posPlayer=Vec2f(x,y);
				player->setPos( posPlayer );
			}	
			else if(token=="weapon")
			{
				Vec2f pos;
				getline(lstring,token,' ');
				pos.x=atof(token.c_str());
				getline(lstring,token,' ');
				pos.y=atof(token.c_str());
				getline(lstring,token,' ');
				int id=atoi(token.c_str());

				WeaponPickup* item = new WeaponPickup( pos , id );
				item->init();
				mLevel->addItem( item );
			}
			else if(token=="key")
			{
				Vec2f pos;
				getline(lstring,token,' ');
				pos.x=atof(token.c_str());
				getline(lstring,token,' ');
				pos.y=atof(token.c_str());
				getline(lstring,token,' ');
				int id=atoi(token.c_str());

				KeyPickup* item = new KeyPickup( pos , id );
				item->init();
				mLevel->addItem( item );
			}
			else if(token=="preload_sound")
			{
				getline(lstring,token,' ');
				getGame()->getSoundMgr()->loadSound(token.c_str());
			}
			else if(token=="music")
			{
				getline(lstring,token,' ');				
				mMusic.openFromFile( token.c_str() );	
				mMusic.setVolume(25);
				mMusic.setLoop(true);
				mMusic.play();
			}
			else if(token=="mob" || token == "object")
			{
				Vec2f pos;
				getline(lstring,token,' ');
				pos.x=atof(token.c_str());
				getline(lstring,token,' ');
				pos.y=atof(token.c_str());
				getline(lstring,token,' ');
				mLevel->spawnObjectByName( token.c_str() , pos );
			}
			else if(token=="mob_trigger")
			{
				AreaTrigger* trigger = new AreaTrigger;

				Vec2f v1 , v2;
				getline(lstring,token,' ');
				v1.x=atof(token.c_str());
				getline(lstring,token,' ');
				v1.y=atof(token.c_str());
				getline(lstring,token,' ');
				v2.x=atof(token.c_str());
				getline(lstring,token,' ');
				v2.y=atof(token.c_str());
				getline(lstring,token,' ');
				trigger->init( v1 , v2 );

				SpawnMobAct* act = new SpawnMobAct;
				act->pos.x =atof(token.c_str());
				getline(lstring,token,' ');
				act->pos.y =atof(token.c_str());
				getline(lstring,token,' ');	
				act->mobName = token;
				trigger->addAction( act );

				mLevel->addOjectInternal( trigger );
			}
			else if(token=="goal_trigger")
			{
				AreaTrigger* trigger = new AreaTrigger;

				Vec2f v1 , v2;
				getline(lstring,token,' ');
				v1.x=atof(token.c_str());
				getline(lstring,token,' ');
				v1.y=atof(token.c_str());
				getline(lstring,token,' ');
				v2.x=atof(token.c_str());
				getline(lstring,token,' ');
				v2.y=atof(token.c_str());
				getline(lstring,token,' ');
				trigger->init( v1 , v2 );

				trigger->addAction( new GoalAct );

				mLevel->addOjectInternal( trigger );
			}
			else if(token=="msg_trigger")
			{				
				AreaTrigger* trigger = new AreaTrigger;

				Vec2f v1 , v2;
				getline(lstring,token,' ');
				v1.x=atof(token.c_str());
				getline(lstring,token,' ');
				v1.y=atof(token.c_str());
				getline(lstring,token,' ');
				v2.x=atof(token.c_str());
				getline(lstring,token,' ');
				v2.y=atof(token.c_str());
				getline(lstring,token,' ');
				trigger->init( v1 , v2 );

				MessageAct* act = new MessageAct;
				getline(lstring,token,';');
				act->sender = token;
				getline(lstring,token,';');
				act->content = token;
				getline(lstring,token,' ');
				act->durstion = atof(token.c_str());	
				getline(lstring,token,' ');
				act->soundName = token;
				trigger->addAction( act );

				mLevel->addOjectInternal( trigger );
			}
		}
	}
	levelFS.close();

}

void LevelStage::reigsterObject()
{
	mObjectCreator->registerClass< LaserMob >( "Mob.Laser" );
	mObjectCreator->registerClass< MinigunMob >( "Mob.Minigun" );
	mObjectCreator->registerClass< PlasmaMob >( "Mob.Plasma" );
	mObjectCreator->registerClass< KeyPickup >( "Pickup.Key" );
	mObjectCreator->registerClass< WeaponPickup >( "Pickup.Weapon" );
	mObjectCreator->registerClass< LightObject >( "Light" );
}
