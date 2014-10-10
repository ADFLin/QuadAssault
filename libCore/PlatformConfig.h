#ifndef PlatformConfig_h__
#define PlatformConfig_h__

#if defined ( _WIN32 ) || defined ( _WIN64 ) || defined(__WIN32__)
#	define SYS_PLATFORM_WIN
#elif defined ( _XBOX )
#   define SYS_PLATFORM_XBOX
#elif defined(linux) || defined(__linux)
#	define SYS_PLATFORM_LINUX
#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
#	define SYS_PLATFORM_MACOS
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#	define SYS_PLATFORM_FREEBSD
else
#   define SYS_PLATFORM_UNKNOWN
#endif

#endif // PlatformConfig_h__
