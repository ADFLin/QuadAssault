#ifndef DevStage_h__
#define DevStage_h__

#include "PropFrame.h"
class DevStage : public GameStage
{
public:

	virtual bool onInit()
	{

		mTexCursor = getGame()->getTextureMgr()->getTexture("cursor.tga");

		GUISystem::getInstance().cleanupWidget();
		PropFrame* frame = new PropFrame( UI_ANY , Vec2i( 10 , 10 ) , NULL );
		val = 1;
		val2 = 2;

		frame->addProp( "a1" , val );
		frame->addProp( "a2" , val2 );
		GUISystem::getInstance().addWidget( frame );

		GTextButton* button;
		button = new GTextButton( UI_ANY , Vec2i( 100 , 100 ) , Vec2i(128, 64) , NULL );
		button->text->setFont( getGame()->getFont(0) );
		button->text->setString( "Start" );
		GUISystem::getInstance().addWidget( button );

		frame->inputData();
		return true;
	}

	virtual void onExit()
	{
		
	}

	virtual void onUpdate( float deltaT )
	{
		getGame()->procSystemEvent();
	}

	virtual void onRender()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		GUISystem::getInstance().render();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		drawSprite( Vec2f( getGame()->getMousePos() ) - Vec2f(16,16) ,Vec2f(32,32), mTexCursor );
		glDisable(GL_BLEND);
	}

	int val;
	float val2;
	Texture*    mTexCursor;
};

#endif // DevStage_h__
