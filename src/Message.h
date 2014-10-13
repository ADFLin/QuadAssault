#ifndef PORUKA_H
#define PORUKA_H

#include "Base.h"
#include "Object.h"

class Level;
class Sound;
class IText;

class Message
{

public:
	~Message();
	void init( String const& sender, String const& content, float durstion, String const& soundName );
	void nodifyShow();
	void tick();
	void updateRender( float dt );


	void renderFrame();
	void render();
	bool unisten;
private:	
	IText*  p_text;
	IText*  text;
	Vec2f   mPos;
	float   mDurstion; //trajanje poruke u sekundama

	float    timer; //pocinje od nule	
	Texture* portrait;
	String   mSoundName;	
	Sound*   sound;

};

#endif