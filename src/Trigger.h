#ifndef TRIGGER_H
#define TRIGGER_H

#include "Object.h"

class Message;

class Action : public IEditable
{
public:
	virtual void fire( Level* level ) = 0;
	virtual void renderDev(){}
};

enum FireMode
{
	FM_ONCE ,
	FM_ON_TOUCH ,
	FM_ALWAYS ,
	FM_ONCE_AND_DESTROY ,
};

class TriggerBase
{
public:
	TriggerBase();

	void fireActions( Level* level );
	void addAction( Action* act );
	void enable( bool beE ){ mEnable = beE; }
	void setFireMode( FireMode mode ){  mMode = mode;  }
	
protected:
	typedef std::vector< Action* > ActionList;
	ActionList mActions;
	bool       mEnable;
	FireMode   mMode;
};

class AreaTrigger : public LevelObject
	              , public TriggerBase
{
	typedef LevelObject BaseClass;
public:
	AreaTrigger();
	AreaTrigger( Vec2f const& min , Vec2f const& max );
	~AreaTrigger();

	virtual void init();
	virtual ObjectType getType(){ return OT_TRIGGER; }
	virtual void tick();
	virtual void enumProp( IPropEditor& editor );
	virtual void setupDefault();
	virtual void renderDev( DevDrawMode mode );

	

private:
	typedef std::vector< LevelObject* >  ObjectList;
	std::vector< LevelObject* > mTouchObjects;
};

class SpawnAct : public Action
{
public:
	virtual void fire( Level* level );
	virtual void enumProp( IPropEditor& editor );

	Vec2f     pos;
	string    className;
};

class MessageAct : public Action
{
public:
	virtual void fire( Level* level );

	virtual void enumProp( IPropEditor& editor );
	virtual void setupDefault();


	string sender;
	string content;
	string soundName;
	float  durstion;
	
};

class GoalAct : public Action
{
public:
	virtual void fire( Level* level );
};

class PlaySoundAct : public Action
{
public:
	virtual void fire( Level* level );

	string soundName;
};

#endif