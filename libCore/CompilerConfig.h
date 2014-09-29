#ifndef CompilerConfig_h__
#define CompilerConfig_h__

#if defined ( _MSC_VER )
#define CPP_COMPILER_MSVC
#elif defined ( __GNUC__ )
#define CPP_COMPILER_GCC
#else
#error "unknown compiler"
#endif

#endif // CompilerConfig_h__
