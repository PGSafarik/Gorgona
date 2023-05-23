// Boxes.cpp Copyright (c) 08/01/2023;  D.A.Tiger; GNU GPL 3
#include<Boxes.h>

/*** FIND BAR *************************************************************************************/
FXDEFMAP( FindBar ) FB_MAP[ ] = {
  FXMAPFUNC( SEL_COMMAND, FindBar::ID_ACTION, FindBar::onCmd_Action ),
  FXMAPFUNC( SEL_COMMAND, FindBar::ID_CLEAN,  FindBar::onCmd_Action )
};

FXIMPLEMENT( FindBar, FXHeaderBox, FB_MAP, ARRAYNUMBER( FB_MAP ) )

FindBar::FindBar( FXPrimaryWindow *p, IconsTheme *ict,  FXObject *tgt, FXSelector sel, FXuint opts )
       : FXHeaderBox( p->getHeader( ), NULL, 0, opts | LAYOUT_CENTER_Y, 1, 2 ) 
{
  m_target   = tgt;
  m_message  = sel;
  
  setColorize( false );

  m_find_btn = new FXButton( this, "\t\t Hledat", ict->getIcon( "Actions_big/system-search.png" ), this, FindBar::ID_ACTION, BUTTON_TOOLBAR | FRAME_RAISED );
  m_field = new FXTextField( this, 25, NULL, 0, TEXTFIELD_NORMAL | LAYOUT_CENTER_Y );
}

void FindBar::create( )
{
  FXHeaderBox::create( );
  getBoxFrame( )->_recolorize( m_find_btn );
}


long FindBar::onCmd_Action( FXObject *sender, FXSelector sel, void *data  )
{
  long res = 0;  
  
  switch( FXSELID( sel ) ) {
    case FindBar::ID_ACTION : 
    {
      res = Notify( ); 
      break;
    }
    case FindBar::ID_CLEAN :
    {
      clean( );
      res = 1;
    }
  }

  return res;
}

long FindBar::Notify( ) 
{
  long resh = 0;
  if( m_target && m_message != 0 ) { resh = m_target->handle( this, FXSEL( SEL_COMMAND, m_message ), ( void * ) m_field->getText( ).text( ) ); }
  if( resh != 0 ) { clean( ); }  

  return resh; 
}


/*** Tool Bar *************************************************************************************/
FXIMPLEMENT( ToolBar, FXHeaderBox, NULL, 0 )

ToolBar::ToolBar( FXPrimaryWindow *p, IconsTheme *ict, FXuint opts )
       : FXHeaderBox( p->getHeader( ), NULL, 0, opts | LAYOUT_CENTER_Y | FRAME_GROOVE, 0, 0 ) 
{
  m_ict = ict;
}

void ToolBar::create( )
{
  FXHeaderBox::create( );
}

void ToolBar::makeSeparator( )
{
  new FXVerticalSeparator( this, SEPARATOR_GROOVE | LAYOUT_CENTER_Y | LAYOUT_FILL_Y );
}

void ToolBar::makeButton( const FXString &Title, const FXString &icon, FXObject *tgt, FXSelector sel )
{
  new FXButton( this, Title, m_ict->getIcon( icon ), tgt,  sel, BUTTON_TOOLBAR | ICON_ABOVE_TEXT | LAYOUT_CENTER_Y );
}


/*** Main Bar *************************************************************************************/
FXIMPLEMENT( MainBar, ToolBar, NULL, 0 )

MainBar::MainBar( FXPrimaryWindow *p, IconsTheme *ict, FXMenuPane *menu, FXuint opts )
       : ToolBar( p, ict, opts ) 
{
  setFrameStyle( FRAME_NONE );
  setLayoutHints ( LAYOUT_FILL_Y );
  m_button = new FXMenuButton( this, "\t\tMenu", p->getMenuIcon( ) /*ict->getIcon( "Actions_big/run-build-install-root.png" )*/, menu, BUTTON_TOOLBAR | FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_LEFT | LAYOUT_CENTER_Y  );
}

void MainBar::create( )
{
  ToolBar::create( );
}

/*** END ******************************************************************************************/

