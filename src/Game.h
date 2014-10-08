#ifndef Game_h__
#define Game_h__


#include "GameInterface.h"

#include "Base.h"
#include "Platform.h"
#include "Dependence.h"
#include "GameStage.h"
#include "IntegerType.h"

#include <vector>

class IFont;
class RenderSystem;

class Game : public IGame
	       , public ISystemListener
{
public:
	Game();
	bool init(char* configFile);
	void run();
	void exit();

	virtual void  addStage( GameStage* stage, bool removePrev );
	virtual void  stopPlay(){ mNeedEnd = true; }
	virtual void  procWidgetEvent( int event , int id , GWidget* sender );
	virtual void  procSystemEvent();

	IFont*        getFont( int idx ){  return mFonts[idx]; }

	virtual bool onMouse( MouseMsg const& msg );
	virtual bool onKey( unsigned key , bool beDown );
	virtual bool onChar( unsigned code );
	virtual bool onSystemEvent( SystemEvent event );

private:


	float    mFPS;
	unsigned mMouseState;
	bool     mNeedEnd;
	RenderSystem* mRenderSystem;
	std::vector< GameStage* > mStageStack;
	std::vector< IFont* >     mFonts;
	GameWindow*               mWindow;

};




#endif // Game_h__