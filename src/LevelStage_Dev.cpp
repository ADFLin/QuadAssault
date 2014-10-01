#include "LevelStage.h"

#include "GUISystem.h"
#include "GameInterface.h"

#include "Light.h"
#include "Trigger.h"

#include "GlobalVariable.h"
#include "DataPath.h"

#include <fstream>

void LevelStage::UpdateDev(float deltaT)
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
		mEditLight->setColorParam(Vec3(sr,sg,sb),si);
		mEditLight->setPos( convertToWorldPos( getGame()->getMousePos() ) );
	}

	updateRender( deltaT );
}

void LevelStage::onSystemEventDev( sf::Event const& event )
{
	switch( event.type )
	{
	case sf::Event::Closed:
		mNeedExit=true;
		break;
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
					Level::addOjectInternal( mEditTrigger );
					mEditTrigger = NULL;
					mStepEdit=0;
					break;

				}

			}			
			else if(event.mouseButton.button==sf::Mouse::Button::Right)
			{				

				Vec2i tPos = convertToTilePos( getGame()->getMousePos() );

				if ( getTerrain().checkRange( tPos.x , tPos.y ) )
				{
					Tile& tile = getTerrain().getData( tPos.x , tPos.y );
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
				DEVMODE=false;
				GUISystem::getInstance().findTopWidget( UI_MAP_TOOL )->show( false );
				break;
			case sf::Keyboard::Key::F4:
				{
					RenderEngine* renderEngine = getGame()->getRenderEenine();
					if( renderEngine->getAmbientLight().x==0.1f)
						renderEngine->setAmbientLight( Vec3(0.8f, 0.8f, 0.8f) );
					else
						renderEngine->setAmbientLight( Vec3(0.1f, 0.1f, 0.1f) );
				}
				break;
			case sf::Keyboard::Key::F6:
				{
					string path = LEVEL_DIR;
					path += gMapFileName;
					saveMap( path.c_str() );				
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

bool LevelStage::saveMap( char const* path )
{
	std::ofstream of( path );

	if ( !of.is_open() )
		return false;

	TileMap& terrain = getTerrain();

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

	for( LightList::iterator iter = mLights.begin() , itEnd = mLights.end();
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

