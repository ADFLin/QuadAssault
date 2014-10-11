#ifndef LevelEditStage_h__
#define LevelEditStage_h__

#include "LevelStage.h"

#include "Singleton.h"

class IText;
class PropFrame;


class EditWorldData : public WorldData
{
public:
	PropFrame* mPropFrame;


};

class EditMode
{
public:
	virtual void cleanup(){}
	virtual void onEnable(){}
	virtual void onDisable(){}
	

	virtual bool onKey( unsigned key , bool isDown ){ return true; }
	virtual bool onMouse( MouseMsg const& msg ){ return true; }
	virtual void onWidgetEvent( int event , int id , GWidget* sender ){}
	virtual void render(){}

	EditWorldData& getWorld(){ return *mWorldData; }
	static EditWorldData* mWorldData;
};

enum EditState
{
	EDIT_CREATE ,
	EDIT_CHIOCE ,
	EDIT_DESTROY ,	
};

class ObjectEditMode  : public EditMode
	                  , public SingletonT< ObjectEditMode >
{
public:

	
	ObjectEditMode();
	virtual void onEnable();
	virtual void onDisable();
	virtual void cleanup();

	virtual bool onMouse( MouseMsg const& msg );
	virtual void onWidgetEvent( int event , int id , GWidget* sender );
	virtual void render();


	class ObjectEditFrame* mFrame;
	LevelObject* mObject;
	char const*  mObjectName;
};



class TileEdit : public IEditable
{
public:
	Tile* getTile(){ return mTile; }

	virtual void enumProp( IPropEditor& editor );
	virtual void updateEdit()
	{

	}

	Tile* mTile;
};


class TileEditMode : public EditMode
	               , public SingletonT< TileEditMode >
{
public:
	TileEditMode();

	TileEdit mEdit;
	int      mEditTileType; //vrsta bloka koji se postavlja
	int      mEditTileMeta;

	class TileEditFrame* mFrame;

	virtual void onEnable();
	virtual void onDisable();
	virtual void cleanup();
	virtual bool onKey( unsigned key , bool isDown );
	virtual bool onMouse( MouseMsg const& msg );
	virtual void onWidgetEvent( int event , int id , GWidget* sender );


};

class LevelEditStage : public LevelStageBase
	                 , public EditWorldData
	                 
{
	typedef LevelStageBase BaseClass;
public:


	LevelEditStage()
	{
		mMode = NULL;
	}
	virtual bool onInit();
	virtual void onExit();
	virtual void onUpdate( float deltaT );
	virtual void onRender();
	virtual void onWidgetEvent( int event , int id , GWidget* sender );
	virtual bool onMouse( MouseMsg const& msg );
	virtual bool onKey( unsigned key , bool isDown );

	bool   saveLevel( char const* path );
	void   generateEmptyLevel();


	void changeMode( EditMode& mode );


protected:

	friend class LevelStage;


	LightObject*     mEditLight;
	float      sr,sg,sb, si, srad; //boja postavljenog svjetla


	bool       postavljaLight;
	int        mStepEdit;
	AreaTrigger* mEditTrigger;

	EditMode*  mMode;

	Vec2f t1, t2, t3; //pocetak, kraj i spawnpoint triggera
};


#endif // LevelEditStage_h__
