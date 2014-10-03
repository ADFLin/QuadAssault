#ifndef GlobalVariable_h__
#define GlobalVariable_h__

#include <iostream>
using namespace std;

#define MAX_LEVEL_NUM 4

extern string gLevelFileName;
extern string gMapFileName;
extern bool   gLevelEnabled[MAX_LEVEL_NUM];
extern int    gIdxCurLevel;
extern bool   odabir_levela_odmah;

#endif // GlobalVariable_h__
