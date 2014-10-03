#ifndef LevelEditStage_h__
#define LevelEditStage_h__

#include "LevelStage.h"

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

	virtual bool init();
	virtual void exit();

	virtual void update( float deltaT );
	virtual void render();
	virtual void onSystemEvent( sf::Event const& event );
	virtual void onWidgetEvent( int event , int id , GWidget* sender );

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
