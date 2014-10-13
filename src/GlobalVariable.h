#ifndef GlobalVariable_h__
#define GlobalVariable_h__

#include "Base.h"
using namespace std;

#define MAX_LEVEL_NUM 4

extern String gLevelFileName;
extern String gMapFileName;
extern bool   gLevelEnabled[MAX_LEVEL_NUM];
extern int    gIdxCurLevel;
extern bool   odabir_levela_odmah;

#endif // GlobalVariable_h__
