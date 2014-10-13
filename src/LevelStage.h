#ifndef LevelState_h__
#define LevelState_h__

#include "GameStage.h"
#include "Level.h"

#include "Object.h"
#include "ObjectFactory.h"
#include "Dependence.h"
#include "RenderEngine.h"
#include "Tween.h"
#include <vector>


class SoundManager;
class GUIManager;
class IText;

class WorldData
{
public:

	Vec2f  convertToScreenPos( Vec2i const& wPos )
	{
		Vec2f offset = wPos - mCamera->getPos();
		return Vec2f( offset.x / mWorldScaleFactor  , offset.y  / mWorldScaleFactor );
	}

	Vec2f  convertToWorldPos( Vec2i const& sPos )
	{
		return mCamera->getPos() + mWorldScaleFactor * Vec2f( sPos );
	}

	Vec2i convertToTilePos( Vec2i const& sPos )
	{
		Vec2f wPos = convertToWorldPos( sPos );
		return Vec2i( Math::floor( wPos.x / BLOCK_SIZE ) , Math::floor( wPos.y  / BLOCK_SIZE ) );
	}

	Level*         getLevel(){ return mLevel; }
	Object*        getCamera(){ return mCamera; }
	ObjectCreator* getObjectCreator(){ return mObjectCreator; }
	ActionCreator* getActionCreator(){ return mActionCreator; }

	void          build();
private:
	void          reigsterObject();
protected:
	Object*        mCamera;
	float          mWorldScaleFactor;
	Level*         mLevel;
	ObjectCreator* mObjectCreator;
	ActionCreator* mActionCreator;
};

class LevelStageBase : public GameStage
{
public:
	virtual bool onInit();
	virtual void onExit();
	virtual void onWidgetEvent( int event , int id , GWidget* sender );
	virtual bool onMouse( MouseMsg const& msg );
	virtual bool onKey( unsigned key , bool isDown );

protected:

	enum
	{
		UI_EDIT_TOOL = 100 ,

		UI_MENU_PANEL ,
		UI_BACK_GAME ,
		UI_EXIT_GAME ,
		UI_GO_MENU   ,

	};

	IText*         mDevMsg;
	Texture*       mTexCursor;
	RenderParam    mRenderParam;
	

	bool        mPause;
};




class LevelStage : public LevelStageBase
	             , public Level::EventListener
				 , public WorldData
{
	typedef LevelStageBase BaseClass;

public:
	virtual bool onInit();
	virtual void onExit();
	virtual void onUpdate(float deltaT);	
	virtual void onRender();

	virtual void onWidgetEvent( int event , int id , GWidget* sender );
	virtual void onLevelEvent( LevelEvent const& event );
	virtual bool onMouse( MouseMsg const& msg );
	virtual bool onKey( unsigned key , bool isDown );

	void tick();
	void updateRender( float dt );

	void loadLevel();
	void generateEmptyLevel();
	

private:

	void changeMenuStage();


	sf::Music  mMusic;
	
	typedef Tween::GroupTweener< float > CTweener;

	CTweener mTweener;
	// when reaches zero, the game goes to the main menu
	float mGameOverTimer; 
	float mTickTimer;
	SrceenFade mScreenFade;
};

#endif // LevelState_h__