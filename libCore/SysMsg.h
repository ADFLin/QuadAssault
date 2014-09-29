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


class TMessage
{
public:
	TMessage( MsgType type) : m_type( type ){}
	MsgType getType(){ return m_type; }
private:
	MsgType m_type;
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

class MouseMsg : public TMessage
{
public:
	typedef unsigned short uint16;
	MouseMsg()
		: TMessage( MSG_TYPE_MOUSE )
		, msg(0),state(0)
	{

	}
	MouseMsg( Vec2i const& pos , uint16 m , uint16 s )
		: TMessage( MSG_TYPE_MOUSE )
		, pos( pos ), msg( m ), state(s )
	{
	}
	MouseMsg( int x , int y , uint16 m , uint16 s )
		: TMessage( MSG_TYPE_MOUSE )
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
