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

	enum State
	{
		MS_NONE ,
		MS_SELECT_MENU ,
		MS_ABOUT ,
		MS_SELECT_LEVEL ,
	};

	MenuStage( State state = MS_NONE );
	virtual bool onInit();
	virtual void onUpdate(float deltaT);	
	virtual void onRender();
	virtual void onExit();
	virtual bool onMouse( MouseMsg const& msg );
	virtual bool onKey( unsigned key , bool isDown );
	virtual void onWidgetEvent( int event , int id , GWidget* sender );

private:


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
		String   levelFile;
		String   mapFile;
		GTextButton* button;
	};


	std::vector<LevelInfo> mLevels;

	State      mState;
	SrceenFade mScreenFade;
	FObjectPtr< IText > mTextAbout;
};

#endif // MenuStage_h__