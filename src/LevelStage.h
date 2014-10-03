#ifndef LevelState_h__
#define LevelState_h__

#include "GameStage.h"
#include "Level.h"

#include "Object.h"
#include "Dependence.h"
#include "RenderEngine.h"
#include "Tween.h"
#include <vector>

class SoundManager;
class GUIManager;

class LevelStageBase : public GameStage
{
public:
	virtual bool onInit();
	virtual void onExit();
	virtual void onWidgetEvent( int event , int id , GWidget* sender );
	virtual void onSystemEvent( sf::Event const& event );
protected:

	enum
	{
		UI_MAP_TOOL = 100 ,

		UI_MENU_PANEL ,
		UI_BACK_GAME ,
		UI_EXIT_GAME ,
		UI_GO_MENU   ,

		UI_EDIT_ID  = 400 ,
	};

	Vec2f  convertToWorldPos( Vec2i const& sPos )
	{
		return mCamera->getPos() + mWorldScaleFactor * Vec2f( sPos.x  , sPos.y  );
	}

	Vec2i convertToTilePos( Vec2i const& sPos )
	{
		Vec2f wPos = convertToWorldPos( sPos );
		return Vec2i( Math::floor( wPos.x / BLOCK_SIZE ) , Math::floor( wPos.y  / BLOCK_SIZE ) );
	}
	
	Texture*    mTexCursor;
	RenderParam mRenderParam;
	Object*     mCamera;
	float       mWorldScaleFactor;
	Level*      mLevel;
	bool        mPause;
};




class LevelStage : public LevelStageBase
	             , public Level::EventListener
{
	typedef LevelStageBase BaseClass;

public:
	virtual bool onInit();
	virtual void onExit();
	virtual void onUpdate(float deltaT);	
	virtual void onRender();

	virtual void onWidgetEvent( int event , int id , GWidget* sender );
	virtual void onSystemEvent( sf::Event const& event );
	virtual void onLevelEvent( LevelEvent const& event );

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