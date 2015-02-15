#ifndef CppVersion_h__
#define CppVersion_h__

#include "CompilerConfig.h"

#ifdef CPP_COMPILER_MSVC
#	if ( _MSC_VER >= 1700 ) 
#		define CPP_RVALUE_REFENCE_SUPPORT 1
#	    define CPP_VARIADIC_TEMPLATE_SUPPORT 1
#       define CPP_C11_STDLIB_SUPPORT 1
#   endif
#	if ( _MSC_VER >= 1600 ) 
#		define CPP_CX11_KEYWORD_SUPPORT 1
#   endif
#endif

#ifndef CPP_RVALUE_REFENCE_SUPPORT
#	define CPP_RVALUE_REFENCE_SUPPORT 0
#endif //CPP_RVALUE_REFENCE_SUPPORT

#ifndef CPP_VARIADIC_TEMPLATE_SUPPORT
#	define CPP_VARIADIC_TEMPLATE_SUPPORT 0
#endif

#ifndef CPP_CX11_KEYWORD_SUPPORT
#	define CPP_CX11_KEYWORD_SUPPORT 0
#endif //CPP_CX11_KEYWORD_SUPPORT

#ifndef CPP_C11_STDLIB_SUPPORT
#	define CPP_C11_STDLIB_SUPPORT 0
#endif //CPP_C11_STDLIB_SUPPORT

#if !CPP_CX11_KEYWORD_SUPPORT
#	define override
#	define final
#	define nullptr 0
#endif


#endif // CppVersion_h__
