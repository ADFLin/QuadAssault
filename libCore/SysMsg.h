#ifndef SysMsg_h__
#define SysMsg_h__

#include "TVector2.h"
typedef TVector2< int > Vec2i;

#ifndef BIT
#define BIT(i) ( 1 << (i) )
#endif


enum MsgType
{
	MSG_TYPE_CHAR  ,
	MSG_TYPE_MOUSE ,
	MSG_TYPE_KEY   ,
};

struct Keyboard 
{
	enum Enum
	{
		eLBUTTON        = 0x01 ,
		eRBUTTON        = 0x02 ,
		eCANCEL         = 0x03 ,
		eMBUTTON        = 0x04 ,

		eBACK           = 0x08 ,
		eTAB            = 0x09 ,
		eCLEAR          = 0x0C ,
		eRETURN         = 0x0D ,

		eSHIFT          = 0x10 ,
		eCONTROL        = 0x11 ,
		eMENU           = 0x12 ,
		ePAUSE          = 0x13 ,
		eCAPITAL        = 0x14 ,

		eKANA           = 0x15 ,
		eJUNJA          = 0x17 , 
		eFINAL          = 0x18 ,
		eHANJA          = 0x19 ,
		eKANJI          = 0x19 ,

		eESCAPE         = 0x1B ,

		eCONVERT        = 0x1C ,
		eNONCONVERT     = 0x1D ,
		eACCEPT         = 0x1E ,
		eMODECHANGE     = 0x1F ,

		eSPACE          = 0x20 ,
		ePRIOR          = 0x21 ,
		eNEXT           = 0x22 ,
		eEND            = 0x23 ,
		eHOME           = 0x24 ,
		eLEFT           = 0x25 ,
		eUP             = 0x26 ,
		eRIGHT          = 0x27 ,
		eDOWN           = 0x28 ,
		eSELECT         = 0x29 ,
		ePRINT          = 0x2A ,
		eEXECUTE        = 0x2B ,
		eSNAPSHOT       = 0x2C ,
		eINSERT         = 0x2D ,
		eDELETE         = 0x2E ,
		eHELP           = 0x2F ,

		eNUM0           = '0' ,
		eNUM1 , eNUM2 , eNUM3 , eNUM4 , eNUM5 ,
		eNUM6 , eNUM7 , eNUM8 , eNUM9 ,
		eA              = 'A',
		eB , eC , eD , eE , eF ,
		eG , eH , eI , eJ , eK ,
		eL , eM , eN , eO , eP ,
		eQ , eR , eS , eT , eU ,
		eV , eW , eX , eY , eZ ,

		eSLEEP          = 0x5F ,

		eNUMPAD0        = 0x60 ,
		eNUMPAD1 , eNUMPAD2 , eNUMPAD3 , eNUMPAD4 , eNUMPAD5 ,
		eNUMPAD6 , eNUMPAD7 , eNUMPAD8 , eNUMPAD9 ,
		eMULTIPLY       = 0x6A ,
		eADD            = 0x6B ,
		eSEPARATOR      = 0x6C ,
		eSUBTRACT       = 0x6D ,
		eDECIMAL        = 0x6E ,
		eDIVIDE         = 0x6F ,

		eF1             = 0x70 ,
		eF2 , eF3 , eF4 , eF5 , eF6 , 
		eF7 , eF8 , eF9 , eF10 , eF11 , 
		eF12 , eF13 , eF14 , eF15 ,

		eNUMLOCK        = 0x90 ,
		eSCROLL         = 0x91 ,

		eLSHIFT         = 0xA0 ,
		eRSHIFT         = 0xA1 ,
		eLCONTROL       = 0xA2 ,
		eRCONTROL       = 0xA3 ,
		eLMENU          = 0xA4 ,
		eRMENU          = 0xA5 ,
	};
};
class SysMessage
{
public:
	SysMessage( MsgType type) : m_type( type ){}
	MsgType getType(){ return m_type; }
private:
	MsgType m_type;
};

struct Mouse
{
	enum Enum
	{
		eLBUTTON = 0,
		eMBUTTON  ,
		eRBUTTON  ,
		eXBUTTON1 ,
		eXBUTTON2 ,
	};
};

enum MouseState
{
	MBS_LEFT         = BIT(0),
	MBS_MIDDLE       = BIT(1),
	MBS_RIGHT        = BIT(2),
	MBS_DOUBLE_CLICK = BIT(4),
	MBS_DOWN         = BIT(5),
	MBS_MOVING       = BIT(6),
	MBS_WHEEL        = BIT(7),

	MBS_BUTTON_MASK  = MBS_LEFT | MBS_MIDDLE | MBS_RIGHT,
	MBS_BUTTON_ACTION_MASK = MBS_DOWN | MBS_DOUBLE_CLICK ,
	MBS_ACTION_MASK  = MBS_BUTTON_ACTION_MASK | MBS_WHEEL | MBS_MOVING ,
};



class MouseMsg : public SysMessage
{
public:
	typedef unsigned short uint16;
	MouseMsg()
		: SysMessage( MSG_TYPE_MOUSE )
		, msg(0),state(0)
	{

	}
	MouseMsg( Vec2i const& pos , uint16 m , uint16 s )
		: SysMessage( MSG_TYPE_MOUSE )
		, pos( pos ), msg( m ), state(s )
	{
	}
	MouseMsg( int x , int y , uint16 m , uint16 s )
		: SysMessage( MSG_TYPE_MOUSE )
		,pos(x,y),msg( m ) ,state(s)
	{
	}
	int    x() const { return pos.x; }
	int    y() const { return pos.y; }
	Vec2i const& getPos() const { return pos; }
	void   setPos( Vec2i const& p ){ pos = p; }

	bool isLeftDown()   const { return ( state & MBS_LEFT ) != 0; }
	bool isMiddleDown() const { return ( state & MBS_MIDDLE ) != 0; }
	bool isRightDown()  const { return ( state & MBS_RIGHT ) != 0; }
	bool isDraging()    const { return ( msg & MBS_MOVING ) && ( state != 0 ); }

	bool onMoving()     const { return ( msg & MBS_MOVING ) != 0; }
	bool onWheelFront() const { return msg ==  MBS_WHEEL ;}
	bool onWheelBack()  const { return msg ==  ( MBS_WHEEL | MBS_DOWN );}

	bool onDown()       const { return ( msg & MBS_DOWN ) != 0; }
	bool onLeftUp()     const { return msg == MBS_LEFT; }
	bool onMiddleUp()   const { return msg == MBS_MIDDLE; }
	bool onRightUp()    const { return msg == MBS_RIGHT; }
	bool onLeftDClick() const { return msg ==( MBS_LEFT | MBS_DOUBLE_CLICK); }
	bool onRightDClick()const { return msg ==( MBS_RIGHT | MBS_DOUBLE_CLICK); }
	bool onLeftDown()   const { return msg ==( MBS_LEFT | MBS_DOWN ); }
	bool onRightDown()  const { return msg ==( MBS_RIGHT | MBS_DOWN ); }
	bool onMiddleDown() const { return msg ==( MBS_MIDDLE | MBS_DOWN ); }

	uint16  getState() const { return state; }
	uint16  getMsg()   const { return msg;  }
private:
	Vec2i  pos;
	uint16  msg;
	uint16  state;
};

#endif // SysMsg_h__
