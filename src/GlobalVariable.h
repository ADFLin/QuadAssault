#ifndef GlobalVariable_h__
#define GlobalVariable_h__

#include <iostream>
using namespace std;

#define BROJ_NIVOA 4

extern string gLevelFileName;
extern string gMapFileName;
extern bool   gLevelEnabled[BROJ_NIVOA];
extern int    gIdxCurLevel;
extern bool   odabir_levela_odmah;

#endif // GlobalVariable_h__
