#include "LevelStage.h"

#include "GameInterface.h"
#include "SoundManager.h"

#include "Player.h"
#include "Message.h"
#include "Mob.h"
#include "Trigger.h"

#include "WeaponPickup.h"
#include "KeyPickup.h"

#include "GlobalVariable.h"
#include "DataPath.h"

#include <fstream>
#include <sstream>

#define MX 128
#define MY 128

void LevelStage::LoadLevel()
{
	int mapWidth  = MX;
	int mapHeight = MY;

	string mapPath = LEVEL_DIR;
	mapPath += gMapFileName;

	std::ifstream mapFS( mapPath.c_str() ,ios::in);

	if ( mapFS.is_open() )
	{
		mapFS >> mapWidth;
		mapFS >> mapHeight;
	}

	mLevel->setupTerrain( mapWidth , mapHeight );

	Player* player = mLevel->getPlayer();

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
					Vec3 color;

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

					Light* light = mLevel->createLight( pos , radius , true );
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
			else if(token=="mob")
			{
				Vec2f pos;
				getline(lstring,token,' ');
				pos.x=atof(token.c_str());
				getline(lstring,token,' ');
				pos.y=atof(token.c_str());
				getline(lstring,token,' ');
				Mob* m = mLevel->spawnMobByName( token , pos );
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


void LevelStage::generateEmptyLevel()
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