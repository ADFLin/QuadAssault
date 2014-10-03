#include "LevelEditStage.h"

#include "GUISystem.h"
#include "GameInterface.h"
#include "TextureManager.h"

#include "Level.h"
#include "Light.h"
#include "Trigger.h"

#include "GlobalVariable.h"
#include "DataPath.h"
#include "RenderUtility.h"

#include <fstream>

bool LevelEditStage::onInit()
{
	if( !BaseClass::onInit() )
		return false;

	mEditTileMeta = 0;
	mEditTileType = TID_FLAT;

	postavljaLight = false;
	mStepEdit=0;
	sr=1.0; sg=10; sb=1.0; si=8.0; srad=128.0;

	{
		GFrame* frame = new GFrame( UI_MAP_TOOL , Vec2i(32,32), Vec2i(320, 240) , NULL );
		//"Tools"
		GUISystem::getInstance().addWidget( frame );

		GImageButton* button;

		button = new GImageButton( UI_CREATE_LIGHT , Vec2i(16,32),Vec2i(32,32) , frame );
		button->texImag = getGame()->getTextureMgr()->getTexture("button_light.tga");

		button = new GImageButton( UI_CREATE_TRIGGER ,Vec2i(64,32),Vec2i(32,32) , frame );
		button->texImag = getGame()->getTextureMgr()->getTexture("button_light.tga");

		button = new GImageButton( UI_EMPTY_MAP  ,Vec2i(16,72),Vec2i(32,32) , frame );
		button->texImag = getGame()->getTextureMgr()->getTexture("button_gen.tga");

		button = new GImageButton( UI_SAVE_MAP ,Vec2i(64,72),Vec2i(32,32) , frame );
		button->texImag = getGame()->getTextureMgr()->getTexture("button_save.tga");
	}


	return true;
}

void LevelEditStage::onExit()
{
	BaseClass::onExit();
}

void LevelEditStage::onUpdate( float deltaT )
{	
	float speed=250;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
		speed=750;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		mCamera->setPos(mCamera->getPos()+Vec2f(-speed*deltaT,0));
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		mCamera->setPos(mCamera->getPos()+Vec2f(speed*deltaT,0));
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		mCamera->setPos(mCamera->getPos()+Vec2f(0, -speed*deltaT));
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		mCamera->setPos(mCamera->getPos()+Vec2f(0, speed*deltaT));

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad7))
		sr+=0.5*deltaT;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad4))
		sr-=0.5*deltaT;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad8))
		sg+=0.5*deltaT;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad5))
		sg-=0.5*deltaT;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad9))
		sb+=0.5*deltaT;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad6))
		sb-=0.5*deltaT;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2))
		si+=50*deltaT;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad1))
		si-=50*deltaT;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I))
		srad+=50*deltaT;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K))
		srad-=50*deltaT;
	if(sr<0.0)
		sr=0.0;
	if(sr>1.0)
		sr=1.0;
	if(sg<0.0)
		sg=0.0;
	if(sg>1.0)
		sg=1.0;
	if(sb<0.0)
		sb=0.0;
	if(sb>1.0)
		sb=1.0;
	if(si<0.0)
		si=0.0;

	getGame()->procSystemEvent();

	if(postavljaLight==true)
	{
		mEditLight->radius=srad;
		mEditLight->setColorParam(Vec3f(sr,sg,sb),si);
		mEditLight->setPos( convertToWorldPos( getGame()->getMousePos() ) );
	}
}



void LevelEditStage::onRender()
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

	Player* player = mLevel->getPlayer();
	glLoadIdentity();

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
	glBlendFunc(GL_ONE, GL_ONE);
	drawSprite( Vec2f( getGame()->getMousePos() ) - Vec2f(16,16) ,Vec2f(32,32), mTexCursor );
	glDisable(GL_BLEND);


	Vec2f posCursor = convertToWorldPos( getGame()->getMousePos() );
	sf::Text t;
	t.setFont( *getGame()->getFont( 0 ) );	
	t.setColor(sf::Color(50,255,50));
	t.setCharacterSize(18);
	char buf[256];
	::sprintf( buf ,"x = %f , y = %f " , posCursor.x , posCursor.y );
	t.setString( buf );
	t.setPosition( 10 , t.getLocalBounds().height/2);

	getGame()->getWindow()->pushGLStates();
	getGame()->getWindow()->draw(t);
	getGame()->getWindow()->popGLStates();
}


void LevelEditStage::onSystemEvent( sf::Event const& event )
{
	switch( event.type )
	{
	case sf::Event::MouseButtonPressed:
		{
			if(event.mouseButton.button==sf::Mouse::Button::Left)
			{
				Vec2f wPos = convertToWorldPos( getGame()->getMousePos() );
				if(postavljaLight==true)
				{
					postavljaLight=false;
				}

				switch( mStepEdit )
				{
				case 1:
					t1= wPos;
					mStepEdit=2;
					break;
				case 2:
					t2 = wPos;
					mStepEdit=3;
					break;
				case 3:
					t3 = wPos;
					mEditTrigger->init(t1,t2);
					mLevel->addOjectInternal( mEditTrigger );
					mEditTrigger = NULL;
					mStepEdit=0;
					break;

				}

			}			
			else if(event.mouseButton.button==sf::Mouse::Button::Right)
			{				
				TileMap& terrain = mLevel->getTerrain();

				Vec2i tPos = convertToTilePos( getGame()->getMousePos() );

				if ( terrain.checkRange( tPos.x , tPos.y ) )
				{
					Tile& tile = terrain.getData( tPos.x , tPos.y );
					tile.type = mEditTileType;
					tile.meta = mEditTileMeta;
				}
			}
		}
		break;
	case sf::Event::MouseButtonReleased:
		{

		}
		break;
	case sf::Event::KeyPressed:
		{		
			switch( event.key.code )
			{
			case sf::Keyboard::Key::F1:
				GUISystem::getInstance().findTopWidget( UI_MAP_TOOL )->destroy();
				stop();
				break;
			case sf::Keyboard::Key::F4:
				{
					RenderEngine* renderEngine = getGame()->getRenderEenine();
					if( renderEngine->getAmbientLight().x==0.1f)
						renderEngine->setAmbientLight( Vec3f(0.8f, 0.8f, 0.8f) );
					else
						renderEngine->setAmbientLight( Vec3f(0.1f, 0.1f, 0.1f) );
				}
				break;
			case sf::Keyboard::Key::F6:
				{
					string path = LEVEL_DIR;
					path += gMapFileName;
					saveLevel( path.c_str() );				
				}
				break;
			case sf::Keyboard::Key::G:
				{
					Vec2f wPos = convertToWorldPos( getGame()->getMousePos() );
					cout << "X: " << wPos.x << endl;
					cout << "Y: " << wPos.y << endl;
				}
				break;

			case sf::Keyboard::Key::Num1:
				mEditTileType = TID_FLAT;
				mEditTileMeta = 0;
				break;
			case sf::Keyboard::Key::Num2:		
				mEditTileType = TID_WALL;
				mEditTileMeta = 0;
				break;
			case sf::Keyboard::Key::Num3:			
				mEditTileType = TID_GAP;
				mEditTileMeta = 0;
				break;
			case sf::Keyboard::Key::Num4:
				mEditTileType = TID_DOOR;
				mEditTileMeta = DOOR_RED;
				break;
			case sf::Keyboard::Key::Num5:	
				mEditTileType = TID_DOOR;
				mEditTileMeta = DOOR_GREEN;
				break;
			case sf::Keyboard::Key::Num6:	
				mEditTileType = TID_DOOR;
				mEditTileMeta = DOOR_BLUE;
				break;
			case sf::Keyboard::Key::Num7:
				mEditTileType = TID_ROCK;
				mEditTileMeta = 2000;
				break;
			}
		}
		break;
	}
}

void LevelEditStage::onWidgetEvent( int event , int id , GWidget* sender )
{
	switch( id )
	{
	case UI_SAVE_MAP:
		{
			string path = LEVEL_DIR;
			path += gMapFileName;
			saveLevel( path.c_str() );
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
			mEditLight = mLevel->createLight( getGame()->getMousePos() , 128 , true);
			mEditLight->setColorParam(Vec3f(1.0, 1.0, 1.0), 8);
		}
		break;
	}
}

bool LevelEditStage::saveLevel( char const* path )
{
	std::ofstream of( path );

	if ( !of.is_open() )
		return false;

	TileMap& terrain = mLevel->getTerrain();

	of << terrain.getSizeX() << " " << terrain.getSizeY() << "\n";

	for(int i=0; i< terrain.getSizeX(); i++)
	for(int j=0; j< terrain.getSizeY(); j++)
	{
		Tile& tile = terrain.getData( i , j );
		if ( tile.type == TID_FLAT && tile.meta == 0 )
			continue;

		of << "block" << " "  
		   << i << " "  << j << " " 
		   << (int)tile.type << " " 
		   << (int)tile.meta << "\n";		
	}


	LightList lights = mLevel->getLights();
	for( LightList::iterator iter = lights.begin() , itEnd = lights.end();
		iter != itEnd ; ++iter )
	{
		Light* light = *iter;
		if( light->isStatic )
		{
		  of << "light " 
			 << light->getPos().x << " " 
			 << light->getPos().y << " " 
			 << light->radius << " " 
			 << light->intensity << " " 
			 << light->color.x << " " 
			 << light->color.y << " " 
			 << light->color.z << "\n";
		}
	}
	of.close ();

	cout << "Map Saved." << endl;

	return true;
}

void LevelEditStage::generateEmptyLevel()
{
	TileMap& terrain = mLevel->getTerrain();
	for(int i=0; i< terrain.getSizeX() ; i++)
	{
		for(int j=0; j< terrain.getSizeY(); j++)
		{		
			Tile& tile = terrain.getData( i , j );
			tile.type = TID_FLAT;
			tile.meta = 0;
			if(i==0 || j==0 || i== terrain.getSizeX()-1 || j== terrain.getSizeY() -1 )
				tile.type = TID_WALL;
		}	
	}

	LightList& lights = mLevel->getLights();
	for(LightList::iterator iter = lights.begin();
		iter != lights.end() ; )
	{
		Light* light = *iter;
		if( light->isStatic )
		{
			++iter;
			delete light;

		}
		else
		{
			++iter;
		}
	}
}
