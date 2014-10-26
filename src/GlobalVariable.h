#ifndef GlobalVariable_h__
#define GlobalVariable_h__

#include "Base.h"
#define MAX_LEVEL_NUM 4

extern String gLevelFileName;
extern String gMapFileName;
extern bool   gLevelEnabled[MAX_LEVEL_NUM];
extern int    gIdxCurLevel;

#endif // GlobalVariable_h__
