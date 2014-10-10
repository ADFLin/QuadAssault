#ifndef PlatformBase_h__
#define PlatformBase_h__

#include "IntegerType.h"
#include "SysMsg.h"

#include "TVector2.h"
typedef TVector2< int > Vec2i;

struct  GLConfig
{
	int colorBits;
};

enum SystemEvent
{
	SYS_WINDOW_CLOSE ,
	SYS_QUIT ,
};

class ISystemListener
{
public:
	virtual bool onSystemEvent( SystemEvent event ){ return true; }
	virtual bool onMouse( MouseMsg const& msg ){ return true; }
	virtual bool onKey( unsigned key , bool beDown ){ return true; }
	virtual bool onChar( unsigned code ){ return true; }
};





#endif // PlatformBase_h__
