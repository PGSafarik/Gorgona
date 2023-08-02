// FSM_Changes.cpp Copyright (c) 2023;  D.A.Tiger; GNU GPL 3
#include<FSM_Changes.h>

FXDEFMAP( FSM_Changes ) FSMCH_MAP[ ] = {
  FXMAPFUNC(  SEL_COMMAND, FSM_Changes::ID_CHANGE, FSM_Changes::OnCmd_Change ),
  FXMAPFUNCS( SEL_COMMAND, FSM_Changes::ID_ACCEPT, FSM_Changes::ID_IGNORE, FSM_Changes::OnCmd_Accept )
};
FXIMPLEMENT( FSM_Changes, FXObject, FSMCH_MAP, ARRAYNUMBER( FSMCH_MAP ) )

/**************************************************************************************************/
FSM_Changes::FSM_Changes( ) 
{ }

FSM_Changes::~FSM_Changes( )
{ }

/**************************************************************************************************/
long FSM_Changes::OnCmd_Change( FXObject *sender, FXSelector sel, void *data )
{
  FXuint st = FXSELTYPE( sel );

  if( st == SEL_COMMAND || ( st == SEL_UPDATE && m_counter == 0 ) ) { 
    this->m_counter++;
    return 1;
  }

  return 0;
}

long FSM_Changes::OnCmd_Accept( FXObject *sender, FXSelector sel, void *data )
{
  long res = 1;

  if( this->m_counter == 0 ) { res = 0; } 
  else {
    switch( FXSELID( sel ) ) {
      case FSM_Changes::ID_ACCEPT: {
        m_counter--;
        break;
      }
      case FSM_Changes::ID_DISCARD: {
        m_counter = 0;
        break;
      }
      case FSM_Changes::ID_IGNORE: {
        if( m_cv ) { m_counter = 0; }
        break;
      }
    }
  }

  return res;
}

/*** END ******************************************************************************************/
