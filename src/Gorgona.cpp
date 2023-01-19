// Gorgona.cpp Copyright (c) %date%;  D.A.Tiger; GNU GPL 3

#include<Gorgona.h>

FXDEFMAP( Gorgona ) GORGONAMAP[ ] = { 
  FXMAPFUNC( SEL_SIGNAL, Gorgona::SIGNAL_CHLD, Gorgona::OnSig_ExitChild )
};

FXIMPLEMENT( Gorgona, FXApp, GORGONAMAP, ARRAYNUMBER( GORGONAMAP ) )

/**************************************************************************************************/
Gorgona::Gorgona( const FXString& name, const FXString& vendor )
       : FXApp( name, vendor )
{
  m_tgt = NULL;
  m_message = 0;

  m_verbose = true;

  addSignal( SIGCHLD, this, Gorgona::SIGNAL_CHLD, false, 0 );
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
  /* Metoda prevezme prikaz a spusti jej jako novy proces, reprezentovany objektem typu FXProcess, ktery je nasledne 
     vlozen do bufferu procesu. V pripade uspechu vrati PID spusteneho procesu, v pripade neuspechu vrati chybovy kod
   
     - Cesta k spoustenemu souboru (args[0]) nesmi zacinat a koncit mezerou (" ")
     - Cesta ke spoustenemu souboru musi byt absolutni (tedy od korenoveho adresare)
     - Navratovy kod spousteneho procesu lze zatim ziskat pouze pri wait = true
     - Aktivace wait zablokuje celou aplikaci launcheru!
  */
  FXint pid = 0;

  if( m_verbose ) {
    std::cout << "Run the process:";
    FXString text = "";
    FXint num = cmd.no( );
    for( FXint i = 0; i != num; i++ ) { 
       text += " "; 
       text += cmd[ i ]; 
    }
    std::cout << text << std::endl;;
  }
  
  FXProcess *proc = new FXProcess; 
  if( proc->start( cmd[ 0 ], cmd.data( ) ) ) {
    pid = proc->id( );
    if( m_verbose ) { std::cout << "PID: " << pid <<  std::endl; }

    m_descendants.push( proc ); 
    Notify( true, SEL_CHANGED );
  }
  else { std::cout << "[WARNING]: Process " << cmd[ 0 ] << "is not running!" << std::endl; }

  std::cout << "\n";
  std::cout.flush( );

  return pid;
}

FXint Gorgona::exec( const FXString &cmd, FXuint term_opts, FXuint sudo_opts, FXuint proc_opts )
{
  FXArray<const char*> buffer;

  if( !cmd.empty( ) ) {
    ParseCommand( cmd, &buffer );
    return exec( buffer, term_opts, sudo_opts, proc_opts );
  }  
  
  return -1;
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
long Gorgona::OnSig_ExitChild( FXObject *sender, FXSelector sel, void *data )
{ 
  FXint  status;
  struct rusage __usage;
  
  FXint pid = ( FXint ) wait3( &status, 0, &__usage );
  std::cout << "Child process " << pid << " with the exit code " << status << std::endl;

  FXint num = m_descendants.no( );
  FXProcess *proc = NULL;
  for( FXint i = 0; i != num; i++ ) {
    proc = m_descendants[ i ];
    if( proc && proc->id( ) == pid ) {
      m_descendants.erase( i );
      delete proc;
      std::cout << "This process removed from decedants list." << std::endl;
    }
  } 
  
  return 1;
}

/**************************************************************************************************/
long Gorgona::Notify( FXbool send, FXuint mtype, void *mdata )
{
  long status = 0;

  if( send && m_tgt ) { status = m_tgt->handle( this, FXSEL( mtype, m_message ), mdata ); }

  return status;
}

void Gorgona::ParseCommand( const FXString &cmd, FXArray<const char*> *buffer )
{
  // Parametry se oddeluji znakem mezery (" ")
  // Je-li to vyzdovano aplikaci musi byt dodreno i jejich poradi
  // mezera za, nebo pred strednikem se predava jako soucast parametru
  // Specialni znaky ( napr uvozovky) je nutno uvadet v zastupne notaci dle XML standardu

  FXint    start, nargs;  // Aktualni sekce, ktera predstavuje argument; Pocet parametru v retezci
  FXString section_str;   // Subsekce retezce
  //FXbool   p_parse = false;

  clear_string_buffer( buffer );
  if( !cmd.empty( ) && ( cmd != "" ) ) {
    nargs = cmd.contains( " " );
    if( nargs > 0 ) {
      nargs++;
      start = 0;
      while( start < nargs ){
        section_str = cmd.section( " ", start );
        if( !section_str.empty( ) ) {
          buffer->append( convert_str( section_str ) );
          start++;
        } else { break; }
      }
    }
    else {
      start = nargs = 1;
      buffer->append( convert_str( cmd ) );
    }
  }

  if( nargs == start ) {
    buffer->append( NULL );
    //p_parse = true;
  }
  else {
    //p_parse = false;
    clear_string_buffer( buffer );
  }
  /*
  #ifdef __DEBUG
  // Vypis obsahu pole prikazu po parsovani
  FXint num = buffer->no( );
  std::cout << "[Perseus."<< getClassName( ) << "::ParseCommand( )] The params array listing of " << p_params.at( 0 )  << " command p_process: " << std::endl;
  for( FXint i = 0; i != num; i++  ) { std::cout << "\n" << i << ". " << p_params.at( i ) << " " ; }
  std::cout << "\n" << std::endl;
  std::cout.flush( );
  #endif
  */
}




/*** END ******************************************************************************************/
