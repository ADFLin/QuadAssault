#ifndef Platform_h__
#define Platform_h__

#include "PlatformConfig.h"

#if defined( SYS_PLATFORM_WIN )

#include "PlatformWin.h"
typedef PlatformWin   Platform;

#elif defined( SYS_PLATFORM_LINUX ) || defined( SYS_PLATFORM_MACOS ) || defined( SYS_PLATFORM_FREEBSD )

#include "PlatformSF.h"
typedef PlatformSF    Platform;

#else
#error "Can't compile in this platform !"
#endif

#endif // Platform_h__
