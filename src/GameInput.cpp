#include "GameInput.h"

#include "Platform.h"

bool Input::sKeyBlocked = false;

bool Input::isKeyPressed( unsigned key )
{
	if ( sKeyBlocked )
		return false;
	return Platform::isKeyPressed( key );
}


