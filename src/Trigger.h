#ifndef Trigger_h__
#define Trigger_h__

#include "Object.h"
#include "ColBody.h"

class Message;

class Action : public IEditable
{
public:
	virtual char const* getName() = 0;
	virtual void fire( Level* level ) = 0;
	virtual void renderDev(){}
};
#define DEF_ACTION_NAME( NAME )\
	static char const* StaticName(){ return NAME; }\
	virtual char const* getName(){ return StaticName(); }


enum FireMode
{
	FM_ONCE ,
	FM_DESTROY ,
	FM_ALWAYS ,
	//Area Trigger
	FM_ON_TOUCH ,
	//Time Trigger
	FM_LOOP ,
};

typedef std::vector< Action* > ActionList;

class TriggerBase
{
public:
	TriggerBase();
	~TriggerBase();

	void fireActions( Level* level );
	void addAction( Action* act );
	bool removeAction( Action* act );
	void enable( bool beE ){ mEnable = beE; }
	void setFireMode( FireMode mode ){  mMode = mode;  }

	ActionList& getActions(){ return mActions; }
protected:
	
	ActionList mActions;
	bool       mEnable;
	FireMode   mMode;
};

class AreaTrigger : public LevelObject
	              , public TriggerBase
{
	DECLARE_OBJECT_CLASS( AreaTrigger , LevelObject )
public:
	AreaTrigger();
	AreaTrigger( Vec2f const& min , Vec2f const& max );
	~AreaTrigger();

	virtual void init();
	virtual void onSpawn( unsigned flag );
	virtual void onDestroy( unsigned flag );
	virtual void tick();
	virtual void setupDefault();
	virtual void renderDev( DevDrawMode mode );

private:
	ColBody mBody;
	typedef std::vector< LevelObject* >  ObjectList;
	ObjectList mTouchObjects;

	BEGIN_CLASS_PROP()
	MEMBER_PROP( "AreaSize" , mSize )
	int const fireModeValue[] = { FM_ONCE , FM_ON_TOUCH , FM_ALWAYS , FM_DESTROY };
	char const* fireModeStr[] = { "Once" , "On Touch" , "Always" , "Destroy" };
	MENBER_ENUM_PROP( "FireMode" , mMode , ARRAY_SIZE( fireModeValue ) , fireModeValue , fireModeStr )
	END_CLASS_PROP()
};

class SpawnAct : public Action
{
	typedef Action BaseClass;
public:
	DEF_ACTION_NAME( "Spawn" )
	virtual void fire( Level* level );
	virtual void enumProp( IPropEditor& editor );
	virtual void setupDefault();

	String    className;
	Vec2f     spawnPos;
	String    spawnProperty;
};

class MessageAct : public Action
{
	typedef Action BaseClass;
public:
	DEF_ACTION_NAME( "Message" )
	virtual void fire( Level* level );
	virtual void enumProp( IPropEditor& editor );
	virtual void setupDefault();


	String sender;
	String content;
	String soundName;
	float  duration;
	
};

class GoalAct : public Action
{
	typedef Action BaseClass;
public:
	DEF_ACTION_NAME( "Goal" )
	virtual void fire( Level* level );
};

class PlaySoundAct : public Action
{
	typedef Action BaseClass;
public:
	DEF_ACTION_NAME( "PlaySound" )
	virtual void fire( Level* level );
	virtual void enumProp( IPropEditor& editor );
	virtual void setupDefault();
	String soundName;
};

#endif // Trigger_h__