// Gorgona.cpp Copyright (c) %date%;  D.A.Tiger; GNU GPL 3

#include<Gorgona.h>

FXDEFMAP( Gorgona ) GORGONAMAP[ ] = { };
FXIMPLEMENT( Gorgona, FXApp, GORGONAMAP, ARRAYNUMBER( GORGONAMAP ) )

/**************************************************************************************************/
Gorgona::Gorgona( const FXString& name, const FXString& vendor )
       : FXApp( name, vendor )
{

}

Gorgona::~Gorgona( )
{

}

/**************************************************************************************************/
void Gorgona::create( )
{
  FXApp::create( );

}

void Gorgona::init( int& argc, char** argv, FXbool connect )
{

  FXApp::init( argc, argv, connect );
}

FXint Gorgona::exec( const FXArray<const FXchar*> &cmd, FXuint term_opts, FXuint sudo_opts, FXuint proc_opts )
{
  FXint resh = 0;


  return resh;
}

FXint Gorgona::wait( FXProcess *process, FXbool notify )
{
  FXint status = 0;

  if( process ) {
    process->wait( status );
    Notify( notify );   
  }
  
  return status;
}

/**************************************************************************************************/
long Gorgona::Notify( FXbool send, FXuint mtype, void *mdata )
{
  long status = 0;

  if( send && m_tgt ) { status = m_tgt->handle( this, FXSEL( mtype, m_message ), mdata ); }

  return status;
}






/*** END ******************************************************************************************/
