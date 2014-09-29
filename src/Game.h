#ifndef Game_h__
#define Game_h__

#include "GameInterface.h"
#include "Base.h"
#include "Dependence.h"
#include "GameStage.h"


#include <vector>

class Game : public IGame
{
public:
	Game();

	bool init(char* configFile);
	void run();
	void exit();

	float GetFrameTime();
	void  addStage( GameStage* stage, bool removePrev );
	void  procWidgetEvent( int event , int id , GWidget* sender );
	void  procSystemEvent();
	sf::RenderWindow* getWindow();
	sf::Font*         getFont( int idx ){  return mFonts[idx]; }

private:

	unsigned mMouseState;
	bool     gotovo;
	std::vector<GameStage*> mStageStack;
	std::vector<sf::Font*>  mFonts;
	sf::RenderWindow rw;
	sf::Clock mClock;

};




#endif // Game_h__