#ifndef DevStage_h__
#define DevStage_h__

#include "GameStage.h"
#include "RenderSystem.h"


class TestBase
{
public:
	virtual bool onInit() = 0;
	virtual void setDevMsg( FString& str ){}
	virtual void onUpdate( float dt ){}
	virtual void onRender() = 0;
	virtual bool onMouse( MouseMsg const& msg ){ return true; }
};

class DevStage : public GameStage
{
public:
	DevStage();
	~DevStage();

	virtual bool onInit();
	virtual void onExit();
	virtual void onUpdate( float deltaT );
	virtual void onRender();
	virtual bool onKey( unsigned key , bool isDown );
	virtual bool onMouse(MouseMsg const& msg);

	FPtr< TestBase > mTest;
	Texture*  mTexCursor;
	FObjectPtr< IText > mDevMsg;
};

#endif // DevStage_h__
