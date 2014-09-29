#ifndef GameInterface_h__
#define GameInterface_h__

#include "Base.h"

class GWidget;
class GameStage;
class RenderEngine;
class SoundManager;
class TextureManager;

namespace sf
{
	class RenderWindow;
	class Font;
}
class IGame
{
public:
	virtual ~IGame(){}

	RenderEngine*   getRenderEenine(){ return mRenderEngine; }
	SoundManager*   getSoundMgr(){ return mSoundMgr; }
	TextureManager* getTextureMgr(){ return mTextureMgr; }

	Vec2i const&  getScreenSize(){ return mScreenSize; }
	Vec2i const&  getMousePos(){ return mMousePos; }

	
	virtual void  addStage( GameStage* stage, bool removePrev ) = 0;
	virtual void  procWidgetEvent( int event , int id , GWidget* sender ) = 0;
	virtual void  procSystemEvent() = 0;

	virtual sf::Font* getFont( int idx ) = 0;
	virtual sf::RenderWindow* getWindow() = 0;

protected:
	Vec2i           mScreenSize;
	RenderEngine*   mRenderEngine;
	SoundManager*   mSoundMgr;
	TextureManager* mTextureMgr;
	Vec2i           mMousePos;
};

IGame* getGame();

#endif // GameInterface_h__
