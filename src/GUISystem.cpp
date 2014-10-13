#include "GUISystem.h"

#include "Dependence.h"
#include "GameInterface.h"
#include "GameInput.h"

#include "RenderSystem.h"
#include "RenderUtility.h"

class ScissorClipStack
{
public:
	
	void push( Vec2i const& pos , Vec2i const& size , bool bOverlapPrev );
	void pop();

	struct Rect
	{
		Vec2i min;
		Vec2i max;

		Vec2i getSize(){ return max - min; }
		bool  overlap( Rect const& other )
		{
			int xMin = std::max( min.x, other.min.x );
			int yMin = std::max( min.y, other.min.y );
			int xMax = std::min( max.x , other.max.x );
			int yMax = std::min( max.y , other.max.y );

			if ( xMax  >= xMin && yMax >= yMin )
			{
				min.x = xMin;
				min.y = yMin;
				max.x = xMax;
				max.y = yMax;
				return true;
			}

			max = min;
			return false;
		}
	};
	bool mPrevEnable;
	std::vector< Rect > mStack;
};

ScissorClipStack gClipStack;

void ScissorClipStack::push( Vec2i const& pos , Vec2i const& size , bool bOverlapPrev )
{
	Rect rect;
	rect.min = pos;
	rect.max = pos + size;

	if ( mStack.empty() )
	{
		mPrevEnable = glIsEnabled( GL_SCISSOR_TEST );
		if ( !mPrevEnable )
			glEnable( GL_SCISSOR_TEST );
	}
	else if ( bOverlapPrev )
	{
		rect.overlap( mStack.back() );
	}
	Vec2i sizeRect = rect.getSize();
	glScissor( rect.min.x , rect.min.y , sizeRect.x , sizeRect.y );
	mStack.push_back( rect );
}

void ScissorClipStack::pop()
{
	mStack.pop_back();

	if ( mStack.empty() )
	{
		if ( !mPrevEnable )
			glDisable( GL_SCISSOR_TEST );
	}
	else
	{
		Rect& rect = mStack.back();
		Vec2i size = rect.getSize();
		glScissor( rect.min.x , rect.min.y , size.x , size.y );
	}
}

void GUISystem::sendMessage( int event , int id , GWidget* sender )
{
	getGame()->procWidgetEvent( event , id , sender );
}

GWidget* GUISystem::findTopWidget( int id , GWidget* start )
{
	GUI::Core* root = mManager.getRoot();
	GWidget* child = ( start ) ? root->nextChild( start ) : root->getChild();
	while( child )
	{
		if ( child->getID() == id )
			return child;
		child = root->nextChild( child );
	}
	return NULL;
}

void GUISystem::render()
{
	mManager.render();
}

void GUISystem::addWidget( GWidget* widget )
{
	mManager.addUI( widget );
}

void GUISystem::cleanupWidget()
{
	mManager.cleanupUI();
}

GWidget::GWidget( Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:TUICore< GWidget >( pos , size , parent )
{
	mId = UI_ANY;
	mClipEnable = false;
	mUserData = 0;
}

GWidget::~GWidget()
{

}

void GWidget::sendEvent( int eventID )
{
	GUI::Core* root = getManager()->getRoot(); 
	GUI::Core* ui   = this;
	while ( ui != root )
	{
		if ( ui->isTopUI())
			break;

		ui = ui->getParent();
		GWidget* widget = static_cast< GWidget* > ( ui );

		if ( !widget->onChildEvent( eventID, mId , this ) )
			return;
	}
	GUISystem::getInstance().sendMessage( eventID , mId , this );
}

GWidget* GWidget::findChild( int id , GWidget* start )
{
	GWidget* child = ( start ) ? nextChild( start ) : getChild();

	while( child )
	{
		if ( child->getID() == id )
			return child;
		child = nextChild( child );
	}
	return NULL;
}

void GWidget::doRenderAll()
{
	if ( mClipEnable )
	{
		Vec2i pos  = getWorldPos();
		Vec2i size = getSize();
		pos.y = getGame()->getScreenSize().y - ( pos.y + size.y );
		gClipStack.push( pos , size , true );
	}

	TUICore< GWidget >::doRenderAll();
	GWidget* ui = getChild();
	while( ui )
	{
		ui->onRenderSiblingsEnd();
		ui = nextChild( ui );
	}

	if ( mClipEnable )
	{
		gClipStack.pop();
	}
}


GTextButton::GTextButton( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:BaseClass( id , pos , size , parent )
{
	text = IText::create( getGame()->getFont(0) , 24 , Color( 255 , 255 , 0 ) );

}

GTextButton::~GTextButton()
{
	text->release();
}

void GTextButton::onRender()
{
	Vec2i const& pos  = getWorldPos();
	Vec2i const& size = getSize();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	if( isEnable() )
	{
		if( getButtonState() == BS_HIGHLIGHT )
			glColor3f(0.0, 0.5, 0.0);
		else
			glColor3f(0.0, 0.25, 0.0);
	}
	else
	{
		glColor3f(0.05, 0.05, 0.05);
	}

	drawRect( pos , size );

	if( isEnable() )
	{
		if( getButtonState() == BS_HIGHLIGHT )
			glColor3f(0.0, 1.0, 0.0);
		else
			glColor3f(0.0, 0.5, 0.0);
	}
	else
	{
		glColor3f(0.1, 0.1, 0.1);
	}

	drawRectLine( pos , size );

	glColor3f(1.0, 1.0, 1.0);

	glDisable(GL_BLEND);

	if( isEnable() )
		text->setColor(Color(50,255,50));
	else
		text->setColor(Color(150,150,150));

	getRenderSystem()->drawText( text , pos  + size / 2  );

}

int const TopSideHeight = 16;
GFrame::GFrame( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:BaseClass( pos , size , parent )
{
	mId = id;
	mTile = NULL;
	mbMiniSize = false;
}

GFrame::~GFrame()
{
	if ( mTile )
		mTile->release();
}

bool GFrame::onMouseMsg( MouseMsg const& msg )
{
	BaseClass::onMouseMsg( msg );

	static int x , y;
	static bool needMove = false;
	if ( msg.onLeftDown() )
	{
		setTop();

		if ( msg.getPos().y > getWorldPos().y &&
			 msg.getPos().y < getWorldPos().y + TopSideHeight )
		{
			x = msg.x();
			y = msg.y();

			getManager()->captureMouse( this );

			needMove = true;
		}
	}
	else if ( msg.onRightDown() )
	{
		if ( msg.getPos().y > getWorldPos().y &&
			 msg.getPos().y < getWorldPos().y + TopSideHeight )
		{ 
			mbMiniSize = !mbMiniSize;
			if ( mbMiniSize )
			{
				mPrevSize = getSize();
				setSize( Vec2i( mPrevSize.x , TopSideHeight ) );
				mClipEnable = true;
			}
			else
			{
				setSize( mPrevSize );
				mClipEnable = false;
			}
		}

	}
	else if ( msg.onLeftUp() )
	{
		needMove = false;
		getManager()->releaseMouse();
	}
	if ( needMove )
	{
		if ( msg.isLeftDown() && msg.isDraging() )
		{
			Vec2i pos = getPos() +Vec2i( msg.x() - x , msg.y() - y );
			setPos( pos );
			x = msg.x();
			y = msg.y();
		}
	}
	return false;
}

void GFrame::onRender()
{
	Vec2i pos = getWorldPos();
	Vec2i size = getSize();
	//TIJELO PROZORA
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f(0.0, 0.25, 0.0 , 0.4 );
	drawRect( pos , size );
	glDisable(GL_BLEND);

	//NASLOVNICA (TRAKA)
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 0.0); glVertex2f(pos.x, pos.y);
	glColor3f(0.0, 1.0, 0.0); glVertex2f(pos.x+size.x, pos.y);
	glColor3f(0.0, 0.0, 0.0); glVertex2f(pos.x+size.x, pos.y+TopSideHeight);
	glColor3f(0.0, 0.0, 0.0); glVertex2f(pos.x, pos.y+TopSideHeight);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);

	if ( mTile )
	{
		getRenderSystem()->drawText( mTile , pos + Vec2i( 10 , 0 ) , TEXT_SIDE_LEFT | TEXT_SIDE_TOP );
	}
}

void GFrame::setTile( char const* name )
{
	if ( !mTile )
	{
		mTile = IText::create( getGame()->getFont(0) , 20 , Color( 255 , 255 , 0 ) );
	}
	mTile->setString( name );
}

void GPanel::onRender()
{
	Vec2i pos = getWorldPos();
	Vec2i size = getSize();
	//TIJELO PROZORA
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 0.0 , 0.25, 0.0 , 0.8 );
	drawRect( pos , size );
	glDisable(GL_BLEND);
}

GPanel::GPanel( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:BaseClass( pos , size , parent )
{
	mId = id;
}

GImageButton::GImageButton( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:BaseClass( id , pos , size , parent )
{
	mHelpText = NULL;
}

GImageButton::~GImageButton()
{
	if ( mHelpText )
		mHelpText->release();
}

void GImageButton::onRender()
{
	if( getButtonState() == BS_HIGHLIGHT )
		glColor3f(0.25, 0.25, 0.25);
	else
		glColor3f(1.0, 1.0, 1.0);

	Vec2f pos;
	pos.x = getWorldPos().x; 
	pos.y = getWorldPos().y; 
	Vec2f size;
	size.x = getSize().x;
	size.y = getSize().y;

	drawSprite( pos , size ,texImag );
	glColor3f(1.0, 1.0, 1.0);

}

void GImageButton::setHelpText( char const* str )
{
	if ( !mHelpText )
	{
		mHelpText = IText::create( getGame()->getFont(0) , 24 , Color( 255 , 255 , 255 ) );
	}
	mHelpText->setString( str );
}

void GImageButton::onRenderSiblingsEnd()
{
	if( getButtonState() == BS_HIGHLIGHT && mHelpText )
	{
		Vec2f sizeHelp = mHelpText->getBoundSize() + 2 * Vec2f( 4 , 4 );
		Vec2f posHelp = getGame()->getMousePos();

		glEnable(GL_BLEND);
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glColor4f(0.0, 0.25, 0.0 , 0.4 );
		drawRect( posHelp , sizeHelp );
		glDisable(GL_BLEND);
		getRenderSystem()->drawText( mHelpText , posHelp + sizeHelp / 2 );
	}
}

GTextCtrl::GTextCtrl( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:BaseClass( pos , size , parent )
{
	mId = id;
	text = IText::create();
	mClipEnable = true;
}


GTextCtrl::~GTextCtrl()
{
	text->release();
}

void GTextCtrl::onRender()
{
	Vec2i pos = getWorldPos();
	Vec2i size = getSize();

	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 0.1, 0.1 , 0.1 , 0.8f );
	drawRect( pos , size );
	glColor3f( 1,1,1);
	glDisable(GL_BLEND);


	if( isFocus() )
		text->setColor(Color(50,255,50));
	else
		text->setColor(Color(150,150,150));

	getRenderSystem()->drawText( text , pos + size / 2 );
}

void GTextCtrl::setFontSize( unsigned size )
{
	text->setCharSize( size );
}

void GTextCtrl::onModifyValue()
{
	text->setString( getValue() );
}

void GTextCtrl::onEditText()
{
	text->setString( getValue() );
	sendEvent( EVT_TEXTCTRL_CHANGE );
}

void GTextCtrl::onFocus( bool beF )
{
	Input::sKeyBlocked = beF;
}

GChoice::GChoice( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:BaseClass(  pos , size , parent )
{
	mId = id;
}

void GChoice::onAddItem( Item& item )
{
	item.text = IText::create( getGame()->getFont( 0 ) , 20 , Color( 150,150,150 ) );
	item.text->setString( item.value.c_str() );
}

void GChoice::onRemoveItem( Item& item )
{
	item.text->release();
}

void GChoice::doRenderItem( Vec2i const& pos , Item& item , bool beLight )
{
	Vec2i size( getSize().x , getMenuItemHeight() );
	if ( beLight )
	{
		glEnable(GL_BLEND);
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glColor4f( 0.1, 0.1 , 0.1 , 0.8f );
		drawRect( pos + Vec2i( 2 , 2 ) , size - Vec2i( 4 , 4 ) );
		glColor3f( 1,1,1);
		glDisable(GL_BLEND);
	}
	getRenderSystem()->drawText( item.text , pos + size / 2 );
}

void GChoice::onRender()
{
	Vec2i pos = getWorldPos();
	Vec2i size = getSize();

	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 0.1, 0.1 , 0.1 , 0.8f );
	drawRect( pos , size );
	glColor3f( 1,1,1);
	glDisable(GL_BLEND);

	if ( mCurSelect != -1 )
	{
		Item& item = mItemList[ mCurSelect ];
		getRenderSystem()->drawText( item.text , pos + size / 2 );
	}
}

void GChoice::doRenderMenuBG( Menu* menu )
{
	Vec2i pos =  menu->getWorldPos();
	Vec2i size = menu->getSize();

	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 0.5, 0.5 , 0.5 , 0.8f );
	drawRect( pos , size );
	glColor3f( 1,1,1);
	glDisable(GL_BLEND);
}

GListCtrl::GListCtrl( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:BaseClass(  pos , size , parent )
{
	mId = id;
}


void GListCtrl::onAddItem( Item& item )
{
	item.text = IText::create( getGame()->getFont( 0 ) , 20 , Color( 255 , 255 , 255 ) );
	item.text->setString( item.value.c_str() );
}

void GListCtrl::onRemoveItem( Item& item )
{
	item.text->release();
}

void GListCtrl::doRenderItem( Vec2i const& pos , Item& item , bool beSelected )
{
	Vec2i size( getSize().x , getItemHeight() );
	if ( beSelected )
	{
		glEnable(GL_BLEND);
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glColor4f( 0.1, 0.1 , 0.1 , 0.8f );
		drawRect( pos + Vec2i( 2 , 2 ) , size - Vec2i( 4 , 4 ) );
		glColor3f( 1,1,1);
		glDisable(GL_BLEND);
	}
	getRenderSystem()->drawText( item.text , pos + size / 2 );
}

void GListCtrl::doRenderBackground( Vec2i const& pos , Vec2i const& size )
{
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 0.5, 0.5 , 0.5 , 0.8f );
	drawRect( pos , size );
	glColor3f( 1,1,1);
	glDisable(GL_BLEND);
}
