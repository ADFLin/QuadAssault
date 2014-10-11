#include "PlatformSF.h"

static sf::Clock gClock;
int64 PlatformSF::getTickCount()
{
	return  gClock.getElapsedTime().asMilliseconds();
}

GameWindow* PlatformSF::createWindow( char const* title , Vec2i const& size , int colorBit , bool bFullScreen )
{
	GameWindow* win = new GameWindow;
	if ( !win->create( title , size , colorBit , bFullScreen ) )
	{
		delete win;
		return NULL;
	}
	return win;
}

GLContext* PlatformSF::createGLContext( GameWindow& window , GLConfig& config )
{
	GLContext* context = new GLContext;
	if ( !context->init( window , config ) )
	{
		delete context;
		return NULL;
	}
	return context;
}

sf::Keyboard::Key convertToSFKey( unsigned key )
{
	//FIXME
	switch( key )
	{
	case Keyboard::eCANCEL : break;

	case Keyboard::eBACK:           break;
	case Keyboard::eTAB:            break;
	case Keyboard::eCLEAR:          break;
	case Keyboard::eRETURN:         break;

	case Keyboard::eSHIFT:          break;
	case Keyboard::eCONTROL:        break;
	case Keyboard::eMENU:           break;
	case Keyboard::ePAUSE:          break;
	case Keyboard::eCAPITAL:        break;

	case Keyboard::eKANA:           break;
	case Keyboard::eJUNJA:          break; 
	case Keyboard::eFINAL:          break;
	case Keyboard::eHANJA:          break;

	case Keyboard::eESCAPE:         break;

	case Keyboard::eCONVERT:        break;
	case Keyboard::eNONCONVERT:     break;
	case Keyboard::eACCEPT:         break;
	case Keyboard::eMODECHANGE:     break;

	case Keyboard::eSPACE:          break;
	case Keyboard::ePRIOR:          break;
	case Keyboard::eNEXT:           break;
	case Keyboard::eEND:            break;
	case Keyboard::eHOME:           break;
	case Keyboard::eLEFT:           break;
	case Keyboard::eUP:             break;
	case Keyboard::eRIGHT:          break;
	case Keyboard::eDOWN:           break;
	case Keyboard::eSELECT:         break;
	case Keyboard::ePRINT:          break;
	case Keyboard::eEXECUTE:        break;
	case Keyboard::eSNAPSHOT:       break;
	case Keyboard::eINSERT:         break;
	case Keyboard::eDELETE:         break;
	case Keyboard::eHELP:           break;

	case Keyboard::eNUM0: return sf::Keyboard::Num0; 
	case Keyboard::eNUM1: return sf::Keyboard::Num1; 
	case Keyboard::eNUM2: return sf::Keyboard::Num2; 
	case Keyboard::eNUM3: return sf::Keyboard::Num3; 
	case Keyboard::eNUM4: return sf::Keyboard::Num4; 
	case Keyboard::eNUM5: return sf::Keyboard::Num5; 
	case Keyboard::eNUM6: return sf::Keyboard::Num6; 
	case Keyboard::eNUM7: return sf::Keyboard::Num7; 
	case Keyboard::eNUM8: return sf::Keyboard::Num8; 
	case Keyboard::eNUM9: return sf::Keyboard::Num9; 
	case Keyboard::eA: return sf::Keyboard::A;    
	case Keyboard::eB: return sf::Keyboard::B;  
	case Keyboard::eC: return sf::Keyboard::C;  
	case Keyboard::eD: return sf::Keyboard::D;  
	case Keyboard::eE: return sf::Keyboard::E;  
	case Keyboard::eF: return sf::Keyboard::F;  
	case Keyboard::eG: return sf::Keyboard::G;  
	case Keyboard::eH: return sf::Keyboard::H;  
	case Keyboard::eI: return sf::Keyboard::I;  
	case Keyboard::eJ: return sf::Keyboard::J;  
	case Keyboard::eK: return sf::Keyboard::K;   
	case Keyboard::eL: return sf::Keyboard::L; 
	case Keyboard::eM: return sf::Keyboard::M; 
	case Keyboard::eN: return sf::Keyboard::N; 
	case Keyboard::eO: return sf::Keyboard::O; 
	case Keyboard::eP: return sf::Keyboard::P;
	case Keyboard::eQ: return sf::Keyboard::Q; 
	case Keyboard::eR: return sf::Keyboard::R; 
	case Keyboard::eS: return sf::Keyboard::S; 
	case Keyboard::eT: return sf::Keyboard::T; 
	case Keyboard::eU: return sf::Keyboard::U;
	case Keyboard::eV: return sf::Keyboard::V; 
	case Keyboard::eW: return sf::Keyboard::W; 
	case Keyboard::eX: return sf::Keyboard::X; 
	case Keyboard::eY: return sf::Keyboard::Y; 
	case Keyboard::eZ: return sf::Keyboard::Z;

	case Keyboard::eSLEEP:                    break;

	case Keyboard::eNUMPAD0: return sf::Keyboard::Numpad0;
	case Keyboard::eNUMPAD1: return sf::Keyboard::Numpad1;
	case Keyboard::eNUMPAD2: return sf::Keyboard::Numpad2;
	case Keyboard::eNUMPAD3: return sf::Keyboard::Numpad3;
	case Keyboard::eNUMPAD4: return sf::Keyboard::Numpad4;
	case Keyboard::eNUMPAD5: return sf::Keyboard::Numpad5;
	case Keyboard::eNUMPAD6: return sf::Keyboard::Numpad6;
	case Keyboard::eNUMPAD7: return sf::Keyboard::Numpad7;
	case Keyboard::eNUMPAD8: return sf::Keyboard::Numpad8;
	case Keyboard::eNUMPAD9: return sf::Keyboard::Numpad9;

	case Keyboard::eMULTIPLY:                  break;
	case Keyboard::eADD:                       break;
	case Keyboard::eSEPARATOR:                 break;
	case Keyboard::eSUBTRACT:                  break;
	case Keyboard::eDECIMAL:                   break;
	case Keyboard::eDIVIDE:                    break;

	case Keyboard::eF1:  return sf::Keyboard::F1;
	case Keyboard::eF2:  return sf::Keyboard::F2;
	case Keyboard::eF3:  return sf::Keyboard::F3; 
	case Keyboard::eF4:  return sf::Keyboard::F4; 
	case Keyboard::eF5:  return sf::Keyboard::F5;
	case Keyboard::eF6:  return sf::Keyboard::F6;
	case Keyboard::eF7:  return sf::Keyboard::F7; 
	case Keyboard::eF8:  return sf::Keyboard::F8;
	case Keyboard::eF9:  return sf::Keyboard::F9;
	case Keyboard::eF10: return sf::Keyboard::F10;
	case Keyboard::eF11: return sf::Keyboard::F11;
	case Keyboard::eF12: return sf::Keyboard::F12; 
	case Keyboard::eF13: return sf::Keyboard::F13;
	case Keyboard::eF14: return sf::Keyboard::F14; 
	case Keyboard::eF15: return sf::Keyboard::F15;

	case Keyboard::eNUMLOCK:  break;
	case Keyboard::eSCROLL:   break;

	case Keyboard::eLSHIFT:    return sf::Keyboard::LShift;
	case Keyboard::eRSHIFT:    return sf::Keyboard::RShift;
	case Keyboard::eLCONTROL:  return sf::Keyboard::LControl;
	case Keyboard::eRCONTROL:  return sf::Keyboard::RControl;
	case Keyboard::eLMENU:     return sf::Keyboard::LAlt;
	case Keyboard::eRMENU:     return sf::Keyboard::RAlt;
	}

	return sf::Keyboard::Unknown;
}

bool PlatformSF::isKeyPressed( unsigned key )
{
	return sf::Keyboard::isKeyPressed( convertToSFKey( key ) );
}

bool PlatformSF::isButtonPressed( unsigned button )
{
	sf::Mouse::Button buttonSF;
	switch (button)
	{
	case Mouse::eLBUTTON:  buttonSF = sf::Mouse::Left;     break;
	case Mouse::eRBUTTON:  buttonSF = sf::Mouse::Right;    break;
	case Mouse::eMBUTTON:  buttonSF = sf::Mouse::Middle;   break;
	case Mouse::eXBUTTON1: buttonSF = sf::Mouse::XButton1; break;
	case Mouse::eXBUTTON2: buttonSF = sf::Mouse::XButton2; break;
	}
	return sf::Mouse::isButtonPressed( buttonSF );
}

GameWindowSF::GameWindowSF()
{

	mListener = NULL;
	mMouseState = 0;
}


bool GameWindowSF::create( char const* title , Vec2i const& size , int colorBit , bool bFullScreen )
{
	int style = sf::Style::Close;		
	mImpl.create(sf::VideoMode( size.x , size.y , colorBit), title, style);
	return mImpl.isOpen();
}

void GameWindowSF::close()
{
	mImpl.close();
}



void GameWindowSF::showCursor( bool bShow )
{
	mImpl.setMouseCursorVisible( bShow );
}


GameWindowSF::~GameWindowSF()
{
	close();
}


int convertSFKey( sf::Keyboard::Key key )
{
	if ( key == sf::Keyboard::Unknown )
		return -1;
	if ( key <= sf::Keyboard::Z )
		return Keyboard::eA + ( key - sf::Keyboard::A );
	if ( key <= sf::Keyboard::Num9 )
		return Keyboard::eNUM0 + ( key - sf::Keyboard::Num0 );

	//FIXME
	switch( key )
	{
	case sf::Keyboard::Escape: return Keyboard::eESCAPE;
	case sf::Keyboard::Return: return Keyboard::eRETURN;
	case sf::Keyboard::BackSpace: return Keyboard::eBACK;
	case sf::Keyboard::Left:  return Keyboard::eLEFT;
	case sf::Keyboard::Right: return Keyboard::eRIGHT;
	case sf::Keyboard::Up:    return Keyboard::eUP;
	case sf::Keyboard::Down:  return Keyboard::eDOWN;
	case sf::Keyboard::Pause: return Keyboard::ePAUSE;
	}
	if ( key <  sf::Keyboard::Numpad0 )
		return -1;
	if ( key <= sf::Keyboard::Numpad9 )
		return Keyboard::eNUMPAD0 + ( key - sf::Keyboard::Numpad0 );
	if ( key <= sf::Keyboard::F15 )
		return Keyboard::eF1 + ( key - sf::Keyboard::F1 );

	return -1;
}


void GameWindowSF::procSystemMessage()
{
	sf::Event event;
	while( mImpl.pollEvent( event ) )
	{
		bool needSend = true;
		switch( event.type )
		{
		case sf::Event::Closed:
			mListener->onSystemEvent( SYS_WINDOW_CLOSE );
			break;
		case sf::Event::TextEntered:
			{
				uint16 out[2];
				uint16* end = sf::Utf32::toUtf16( &event.text.unicode , &event.text.unicode + 1 , out);
				for( uint16* it = out ; it != end ; ++it )
				{
					uint16 c = *it;
					if ( c & 0xff00 )
						mListener->onChar( c >> 8 );
					mListener->onChar( c & 0xff );
				}
			}
			break;
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			{
				int key = convertSFKey( event.key.code );
				if ( key != -1 )
					mListener->onKey( key , event.type == sf::Event::KeyPressed );
			}
			break;
		case sf::Event::MouseButtonReleased:
		case sf::Event::MouseButtonPressed:
			{
				unsigned msg = 0;

				switch( event.mouseButton.button )
				{
				case sf::Mouse::Button::Left:   msg |= MBS_LEFT; break;
				case sf::Mouse::Button::Right:  msg |= MBS_RIGHT; break;
				case sf::Mouse::Button::Middle: msg |= MBS_MIDDLE; break;
				}

				if ( event.type == sf::Event::MouseButtonPressed )
				{
					mMouseState |= msg;
					msg |= MBS_DOWN;
				}
				else
				{
					mMouseState &= ~msg;
				}

				mListener->onMouse( MouseMsg( event.mouseButton.x , event.mouseButton.y , msg , mMouseState ) );
			}
			break;
		case sf::Event::MouseMoved:
			{
				mListener->onMouse( MouseMsg( event.mouseMove.x , event.mouseMove.y , MBS_MOVING , mMouseState ) );
			}
			break;
		}
	}
}

void GLContextSF::swapBuffers()
{
	mWindow->display();
}

bool GLContextSF::setCurrent()
{
	return mWindow->setActive( true );
}

bool GLContextSF::init( GameWindowSF& window , GLConfig& config )
{
	mWindow = &window.mImpl;
	return true;
}

void GLContextSF::release()
{
	delete this;
}
