// Signal.cpp Copyright (c) 2024;  D.A.Tiger; GNU GPL 3
#include<Echidna/Signal.h>

using namespace ECHIDNA;
namespace ECHIDNA {
  FXIMPLEMENT( __SIGNAL, FXObject, NULL, 0 )
 
  __SIGNAL::__SIGNAL( void *owner/*, const FXString &name, FXuint msg_type*/ )
          : FXObject( )
  {
    m_owner  = owner;
    //m_name   = name;
    //m_type   = msg_type;
    //m_status = 0;
  }

  __SIGNAL::~__SIGNAL( )
  {
    disconnect( );
  }

  int __SIGNAL::emit( void *data, bool change_silence )
  {
    FXint count = 0;
    /*
    FXint num = m_slots->no( );
    for( FXint i = 0; i != num; i++ ) {
      Slot *sl = m_slots[ i ];
      if( sl != NULL ) {
        if( sl->tryHandle( this, FXSEL( SEL_SIGNAL, 0 ), data ) == 0 ) { count++; }
      }
    }
    */
    return count;

  }


  int __SIGNAL::connect( Slot *slot )
  {/*
    FXint  num   = m_slots.no( );

    for( FXint i = 0; i != num; i++ ) {
      if( m_slots[ i ] == slot ) { return SIGNAL_EXISTS; }
    }

    if( m_slots.push( slot ) ) { return SIGNAL_OK; }

    return SIGNAL_ERR;
    */
    return 0;
  }


  int __SIGNAL::disconnect( Slot *slot )
  {

    return 0;
  }

  int __SIGNAL::disconnect( )
  {

    return 0;
  }
  /*
  long __SIGNAL::on_connect( FXObject *sender, FXSelector sel, void *data )
  {


  }

  long __SIGNAL::on_update( FXObject *sender, FXSelector sel, void *data )
  {


  }

  long __SIGNAL::on_CmdLock( FXObject *sender, FXSelector sel, void *data )
  {


  }
  */
}; /* namespace ECHIDNA */

/*** END ******************************************************************************************/ 
