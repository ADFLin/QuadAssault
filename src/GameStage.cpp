#include "GameStage.h"

#include "GameInterface.h"
#include "RenderUtility.h"

SrceenFade::SrceenFade()
{
	mState     = eNONE;
	mFadeSpeed = 1.0f;
	mColor     = 0;
}

void SrceenFade::render()
{
	if ( mColor != 1.0 )
	{
		glEnable(GL_BLEND);
		glBlendFunc( GL_DST_COLOR, GL_ZERO );
		glColor3f( mColor, mColor , mColor );
		drawRect( Vec2f(0.0, 0.0) , Vec2f( getGame()->getScreenSize() ) );
		glDisable(GL_BLEND);	
	}
}

void SrceenFade::update( float dt )
{
	if ( mState == eNONE )
		return;
	switch( mState )
	{
	case eIN:
		mColor += mFadeSpeed * dt;
		if( mColor > 1.0f )
		{
			mState = eNONE;
			mColor=1.0f;
		}
		break;
	case eOUT:
		mColor -= mFadeSpeed * dt;
		if( mColor < 0.0f)
		{
			mState = eNONE;
			mColor = 0.0f;
		}
		break;
	}

	if ( mState == eNONE )
	{
		if ( mFunFisish )
			mFunFisish();
	}
}

GameStage::GameStage()
{
	mNeedStop = false;
}
