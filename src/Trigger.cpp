#include "Trigger.h"

#include "Player.h"
#include "Level.h"

#include "Message.h"
#include "Mob.h"

#include "GlobalVariable.h"
#include "DataPath.h"
#include <fstream>

Trigger::~Trigger()
{
	for ( ActionList::iterator iter = mActions.begin() , itEnd = mActions.end();
		iter != itEnd ; ++iter )
	{
		delete *iter;
	}
}

void Trigger::init( Vec2f const& v1, Vec2f const& v2 )
{
	mPos    = v1;
	mSize   = v2-v1;
	mMode   = FM_ONCE_AND_DESTROY;
	mEnable = true;
}

void Trigger::tick()
{
	Rect k1;
	k1.min=mPos;
	k1.max=mPos+mSize;

	if ( mEnable )
	{
		Player* player = getLevel()->getPlayer();
		Rect k2;
		k2.min= player->getPos();
		k2.max= k2.min + player->getSize();

		if( k1.intersect(k2) )
		{
			switch( mMode )
			{
			case FM_ALWAYS:
				fireActions();
				break;
			case FM_ONCE_AND_DESTROY:
				destroy();
				fireActions();
				mEnable = false;
				break;
			case FM_ONCE:
				fireActions();
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
						fireActions();
				}
				break;
			}
		}
	}

	for( ObjectList::iterator iter = mTouchObjects.begin() , itEnd = mTouchObjects.end();
		iter != itEnd ; )
	{
		LevelObject* obj = *iter;

		Rect k2;
		k2.min= obj ->getPos();
		k2.max= k2.min + obj->getSize();

		if ( k1.intersect(k2) )
		{
			iter = mTouchObjects.erase( iter );
		}
		else
		{
			++iter;
		}
	}

}
void Trigger::fireActions()
{
	for ( ActionList::iterator iter = mActions.begin() , itEnd = mActions.end();
		iter != itEnd; ++iter )
	{
		(*iter)->fire( getLevel() );
	}
}

void Trigger::addAction( Action* act )
{
	mActions.push_back( act );
}

void Trigger::renderDev()
{
	glColor3f(1,1,1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(mPos.x, mPos.y);
	glVertex2f(mPos.x+mSize.x, mPos.y);
	glVertex2f(mPos.x+mSize.x, mPos.y+mSize.y);
	glVertex2f(mPos.x, mPos.y+mSize.y);
	glEnd();
}

void SpawnMobAct::fire( Level* level )
{
	Mob* mob = level->spawnMobByName( mobName , pos );
	mob->spawnEffect();
}

void MessageAct::fire( Level* level )
{
	Message* msg = new Message();
	msg->init( sender , content , durstion , soundName );
	level->addMessage( msg );
}

void GoalAct::fire( Level* level )
{
	level->changeState( Level::eFinish );
	//SPREMANJE INFORMACIJA O OTKLJUCAVANJU				
	if(gIdxCurLevel<BROJ_NIVOA-1)
		gLevelEnabled[gIdxCurLevel+1]=true;

	std::ofstream of( LEVEL_DIR LEVEL_LOCK_FILE );	
	for(int i=0; i<BROJ_NIVOA; i++)
	{
		of << gLevelEnabled[i] << " ";	
	}
	of.close();
}
