#include "Trigger.h"

#include "Player.h"
#include "Level.h"

#include "Message.h"
#include "Mob.h"

#include "RenderUtility.h"

TriggerBase::TriggerBase()
{
	mEnable = true;
	mMode   = FM_DESTROY;
}

TriggerBase::~TriggerBase()
{
	for ( ActionList::iterator iter = mActions.begin() , itEnd = mActions.end();
		iter != itEnd ; ++iter )
	{
		delete *iter;
	}
}

void TriggerBase::fireActions( Level* level )
{
	for ( ActionList::iterator iter = mActions.begin() , itEnd = mActions.end();
		iter != itEnd; ++iter )
	{
		(*iter)->fire( level );
	}
}

void TriggerBase::addAction( Action* act )
{
	mActions.push_back( act );
}

bool TriggerBase::removeAction( Action* act )
{
	ActionList::iterator iter = std::find( mActions.begin() , mActions.end() , act );
	if ( iter == mActions.end() )
		return false;

	mActions.erase( iter );
	delete act;
	return true;
}

AreaTrigger::~AreaTrigger()
{

}

AreaTrigger::AreaTrigger( Vec2f const& min , Vec2f const& max )
	:BaseClass( min )
{
	mSize = max - min;
}

AreaTrigger::AreaTrigger()
{

}

void AreaTrigger::init()
{
	BaseClass::init();
}

void AreaTrigger::onSpawn( unsigned flag )
{
	BaseClass::onSpawn( flag );
}

void AreaTrigger::onDestroy( unsigned flag )
{
	
	BaseClass::onSpawn( flag );
}


void AreaTrigger::tick()
{
	Rect bBox;
	calcBoundBox( bBox );

	if ( mEnable )
	{
		Player* player = getLevel()->getPlayer();
		Rect bBoxOther;
		player->calcBoundBox( bBoxOther );

		if( bBox.intersect(bBoxOther) )
		{
			switch( mMode )
			{
			case FM_ALWAYS:
				fireActions( getLevel() );
				break;
			case FM_DESTROY:
				fireActions( getLevel() );
				destroy();
				mEnable = false;
				break;
			case FM_ONCE:
				fireActions( getLevel() );
				mEnable = false;
				break;
			case FM_ON_TOUCH:
				{
					bool needFire = true;
					for( ObjectList::iterator iter = mTouchObjects.begin() , itEnd = mTouchObjects.end();
						iter != itEnd ; ++iter )
					{
						if ( *iter == player )
						{
							needFire = false;
							break;
						}
					}
					if ( needFire )
					{
						fireActions( getLevel() );
						mTouchObjects.push_back( player );
					}
				}
				break;
			}
		}
	}

	for( ObjectList::iterator iter = mTouchObjects.begin();
		 iter != mTouchObjects.end(); )
	{
		LevelObject* obj = *iter;

		Rect bBoxOther;
		obj->calcBoundBox( bBoxOther );

		if ( !bBox.intersect(bBoxOther) )
		{
			iter = mTouchObjects.erase( iter );
		}
		else
		{
			++iter;
		}
	}
}

void AreaTrigger::renderDev( DevDrawMode mode )
{
	if ( mode == DDM_EDIT )
	{
		Vec2f pos  = getRenderPos();
		Vec2f size = getSize();

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
		glColor4f( 1 , 0.4 , 0.4 , 0.2 );
		drawRect( pos , size );
		glDisable( GL_BLEND );
		glColor3f( 1 , 0.2 , 0.2 );
		drawRectLine( pos , size );
	}
}


void AreaTrigger::setupDefault()
{
	BaseClass::setupDefault();
	setSize( Vec2f( 100 , 100 ) );
}

void SpawnAct::fire( Level* level )
{
	unsigned flag = level->getSpawnDestroyFlag();
	flag = level->setSpwanDestroyFlag( flag | SDF_SETUP_DEFAULT );
	LevelObject* object = level->spawnObjectByName( className.c_str() , spawnPos );
	level->setSpwanDestroyFlag( flag );
	//TODO
	if ( object )
	{



	}
}

void SpawnAct::enumProp( IPropEditor& editor )
{
	BaseClass::enumProp( editor );
	editor.addProp( "ClassName" , className );
	editor.addProp( "SpawnPos" , spawnPos );
	editor.addProp( "SpawnProperty" , spawnProperty );
}

void SpawnAct::setupDefault()
{
	className = "Mob.Minigun";
	spawnPos = Vec2i( 100 , 100 );
}

void MessageAct::fire( Level* level )
{
	Message* msg = new Message();
	msg->init( sender , content , duration , soundName );
	level->addMessage( msg );
}

void MessageAct::enumProp( IPropEditor& editor )
{
	editor.addProp( "Sender" , sender );
	editor.addProp( "Content" , content );
	editor.addProp( "SoundName" , soundName );
	editor.addProp( "Duration" , duration );
}

void MessageAct::setupDefault()
{
	BaseClass::setupDefault();
	sender  = "Base";
	content = "This is context";
	soundName = "blip.wav";
	duration = 4;
}

void GoalAct::fire( Level* level )
{
	level->changeState( Level::eFINISH );
}

void PlaySoundAct::fire( Level* level )
{
	level->playSound( soundName.c_str() );
}

void PlaySoundAct::enumProp( IPropEditor& editor )
{
	BaseClass::enumProp( editor );
	editor.addProp( "SoundName" , soundName );
}

void PlaySoundAct::setupDefault()
{
	BaseClass::setupDefault();
	soundName = "blip.wav";
}
