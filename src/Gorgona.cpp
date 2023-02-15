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
  m_tgt         = NULL;
  m_message     = 0;
  m_verbose     = true;
  m_initialized = false;
  m_created     = false;

  m_lua     = luaL_newstate( );
  
  m_initscript = m_profiledir = m_gamelist = FXString::null;

  mx_document = NULL;
  mx_root     = NULL;

  m_library = NULL;

  addSignal( SIGCHLD, this, Gorgona::SIGNAL_CHLD, false, 0 );
}

Gorgona::~Gorgona( )
{

}

/**************************************************************************************************/
FXbool Gorgona::hasChild( FXint pid )     
{ 
  if( !m_descendants.empty( ) && pid > 0 ) { 
    return m_descendants.has( FXString::value( pid ) ); 
  }
    
  return false;
}
  
FXProcess* Gorgona::findChild( FXint pid )
{
  if( hasChild( pid ) ) {
    return m_descendants[ FXString::value( pid ) ];
  }

  return NULL; 
}


/**************************************************************************************************/
void Gorgona::create( )
{
  FXApp::create( );
  m_created = true;
}

void Gorgona::init( int& argc, char** argv, FXbool connect )
{
  Welcome( this ); 
 
  FXApp::init( argc, argv, connect );
  ReadConfig( );
  LuaInit( );

  m_initialized = true;
}

FXint Gorgona::exec( const FXArray<const FXchar*> &cmd, FXuint proc_opts, FXuint term_opts, FXuint sudo_opts )
{
  /* Metoda prevezme prikaz a spusti jej jako novy proces, reprezentovany objektem typu FXProcess, ktery je nasledne 
     vlozen do bufferu procesu. V pripade uspechu vrati PID spusteneho procesu, v pripade neuspechu vrati chybovy kod
   
     - Cesta k spoustenemu souboru (args[0]) nesmi zacinat a koncit mezerou (" ")
     - Cesta ke spoustenemu souboru musi byt absolutni (tedy od korenoveho adresare)
     - Navratovy kod spousteneho procesu lze zatim ziskat pouze pri wait = true
     - Aktivace wait zablokuje celou aplikaci launcheru!
     FIXME GORGONA_003: Dopracovat spousteci priznaky
  */
  FXint pid = 0;

  FXProcess *proc = new FXProcess; 
  if( proc->start( cmd[ 0 ], cmd.data( ) ) ) {
    pid = proc->id( );

    FXString key =  FXString::value( pid );
    m_descendants.insert( key , proc ); 
    Notify( true, SEL_CHANGED );

    if( m_verbose ) {
      std::cout << "EXECUTE the process:";
      FXString text = "";
      FXint num = cmd.no( );
      for( FXint i = 0; i != num; i++ ) { 
        text += " "; 
        text += cmd[ i ]; 
      }
      text += "\n";
      text += "PID: " + key; 
      std::cout << text << std::endl;;
    }

  }
  else { std::cout << "[WARNING]: Process " << cmd[ 0 ] << "is not running!" << std::endl; }


  std::cout << "\n";
  std::cout.flush( );

  return pid;
}

FXint Gorgona::exec( const FXString &cmd, FXuint proc_opts, FXuint term_opts, FXuint sudo_opts )
{
  FXArray<const char*> buffer;

  if( !cmd.empty( ) ) {
    ParseCommand( cmd, &buffer );
    return exec( buffer, proc_opts, term_opts, sudo_opts );
  }  
  
  return -1;
}

FXint Gorgona::execLuaFile( const FXString &script )
{
  FXint result = -1;

  if( m_lua ) {
    if( ( result = luaL_dofile( m_lua, script.text( ) ) ) != 0 ) { l_ErrorMessage( result ); }    
  }

  return result;
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

long Gorgona::notify( FXuint mtype, void *mdata ) 
{
   long retv = -1;
   if( m_tgt && m_message > 0 ) { retv = m_tgt->handle( this, FXSEL( mtype, m_message ), mdata ); } 
   return retv;
}

/**************************************************************************************************/
long Gorgona::OnSig_ExitChild( FXObject *sender, FXSelector sel, void *data )
{ 
  FXint  status;
  struct rusage __usage;
  
  FXint pid = ( FXint ) wait3( &status, 0, &__usage );
  
  FXString key = FXString::value( pid );
  if( m_descendants.has( key ) ) {
    FXProcess *proc = m_descendants[ key ];
    if( proc != NULL ) {
      m_descendants.remove( key );
      delete proc;
      std::cout << "Unregister the process of the descendant " << pid << ", which just finished with exit code " << status << std::endl;
    }
    std::cout << "Remaining number of registered processes: " << m_descendants.used( ) << std::endl;
  }
  else { std::cout << "The process of the descendant " << pid << ", which just finished with exit code " << status << std::endl; }    
  
  
  return 1;
}

/**************************************************************************************************/
long Gorgona::Notify( FXbool enable, FXuint mtype, void *mdata )
{
  return ( enable ? notify( mtype, mdata ) : -1 );
}

void Gorgona::ParseCommand( const FXString &cmd, FXArray<const char*> *buffer )
{
  /* Metoda provadi rozdeleni retezce prikazu do pole typu const char*, jak to pozaduje trida FXProcess
     buffer[ 0 ] je samotny prikaz urceny ke spusteni, kazda dalsi polozka  predstavuje jeden argument.
 
    - Parametry se oddeluji znakem mezery (" "), nebo env IFS 
    - Musi byt dodreno i poradi jednotlivych casti prikazu, jak bylo zadano
    - mezera za, nebo pred strednikem se predava jako soucast parametru
    - Specialni znaky ( napr uvozovky) je nutno uvadet v zastupne notaci dle XML standardu
    FIXME GORGONA_001 : Doplnit funkcionalitu znaku ', " a \
    FIXME GORGONA_002 : dodat cteni ENV IFS, alternativne zadat separator argumentem metody 
  */
  FXint    start, nargs;  // Aktualni sekce, ktera predstavuje argument; Pocet parametru v retezci
  FXString section_str;   // Subsekce retezce
 

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
  }
  else {
    clear_string_buffer( buffer );
  }
}

void Gorgona::ReadConfig( )
{
  m_initscript = reg( ).readStringEntry( "Modules", "launchers", "/usr/share/Gorgona/modules/Launchers.lua" );
  m_profiledir = reg( ).readStringEntry( "Profile", "Directory", ( FXSystem::getHomeDirectory( ) + "/.config/Gorgona" ).text( ) );
  m_gamelist   = reg( ).readStringEntry( "Profile", "Gamelist",  "gamelist" );
  
}

void Gorgona::LuaInit( )
{
  if( l_open( this ) ) { 
    FXint result = execLuaFile( m_initscript );
    m_luaOk = ( result == 0 ); 
    std::cout << "[INFO Gorgona]: Lua initialized: " << m_initscript  << " => " << result << std::endl; 
  }
  else { std::cout << "[ERROR Gorgona]: Lua its NOT opened!"; }
}

/*** END ******************************************************************************************/
