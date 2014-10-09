#ifndef LevelEditStage_h__
#define LevelEditStage_h__

#include "LevelStage.h"

class IText;

class LevelEditStage : public LevelStageBase
{
	typedef LevelStageBase BaseClass;
public:
	enum
	{
		UI_CREATE_LIGHT  = UI_EDIT_ID ,
		UI_CREATE_TRIGGER ,
		UI_EMPTY_MAP ,
		UI_SAVE_MAP  ,

	};

	virtual bool onInit();
	virtual void onExit();

	virtual void onUpdate( float deltaT );
	virtual void onRender();
	virtual void onWidgetEvent( int event , int id , GWidget* sender );
	virtual bool onMouse( MouseMsg const& msg );
	virtual bool onKey( unsigned key , bool isDown );

	bool   saveLevel( char const* path );
	void   generateEmptyLevel();



protected:

	friend class LevelStage;

	Light*   mEditLight;
	float    sr,sg,sb, si, srad; //boja postavljenog svjetla

	int      mEditTileType; //vrsta bloka koji se postavlja
	int      mEditTileMeta;
	bool     postavljaLight;
	int      mStepEdit;
	AreaTrigger* mEditTrigger;

	Vec2f t1, t2, t3; //pocetak, kraj i spawnpoint triggera
};


#endif // LevelEditStage_h__
