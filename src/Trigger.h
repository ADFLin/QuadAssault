#ifndef TRIGGER_H
#define TRIGGER_H

#include "Object.h"

class Message;

class Action
{
public:
	virtual void fire( Level* level ) = 0;
	virtual void render(){}
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

public:
	~AreaTrigger();
	void init( Vec2f const& v1, Vec2f const& v2 );

	ObjectType getType(){ return OT_TRIGGER; }
	void tick();
	void renderDev();

private:
	typedef std::vector< LevelObject* >  ObjectList;
	std::vector< LevelObject* > mTouchObjects;
};

class SpawnMobAct : public Action
{
public:
	void fire( Level* level );
	Vec2f     pos;
	string    mobName;
};

class MessageAct : public Action
{
public:
	void fire( Level* level );


	string sender;
	string content;
	string soundName;
	float  durstion;
	
};

class GoalAct : public Action
{
public:
	void fire( Level* level );
};

#endif