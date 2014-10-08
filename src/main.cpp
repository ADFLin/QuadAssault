#include "Game.h"
#include "MenuStage.h"

int main()
{
	try
	{
		Game* game = new Game();

		if( !game->init("config.txt") )
			return -1;

		game->addStage( new MenuStage(), false );
		game->run();
		game->exit();

	}
	catch( ... )
	{





	}

	return 0;
}