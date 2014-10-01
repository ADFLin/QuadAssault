#ifndef Platform_h__
#define Platform_h__

#include "IntegerType.h"

class PlatformWin
{
public:
	static int64 getTickCount(){ return  ::GetTickCount();  }
};


typedef PlatformWin Platform;

#endif // Platform_h__
