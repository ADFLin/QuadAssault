#ifndef PORUKA_H
#define PORUKA_H

#include "Dependence.h"
#include "Object.h"

class Level;
class Sound;
class IText;

class Message
{

public:
	~Message();
	void init( string const& sender, string const& content, float durstion, string const& soundName );
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
	string   mSoundName;	
	Sound*   sound;

};

#endif