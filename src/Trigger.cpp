#include "Trigger.h"

#include "Player.h"
#include "Level.h"

#include "Message.h"
#include "Mob.h"

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

void AreaTrigger::init( Vec2f const& v1, Vec2f const& v2 )
{
	setPos( v1 );
	mSize   = v2-v1;
	mMode   = FM_ONCE_AND_DESTROY;
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


void AreaTrigger::renderDev()
{
	Vec2f pos  = getRenderPos();
	Vec2f size = getSize();
	glColor3f(1,1,1);
	glBegin(GL_LINE_LOOP);
	glVertex2f( pos.x        , pos.y        );
	glVertex2f( pos.x+size.x , pos.y        );
	glVertex2f( pos.x+size.x , pos.y+size.y );
	glVertex2f( pos.x        , pos.y+size.y );
	glEnd();
}

void SpawnMobAct::fire( Level* level )
{
	LevelObject* object = level->spawnObjectByName( mobName.c_str() , pos );
	if ( object->getType() == OT_MOB )
		object->cast< Mob >()->spawnEffect();
}

void MessageAct::fire( Level* level )
{
	Message* msg = new Message();
	msg->init( sender , content , durstion , soundName );
	level->addMessage( msg );
}

void GoalAct::fire( Level* level )
{
	level->changeState( Level::eFINISH );
}
