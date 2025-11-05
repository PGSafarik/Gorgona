// Gorgona.cpp; Copyright (c) 2025 D.A.Tiger; GNU GPL 3
#include<Library.h>
#include<Perseus/Desktop.h>
#include<Perseus/TermProfile.h>
#include<Perseus/Process.h>
#include<LuaAPI.h>
#include<Gorgona.h>

FXDEFMAP( Gorgona ) GORGONAMAP[ ] = { 
  FXMAPFUNC( SEL_SIGNAL,  Gorgona::SIGNAL_CHLD,    Gorgona::OnSig_ExitChild ),
  FXMAPFUNC( SEL_COMMAND, Gorgona::SAVE_LIBRARY,   Gorgona::OnCmd_Save ),
  FXMAPFUNC( SEL_COMMAND, Gorgona::SAVE_CONFIGURE, Gorgona::OnCmd_Save ),
  FXMAPFUNC( SEL_COMMAND, FXApp::ID_QUIT,          Gorgona::onCmdQuit )
};

FXIMPLEMENT( Gorgona, FXApp, GORGONAMAP, ARRAYNUMBER( GORGONAMAP ) )

/**************************************************************************************************/
Gorgona::Gorgona( const FXString& name, const FXString& vendor )
       : FXApp( name, vendor )
{
  m_initscript = m_profiledir = m_gamelist = FXString::null;

  m_verbose     = true;
  m_initialized = false;
  m_created     = false;

  m_lua = luaL_newstate( );  

  mx_root   = NULL;
  m_library = std::unique_ptr<Library>( new Library( this ) );
  m_term    = std::unique_ptr<PERSEUS::TermProfile>( new PERSEUS::TermProfile );

  addSignal( SIGCHLD, this, Gorgona::SIGNAL_CHLD, false, 0 );
  sig_child_exit = new GSignal( this, SEL_SIGNAL );
}

Gorgona::~Gorgona( )
{
  m_library->close( );
  delete sig_child_exit;
}

/**************************************************************************************************/
FXbool Gorgona::hasChild( FXint pid )     
{ 
  FXbool res = false;


  if( !m_session.empty( ) ) {
    if( pid == 0 ) { res = true; } // Is there even one descendant of the Gorgon?
    else if( pid > 0 ) {           // Does a Gorgon descendant run with this particular PID?
      res = m_session.has( pid );
    }
  }
    
  return res;
}
  
PERSEUS::Process* Gorgona::findChild( FXint pid )
{
  auto it = m_session.find( pid );
  if( it != m_session.end( ) ) { return it->second; }
  return NULL;
}

FXbool Gorgona::removeChild( FXint pid, FXbool force )
{
  FXbool res = false;

  auto it = m_session.find( pid );
  if( it != m_session.end( ) ) {
    PERSEUS::Process *proc = it->second;
    if( proc->is_running( ) ) {
      if( force && !proc->kill( ) ) { return false; }
    }

    m_session.erase( it );
    delete proc;
    res = true;
  }

  return res;
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

  FXStringList buff;

  m_localdatadir = PERSEUS::HomeDataDirectory( ) + PATHSEPSTRING + getAppName( );
  m_localconfdir = PERSEUS::HomeConfigDirectory( ) + PATHSEPSTRING + getVendorName( ) + PATHSEPSTRING + getAppName( );

  PERSEUS::SystemConfigDirectories( buff );
  m_configdir = FindSubDirectory( buff, getAppName( ) );
  buff.clear( );

  PERSEUS::SystemDataDirectories( buff );
  m_datadir = FindSubDirectory( buff, getAppName( ) );
  buff.clear( );

  m_session.start( getUtilsDir( false ) + PATHSEPSTRING + GROUP_CHECKER );
  ReadConfig( );
  LuaInit( );

  m_library->open( m_gamelist );
  m_library->load( );

  /* terminal emulator - Docasne */
  m_term->name      = "xterm";
  m_term->exec      = "/usr/bin/xterm";
  m_term->p_run     = "-e";
  m_term->p_noclose = "+hold";
  m_term->p_workdir = FXString::null;

  m_session.check( );


  m_initialized = true;

  DEBUG_OUT( reg( ).getAppKey( ) << " [" << reg( ).getVendorKey( ) << "]" )
  DEBUG_OUT( "User home directory:"           << FXSystem::getHomeDirectory( ) )
  DEBUG_OUT( "System configure directory:: "  << getConfDir( false ) )
  DEBUG_OUT( "User configure directory: "     << getConfDir( ) )
  DEBUG_OUT( "System data directory: "        << getDataDir( false ) )
  DEBUG_OUT( "User data directory: "          << getDataDir( ) )
  DEBUG_OUT( "System Scripts directory: "     << getScriptDir( false ) )
  DEBUG_OUT( "User Scripts directory: "       << getScriptDir( ) )
  DEBUG_OUT( "System Utils directory: "       << getUtilsDir( false ) )
  DEBUG_OUT( "User Utilss directory: "        << getUtilsDir( ) )
  DEBUG_OUT( "System lua libs directory: "    << getLualibsDir( false ) )
  DEBUG_OUT( "User lua libs directory: "      << getLualibsDir( ) )
  DEBUG_OUT( "System Lua modules directory: " << getLuamodsDir( false ) )
  DEBUG_OUT( "User Lua modules directory: "   << getLuamodsDir( ) << "\n" )
}

FXint Gorgona::exec( const FXArray<const FXchar*> &cmd, FXuint proc_opts )
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
  
  DEBUG_OUT( "Create new process" )
  PERSEUS::Process *proc = new PERSEUS::Process; 

  DEBUG_OUT( "Run new process" )
  FXint jid = m_session.set_job( JOB_NORMAL );
  if( proc->run( cmd ) ) {
    pid = proc->id( );
    m_session.insert( std::pair<FXint, PERSEUS::Process* >( pid, proc ) );
    DEBUG_OUT( "New process OK. PID: " << pid << " Job ID: " << jid );

    if( m_verbose ) {
      std::cout << "EXECUTE the process:";
      FXString text = "";
      FXint num = cmd.no( );
      for( FXint i = 0; i != num; i++ ) { 
        text += " "; 
        text += cmd[ i ]; 
      }
      text += "\n";
      text += "PID: " + FXString::value( pid );
      std::cout << text << std::endl;;
    }
  }
  else { 
    /* FIXME: Info in message box? */
    std::cout << "[WARNING]: Process " << cmd[ 0 ] << " is not running!" << std::endl; 
  }

  std::cout << "\n";
  std::cout.flush( );

  DEBUG_OUT( "Update internal list of the  process session" )
  m_session.set_job( JOB_SYSTEM );
  m_session.check( );

  return pid;
}

FXint Gorgona::exec( const FXString &cmd, FXuint proc_opts )
{
  FXArray<const char*> buffer;

  if( !cmd.empty( ) ) {
    ParseCommand( cmd, &buffer );
    return exec( buffer, proc_opts );
  }  
  
  return -1;
}

FXint Gorgona::execLuaFile( const FXString &script )
{
  FXint result = -1;

  if( m_lua ) {
    if( ( result = luaL_dofile( m_lua, script.text( ) ) ) != 0 ) { l_ErrorMessage( result, FXString::null, false ); }    
  }

  return result;
}

FXbool Gorgona::hasTerminal( )  { return ( !m_term->exec.empty( ) ); }

/**************************************************************************************************/
long Gorgona::OnSig_ExitChild( FXObject *sender, FXSelector sel, void *data )
{
  DEBUG_OUT(  "SIGCHLD - START" )

  int    status;
  FXint  pid = 0;
  FXString msg  = "";
  while( ( pid = waitpid( -1, &status, WNOHANG ) ) > 0 ) {
    std::cout << "PID :" << pid << std::endl;
    PERSEUS::Process *proc = findChild( pid );
    if( proc ) {
      proc->exited( status );

      /*
      Potentially there is a risk of conflict when two or more objects are operated at the same
      signal! Object of type PERSEUS::Runnable is removed from the memory during signal handling!
      And that regardless of whether only the PID is sent, or a direct pointer to
      PERSEUS::Process. It is necessary to test well!!!
      */
      sig_child_exit->emit( proc );

      msg += "Remaining number of registered processes: " + FXString::value( m_session.size( ) );
    }
    else {
      msg += "The UNKNOWN chidern process of the descendant " + FXString::value( pid ) + ", ";
      if( WIFEXITED( status ) ) {
        msg +=  "which just finished with exit code " + FXString::value( WEXITSTATUS( status ) );
      } else if( WIFSIGNALED( status ) ) {
        msg += strsignal( WTERMSIG( status ) );
      }
    }
    std::cout << msg << std::endl;
  }
  
  DEBUG_OUT(  "SIGCHLD - FINISH" )
  std::cout.flush( );
  return 1;
}

long Gorgona::OnCmd_Save( FXObject *sender, FXSelector sel, void *data )
{
  long res = 1;

  switch( FXSELID( sel ) ) {
    case Gorgona::SAVE_LIBRARY: 
    {
      m_library->save( );
      break;  
    }
    case Gorgona::SAVE_CONFIGURE: 
    {
      //Write_Config( );
      break;
    }
  } 

  return res;
}

long Gorgona::onCmdQuit( FXObject *sender, FXSelector sel, void *data )
{
  if( m_modify( ) ) {
    m_library->save( ); 
    if( m_modify( ) ) { std::cout << "Gorgona: NEULOZENE ZMENY" << std::endl; }
 }
    
  std::cout << "Gorgona: === BYE! ======================" << std::endl;
  return FXApp::onCmdQuit( sender, sel, data );
}

/**************************************************************************************************/
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
 
  if( cmd.empty( ) ) { return; }

  clear_string_buffer( buffer );
  //if( !cmd.empty( ) && ( cmd != "" ) ) {
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
  //}

  if( nargs == start ) {
    buffer->append( NULL );
  }
  else {
    clear_string_buffer( buffer );
  }
}

void Gorgona::ReadConfig( )
{
  m_initscript = reg( ).readStringEntry( "Modules", "launchers", ( getDataDir( false ) + "/scripts/Launchers.lua" ).text( ) );
  m_profiledir = reg( ).readStringEntry( "Profile", "Directory", getConfDir( ).text( ) );
  FXString xmllist = reg( ).readStringEntry( "Profile", "Gamelist",  "gamelist" );
  
  m_gamelist = m_profiledir + "/data/" + xmllist + ".xml";
  
}

void Gorgona::LuaInit( )
{
  FXString msg = "[INFO Gorgona]: Lua initialize ";
  FXString init_script = getUtilsDir( false ) + PATHSEPSTRING + MODULES_MANAGER;
  DEBUG_OUT(  "Lua init script:"  << init_script )

  if( l_open( this ) && ( execLuaFile( init_script ) == 0 ) ) { msg += "OK"; }
  else { msg += " FAILED"; }

  std::cout << msg << std::endl; 
}

FXString Gorgona::FindSubDirectory( const FXStringList &buff, const FXString &dir_name )
{
  FXString dir;

  FXuint num = buff.no( );
  for( FXuint i = 0; i != num; i++ ) {
    dir = buff[ i ] + PATHSEPSTRING + dir_name;
    DEBUG_OUT( dir.text( ) )
    if( FXStat::exists( dir ) ) { break; } else { dir.clear( ); }
  }

  return dir;
}

/*** END ******************************************************************************************/
