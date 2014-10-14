#ifndef LevelFile_h__
#define LevelFile_h__

#include "IntegerType.h"

class DataSerializer
{





};

class LevelFile
{
public:
	enum DataBlock
	{
		DB_MAP ,
		DB_OBJECT ,
		NUM_DATA_BLOCK ,
	};

	struct Header
	{
		uint32 mapVersion;
		uint32 objVersion;
	};

};


#endif // LevelFile_h__
