#ifndef GameInterface_h__
#define GameInterface_h__

#include "Base.h"

class GWidget;
class GameStage;
class RenderEngine;
class SoundManager;
class TextureManager;
class IFont;

class IGame
{
public:
	virtual ~IGame();

	RenderEngine*   getRenderEenine(){ return mRenderEngine; }
	SoundManager*   getSoundMgr(){ return mSoundMgr; }

	Vec2i const&  getScreenSize(){ return mScreenSize; }
	Vec2i const&  getMousePos(){ return mMousePos; }

	
	virtual void  addStage( GameStage* stage, bool removePrev ) = 0;
	virtual void  stopPlay() = 0;


	virtual void  procWidgetEvent( int event , int id , GWidget* sender ) = 0;
	virtual void  procSystemEvent() = 0;

	virtual IFont* getFont( int idx ) = 0;
protected:
	Vec2i           mScreenSize;
	Vec2i           mMousePos;

	FPtr< RenderEngine >  mRenderEngine;
	FPtr< SoundManager >  mSoundMgr;
	
};

IGame* getGame();

#endif // GameInterface_h__
