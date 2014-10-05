#ifndef PlatformBase_h__
#define PlatformBase_h__


#include "IntegerType.h"
#include "SysMsg.h"

class ISystemListener
{
public:
	virtual bool onMouse( MouseMsg const& mouse ){ return true; }
	virtual bool onKey( char key , bool beDown ){ return true; }
	virtual bool onChar( char key ){ return true; }
};


#endif // PlatformBase_h__
