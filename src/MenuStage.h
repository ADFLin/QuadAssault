#ifndef MenuState_h__
#define MenuStage_h__

#include "GameStage.h"

#include "TextureManager.h"
#include "SoundManager.h"
#include "MathCore.h"

class GTextButton;

class MenuStage : public GameStage
{
public:
	bool init();
	void update(float deltaT);	
	void render();
	void exit();

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

	State   mState;

	unsigned char mTransition; //prelazak na tamno (fade to black, ne znak kak se to zove na hrvatskom)
	float   mFadeColor; //boja tranzicije
	float   mFadeSpeed;

	sf::Text mAboutText;
};

#endif // MenuStage_h__