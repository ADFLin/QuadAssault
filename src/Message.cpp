#include "Message.h"

#include "Level.h"
#include "TextureManager.h"
#include "SoundManager.h"

#include "GameInterface.h"
#include "Texture.h"

void Message::init( string const& sender, string const& content, float durstion, string const& soundName )
{	
	mPos.x=64;
	mPos.y=64;
	mSoundName = soundName;
	mDurstion  = durstion;
	timer=0.0;
	unisten=false;

	p_text.setString(sender);
	p_text.setPosition(mPos.x+48, mPos.y+4);
	p_text.setFont( *getGame()->getFont(0) );
	p_text.setCharacterSize(24);	
	p_text.setColor(sf::Color(25,255,25));

	text.setString(content);
	text.setPosition(mPos.x+48, mPos.y+4+24);
	text.setFont( *getGame()->getFont(0) );
	text.setCharacterSize(24);
	text.setColor(sf::Color(255,255,255));

	portrait = getGame()->getTextureMgr()->getTexture("portrait2.tga");	
}

void Message::nodifyShow()
{
	sound = getGame()->getSoundMgr()->addSound( mSoundName.c_str() );			
	sound->play();
}

void Message::tick()
{
	if( timer < mDurstion )
	{
		timer += TICK_TIME;
	}
	else
	{
		if( sound )
			sound->stop();
		unisten=true;		
		sound=NULL;
	}
}
void Message::updateRender( float dt )
{

}

void Message::renderFrame()
{

	float width=text.getLocalBounds().width;

	glColor3f(0.0, 0.25, 0.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);		
	glBegin(GL_QUADS); //PUNI KVADRAT PO CIJELOM
	glTexCoord2f(0.0, 0.0); glVertex2f(mPos.x+32,mPos.y);
	glTexCoord2f(1.0, 0.0); glVertex2f(mPos.x+32+width+48,mPos.y);
	glTexCoord2f(1.0, 1.0); glVertex2f(mPos.x+32+width+48,mPos.y+64);
	glTexCoord2f(0.0, 1.0);	glVertex2f(mPos.x+32,mPos.y+64);
	glEnd();
	glDisable(GL_BLEND);

	glColor3f(1.0, 1.0, 1.0);	

	glEnable(GL_TEXTURE_2D);//portrait

	portrait->bind();
	glBegin(GL_QUADS); 
	glTexCoord2f(0.0, 0.0); glVertex2f(mPos.x-32,mPos.y);
	glTexCoord2f(1.0, 0.0); glVertex2f(mPos.x+32,mPos.y);
	glTexCoord2f(1.0, 1.0); glVertex2f(mPos.x+32,mPos.y+64);
	glTexCoord2f(0.0, 1.0);	glVertex2f(mPos.x-32,mPos.y+64);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.1, 1.0, 0.1);	

	glBegin(GL_LINE_LOOP); //ZELENI OBRUB
	glTexCoord2f(0.0, 0.0); glVertex2f(mPos.x-32,mPos.y);
	glTexCoord2f(1.0, 0.0); glVertex2f(mPos.x-32+width+112,mPos.y);
	glTexCoord2f(1.0, 1.0); glVertex2f(mPos.x-32+width+112,mPos.y+64);
	glTexCoord2f(0.0, 1.0);	glVertex2f(mPos.x-32,mPos.y+64);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);	
}

void Message::render()
{	
	renderFrame();

	getGame()->getWindow()->pushGLStates();
	getGame()->getWindow()->draw(p_text);	
	getGame()->getWindow()->draw(text);
	getGame()->getWindow()->popGLStates();
}
