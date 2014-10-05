#ifndef MenuState_h__
#define MenuStage_h__

#include "GameStage.h"

#include "TextureManager.h"
#include "SoundManager.h"
#include "MathCore.h"

class IText;
class GTextButton;


class MenuStage : public GameStage
{
public:
	MenuStage();
	bool onInit();
	void onUpdate(float deltaT);	
	void onRender();
	void onExit();

	void onSystemEvent( sf::Event const& event );
	void onWidgetEvent( int event , int id , GWidget* sender );

	void renderLoading();

private:
	enum State
	{
		MS_NONE ,
		MS_SELECT_MENU ,
		MS_ABOUT ,
		MS_SELECT_LEVEL ,
	};

	void changeState( State state );
	void showStateWidget( State state , bool beShow );

	enum
	{
		UI_START = 100 ,
		UI_EXIT  ,
		UI_ABOUT ,
		UI_BACK  ,
		UI_LEVEL ,
		UI_DEV_TEST   ,
	};

	Texture* texCursor;
	Texture* texBG;
	Texture* texBG2;


	struct LevelInfo
	{
		int      index;
		string   levelFile;
		string   mapFile;
		GTextButton* button;
	};


	std::vector<LevelInfo> mLevels;

	State      mState;
	SrceenFade mScreenFade;
	IText*     mTextAbout;
};

#endif // MenuStage_h__