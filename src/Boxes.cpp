// Boxes.cpp Copyright (c) 08/01/2023;  D.A.Tiger; GNU GPL 3
#include<Boxes.h>

/*** FIND BAR *************************************************************************************/
FXDEFMAP( FindBar ) FB_MAP[ ] = {
  FXMAPFUNC( SEL_COMMAND, FindBar::ID_ACTION, FindBar::onCmd_Action ),
  FXMAPFUNC( SEL_COMMAND, FindBar::ID_CLEAN,  FindBar::onCmd_Action )
};

FXIMPLEMENT( FindBar, FXHeaderBox, FB_MAP, ARRAYNUMBER( FB_MAP ) )

FindBar::FindBar( FXWindowHeader *p, IconsTheme *ict,  FXObject *tgt, FXSelector sel, FXuint opts )
       : FXHeaderBox( p, NULL, 0, opts | LAYOUT_CENTER_Y  ) 
{
  m_target   = tgt;
  m_message  = sel;

  new FXButton( this, "\t\t Hledat", ict->getIcon( "Actions_big/system-search.png" ), this, FindBar::ID_ACTION, BUTTON_TOOLBAR | FRAME_RAISED );
  m_field = new FXTextField( this, 25, NULL, 0, TEXTFIELD_NORMAL | LAYOUT_CENTER_Y );
}

void FindBar::create( )
{
  FXHeaderBox::create( );
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

/*** END ******************************************************************************************/

