#include "LevelEditStage.h"

#include "GameInterface.h"
#include "RenderSystem.h"
#include "TextureManager.h"

#include "GUISystem.h"
#include "EditorWidget.h"

#include "Level.h"
#include "LightObject.h"
#include "Player.h"
#include "Trigger.h"

#include "GlobalVariable.h"
#include "DataPath.h"
#include "RenderUtility.h"

#include "FixString.h"
#include <fstream>


EditWorldData* EditMode::mWorldData = NULL;

static Vec2i getModeWidgetPos( Vec2i const& size )
{
	return Vec2i( getGame()->getScreenSize().x - size.x - 10 , 30 );
}

bool LevelEditStage::onInit()
{
	if( !BaseClass::onInit() )
		return false;

	EditMode::mWorldData = this;


	{
		GFrame* frame = new GFrame( UI_EDIT_TOOL , Vec2i( 10 , 40 ), Vec2i( 320 , GFrame::TopSideHeight + 32 + 8 ) , NULL );
		//"Tools"

		mEditToolFrame = frame;
		GUISystem::getInstance().addWidget( frame );


		Vec2i pos = Vec2i( 4 , GFrame::TopSideHeight + 4 );
		{
			Vec2i size = Vec2i( 32 , 32 );
			int offset = size.x + 4;

			GImageButton* button;
			button = new GImageButton( UI_NEW_MAP  , pos , size  , frame );
			button->setHelpText( "New Map" );
			button->texImag = getGame()->getTextureMgr()->getTexture("button_gen.tga");
			pos.x += offset;

			button = new GImageButton( UI_SAVE_MAP , pos , size  , frame );
			button->setHelpText( "Save Map" );
			button->texImag = getGame()->getTextureMgr()->getTexture("button_save.tga");
			pos.x += offset;
			
#if 0
			button = new GImageButton( UI_CREATE_LIGHT , pos , size , frame );
			button->setHelpText( "Create Light" );
			button->texImag = getGame()->getTextureMgr()->getTexture("button_light.tga");
			pos.x += offset;

			button = new GImageButton( UI_CREATE_TRIGGER , pos , size  , frame );
			button->setHelpText( "Create Trigger" );
			button->texImag = getGame()->getTextureMgr()->getTexture("button_light.tga");
			pos.x += offset;
#endif
		}
		{

			Vec2i size = Vec2i( 64 , 32 );
			int offset = size.x + 4;

			GTextButton* button;
			button = new GTextButton( UI_TILE_EDIT , pos , size  , frame );
			button->text->setFont( getGame()->getFont(0) );
			button->text->setCharSize( 20 );
			button->text->setString( "Tile" );
			pos.x += offset;
			button = new GTextButton( UI_OBJECT_EDIT , pos , size  , frame );
			button->text->setFont( getGame()->getFont(0) );
			button->text->setCharSize( 20 );
			button->text->setString( "Object" );
			pos.x += offset;
			button = new GTextButton( UI_TRIGGER_EDIT , pos , size  , frame );
			button->text->setFont( getGame()->getFont(0) );
			button->text->setCharSize( 20 );
			button->text->setString( "Trigger" );
			button->enable( false );
			pos.x += offset;
		}
	}


	mPropFrame = new PropFrame( UI_PROP_FRAME , Vec2i( 10 , 120 ) , NULL );
	GUISystem::getInstance().addWidget( mPropFrame );

	mPropFrame->changeEdit( *mLevel->getPlayer() );

	changeMode( TileEditMode::getInstance() );
	return true;
}

void LevelEditStage::onExit()
{
	ObjectEditMode::getInstance().cleanup();
	TileEditMode::getInstance().cleanup();

	mPropFrame->destroy();
	mEditToolFrame->destroy();
	BaseClass::onExit();
}

void LevelEditStage::onUpdate( float deltaT )
{	
	float speed=250;

	if( Platform::isKeyPressed( Keyboard::eLSHIFT ) )
		speed=750;
	if( Platform::isKeyPressed( Keyboard::eLEFT ) || Platform::isKeyPressed( Keyboard::eA ) )
		mCamera->setPos(mCamera->getPos()+Vec2f(-speed*deltaT,0));
	if( Platform::isKeyPressed( Keyboard::eRIGHT ) || Platform::isKeyPressed( Keyboard::eD) )
		mCamera->setPos(mCamera->getPos()+Vec2f(speed*deltaT,0));
	if( Platform::isKeyPressed( Keyboard::eUP ) || Platform::isKeyPressed( Keyboard::eW ) )
		mCamera->setPos(mCamera->getPos()+Vec2f(0, -speed*deltaT));
	if( Platform::isKeyPressed( Keyboard::eDOWN ) || Platform::isKeyPressed( Keyboard::eS ) )
		mCamera->setPos(mCamera->getPos()+Vec2f(0, speed*deltaT));

	getGame()->procSystemEvent();
	mLevel->updateRender( deltaT );
}



void LevelEditStage::onRender()
{

	RenderEngine* renderEngine = getGame()->getRenderEenine();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	mRenderParam.camera      = mCamera;
	mRenderParam.level       = mLevel;
	mRenderParam.scaleFactor = mWorldScaleFactor;
	mRenderParam.mode        = RM_ALL;

	renderEngine->renderScene( mRenderParam );


	Vec2f camPos = getCamera()->getPos();
	glPushMatrix();
	glTranslatef( -camPos.x , -camPos.y , 0 );
	getLevel()->renderDev( DDM_EDIT );
	mMode->render();
	glPopMatrix();

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
	

	FixString< 256 > str;
	str.format( "x = %f , y = %f " , posCursor.x , posCursor.y );
	mDevMsg->setString( str );
	getRenderSystem()->drawText( mDevMsg , Vec2i( 10 , 10 ) , TEXT_SIDE_LEFT | TEXT_SIDE_RIGHT );

}

bool LevelEditStage::onMouse( MouseMsg const& msg )
{
	if ( !mMode->onMouse( msg ) )
		return false;		

	return false;
}

bool LevelEditStage::onKey( unsigned key , bool isDown )
{
	if ( !mMode->onKey( key , isDown ) )
		return false;

	if ( isDown )
	{
		switch( key )
		{
		case Keyboard::eF1:
			stop();
			break;
		case Keyboard::eF4:
			{
				RenderEngine* renderEngine = getGame()->getRenderEenine();
				if( renderEngine->getAmbientLight().x==0.1f)
					renderEngine->setAmbientLight( Vec3f(0.8f, 0.8f, 0.8f) );
				else
					renderEngine->setAmbientLight( Vec3f(0.1f, 0.1f, 0.1f) );
			}
			break;
		case Keyboard::eF6:
			{
				string path = LEVEL_DIR;
				path += gMapFileName;
				saveLevel( path.c_str() );				
			}
			break;
		case Keyboard::eG:
			{
				Vec2f wPos = convertToWorldPos( getGame()->getMousePos() );
				cout << "X: " << wPos.x << endl;
				cout << "Y: " << wPos.y << endl;
			}
			break;
		}
	}
	return BaseClass::onKey( key , isDown );
}

void LevelEditStage::onWidgetEvent( int event , int id , GWidget* sender )
{
	if ( id >= UI_EDIT_ID )
		mMode->onWidgetEvent( event , id , sender );

	switch( id )
	{
	case UI_SAVE_MAP:
		{
			string path = LEVEL_DIR;
			path += gMapFileName;
			saveLevel( path.c_str() );
		}
		break;
	case UI_NEW_MAP:
		{
			generateEmptyLevel();
		}
		break;
	case UI_CREATE_TRIGGER:
		break;
	case UI_CREATE_LIGHT:
		break;
	case UI_OBJECT_EDIT:
		changeMode( ObjectEditMode::getInstance() );
		break;
	case UI_TILE_EDIT:
		changeMode( TileEditMode::getInstance() );
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
		if ( tile.type == BID_FLAT && tile.meta == 0 )
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
		LightObject* light = *iter;

		of << "light " 
			<< light->getPos().x << " " 
			<< light->getPos().y << " " 
			<< light->radius << " " 
			<< light->intensity << " " 
			<< light->color.x << " " 
			<< light->color.y << " " 
			<< light->color.z << "\n";

	}
	of.close ();

	cout << "Map Saved." << endl;

	return true;
}

void LevelEditStage::generateEmptyLevel()
{
	mLevel->destroyAllObject( true );

	TileMap& terrain = mLevel->getTerrain();
	for(int i=0; i< terrain.getSizeX() ; i++)
	{
		for(int j=0; j< terrain.getSizeY(); j++)
		{		
			Tile& tile = terrain.getData( i , j );
			tile.type = BID_FLAT;
			tile.meta = 0;
			if(i==0 || j==0 || i== terrain.getSizeX()-1 || j== terrain.getSizeY() -1 )
				tile.type = BID_WALL;
		}	
	}
}

void LevelEditStage::changeMode( EditMode& mode )
{
	if ( mMode == &mode )
		return;

	if ( mMode )
		mMode->onDisable();

	mMode = &mode;
	mMode->onEnable();
}


TileEditMode::TileEditMode()
{
	mFrame = NULL;
	mTile = NULL;
	mEditTileMeta = 0;
	mEditTileType = BID_FLAT;
}

bool TileEditMode::onKey( unsigned key , bool isDown )
{
	if ( !isDown )
		return true;

	switch( key )
	{
	case Keyboard::eNUM1:
		mEditTileType = BID_FLAT;
		mEditTileMeta = 0;
		return false;
	case Keyboard::eNUM2:		
		mEditTileType = BID_WALL;
		mEditTileMeta = 0;
		return false;
	case Keyboard::eNUM3:			
		mEditTileType = BID_GAP;
		mEditTileMeta = 0;
		return false;
	case Keyboard::eNUM4:
		mEditTileType = BID_DOOR;
		mEditTileMeta = DOOR_RED;
		return false;
	case Keyboard::eNUM5:	
		mEditTileType = BID_DOOR;
		mEditTileMeta = DOOR_GREEN;
		return false;
	case Keyboard::eNUM6:	
		mEditTileType = BID_DOOR;
		mEditTileMeta = DOOR_BLUE;
		return false;
	case Keyboard::eNUM7:
		mEditTileType = BID_ROCK;
		mEditTileMeta = 2000;
		return false;
	}
	return true;
}

bool TileEditMode::onMouse( MouseMsg const& msg )
{
	Vec2f wPos = getWorld().convertToWorldPos( msg.getPos() );
	Tile* tile = getWorld().getLevel()->getTile( wPos );

	if ( msg.onLeftDown() )
	{
		if ( tile )
		{
			mTile = tile;
			getWorld().mPropFrame->changeEdit( *this );
			return false;
		}
	}
	else if( msg.onRightDown() )
	{				
		if ( tile )
		{
			tile->type = mEditTileType;
			tile->meta = mEditTileMeta;

			mTile = tile;
			getWorld().mPropFrame->changeEdit( *this );
			return false;
		}
	}


	return true;
}

void TileEditMode::onEnable()
{
	if ( mFrame == NULL )
	{
		mFrame = new TileEditFrame( UI_ANY , Vec2i( 10 , 10 ) , NULL );
		mFrame->setPos( getModeWidgetPos( mFrame->getSize() ) );
		GUISystem::getInstance().addWidget( mFrame );
	}
	mFrame->show( true );
}

void TileEditMode::onDisable()
{
	mFrame->show( false );
}

void TileEditMode::onWidgetEvent( int event , int id , GWidget* sender )
{
	switch( id )
	{
	case UI_TILE_SELECT:
		int id = (int)sender->getUserData();
		mEditTileType = id;
		break;
	}
}

void TileEditMode::cleanup()
{
	if ( mFrame )
	{
		mFrame->destroy();
		mFrame = NULL;
	}
	mTile = NULL;

}

void TileEditMode::render()
{
	if (  mTile  )
	{
		glColor3f( 0 , 1 , 0 );
		drawRectLine( mTile->pos , gSimpleBlockSize );
		glColor3f( 1 , 1 , 1 );
	}
}

void TileEditMode::enumProp( IPropEditor& editor )
{
	int tileValue[] = 
	{ 
#define BLOCK_INFO( ID , NAME ) ID ,
#include "BlockType.h"
	};
	char const* tileStr[] = 
	{
#define BLOCK_INFO( ID , NAME ) NAME ,
#include "BlockType.h"
	};
	editor.addEnumProp( "Block Type" , mTile->type , ARRAY_SIZE( tileValue ) , tileValue , tileStr );
	editor.addProp( "Meta" , mTile->meta );
}

ObjectEditMode::ObjectEditMode()
{
	mFrame = NULL;
	mObject = NULL;
	mObjectName = NULL;
}


void ObjectEditMode::onEnable()
{
	if ( mFrame == NULL )
	{
		mFrame = new ObjectEditFrame( UI_ANY , Vec2i( 0 , 0 ) , NULL );
		mFrame->setupObjectList( *getWorld().getObjectCreateor() );
		mFrame->setPos( getModeWidgetPos( mFrame->getSize() ) );
		GUISystem::getInstance().addWidget( mFrame );
	}
	mFrame->show( true );
}

void ObjectEditMode::onDisable()
{
	mFrame->show( false );
}

bool ObjectEditMode::onMouse( MouseMsg const& msg )
{
	Vec2f wPos = getWorld().convertToWorldPos( msg.getPos() );

	if ( msg.onRightDown() )
	{
		if ( mObjectName )
		{
			LevelObject* obj = getWorld().getLevel()->spawnObjectByName( mObjectName , wPos , true );
			changeObject( obj );

			return false;
		}
	}
	else if ( msg.onLeftDown() )
	{
		LevelObject* obj = getWorld().getLevel()->hitObjectTest( wPos );
		if ( obj )
		{
			changeObject( obj );
			return false;
		}
	}
	else if ( msg.onMoving() )
	{
		if ( mObject && msg.isLeftDown() )
		{
			if ( Platform::isKeyPressed( Keyboard::eLSHIFT ) )
			{
				mObject->setPos( wPos );
				getWorld().mPropFrame->inputData();
			}
			else if ( Platform::isKeyPressed( Keyboard::eLCONTROL ) )
			{
				//TODO Dont use dynamic_cast
				Actor* actor = dynamic_cast< Actor* >( mObject );
				if ( actor )
				{
					Vec2f dir = wPos - mObject->getPos();
					float angle = Math::atan2( dir.y , dir.x );
					actor->setRotation( angle );
				}
			}
		}
	}

	return true;

}

void ObjectEditMode::onWidgetEvent( int event , int id , GWidget* sender )
{
	switch( id )
	{
	case UI_OBJECT_SELECT:
		mObjectName = static_cast< char const* >( sender->getUserData() );
		break;
	}
}

void ObjectEditMode::render()
{
	if ( mObject )
	{
		glColor3f( 0 , 1 , 0 );
		drawRectLine( mObject->getRenderPos() , mObject->getSize() );
		glColor3f( 1 , 1 , 1 );
	}
}

void ObjectEditMode::cleanup()
{
	if ( mFrame )
	{
		mFrame->destroy();
		mFrame = NULL;
	}

	mObjectName = NULL;
	mObject = NULL;
}

void ObjectEditMode::changeObject( LevelObject* object )
{
	if ( mObject == object )
		return;

	mObject = object;
	if ( mObject )
	{
		getWorld().mPropFrame->changeEdit( *mObject );
		GWidget* widget = getWorld().mEditToolFrame->findChild( UI_TRIGGER_EDIT );
		if ( widget )
			widget->enable( mObject->getType() == OT_TRIGGER );
	}
	else
	{
		getWorld().mPropFrame->cleanAllPorp();
	}

}
