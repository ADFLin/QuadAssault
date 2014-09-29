#ifndef IntegerType_h__
#define IntegerType_h__

typedef unsigned char byte;

#if defined( _MSC_VER )

typedef __int64  int64;
typedef __int32  int32;
typedef __int16  int16;
typedef __int8   int8;

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8  uint8;

#else

#error "Compiler No Support!"

#endif 


#endif // IntegerType_h__
