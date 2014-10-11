#include "Trigger.h"

#include "Player.h"
#include "Level.h"

#include "Message.h"
#include "Mob.h"

#include "RenderUtility.h"

TriggerBase::TriggerBase()
{
	mEnable = true;
	mMode   = FM_ONCE_AND_DESTROY;
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

AreaTrigger::~AreaTrigger()
{
	for ( ActionList::iterator iter = mActions.begin() , itEnd = mActions.end();
		iter != itEnd ; ++iter )
	{
		delete *iter;
	}
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
			case FM_ONCE_AND_DESTROY:
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
						fireActions( getLevel() );
				}
				break;
			}
		}
	}

	for( ObjectList::iterator iter = mTouchObjects.begin() , itEnd = mTouchObjects.end();
		iter != itEnd ; )
	{
		LevelObject* obj = *iter;

		Rect bBoxOther;
		obj->calcBoundBox( bBoxOther );

		if ( bBox.intersect(bBoxOther) )
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

void AreaTrigger::enumProp( IPropEditor& editor )
{
	int fireModeValue[] = { FM_ONCE , FM_ON_TOUCH , FM_ALWAYS , FM_ONCE_AND_DESTROY };
	char const* fireModeStr[] = { "Once" , "On Touch" , "Always" , "Once And Destroy" };

	editor.addProp( "AreaSize" , mSize );
	editor.addEnumProp( "FireMode" , mMode , ARRAY_SIZE( fireModeValue ) , fireModeValue , fireModeStr );
}

void AreaTrigger::setupDefault()
{
	setSize( Vec2f( 100 , 100 ) );
}

void SpawnAct::fire( Level* level )
{
	LevelObject* object = level->spawnObjectByName( className.c_str() , pos );
	if ( object->getType() == OT_MOB )
		object->cast< Mob >()->spawnEffect();
}

void SpawnAct::enumProp( IPropEditor& editor )
{
	editor.addProp( "ClassName" , className );
	editor.addProp( "SpawnPos" , pos );
}

void MessageAct::fire( Level* level )
{
	Message* msg = new Message();
	msg->init( sender , content , durstion , soundName );
	level->addMessage( msg );
}

void MessageAct::enumProp( IPropEditor& editor )
{
	
}

void MessageAct::setupDefault()
{
	
}

void GoalAct::fire( Level* level )
{
	level->changeState( Level::eFINISH );
}

void PlaySoundAct::fire( Level* level )
{
	level->playSound( soundName.c_str() );
}
