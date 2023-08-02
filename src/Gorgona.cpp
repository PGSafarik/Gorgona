// Gorgona.cpp Copyright (c) %date%;  D.A.Tiger; GNU GPL 3

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

  m_lua     = luaL_newstate( );  

  mx_root     = NULL;
  m_library = new Library( this );

  m_term = new TermInfo;

  addSignal( SIGCHLD, this, Gorgona::SIGNAL_CHLD, false, 0 );

  sig_child_exit = new GSignal( this, SEL_SIGNAL );
}

Gorgona::~Gorgona( )
{

}

/**************************************************************************************************/
FXbool Gorgona::hasChild( FXint pid )     
{ 
  FXbool res = false;

  if( !m_descendants.empty( ) ) { 
    if( pid == 0 ) { res = true; } // Bezi aspon jeden jakykoliv potomek
    else if( pid > 0 ) {           // Dotaz na konkretni proces 
      res = m_descendants.has( FXString::value( pid ) ); 
    }
  }
    
  return res;
}
  
PERSEUS::Process* Gorgona::findChild( FXint pid )
{
  if( hasChild( pid ) ) {
    return m_descendants[ FXString::value( pid ) ];
  }

  return NULL; 
}

FXbool Gorgona::removeChild( FXint pid, FXbool force )
{
  FXbool res = false;
  FXString key = FXString::value( pid );

  if( m_descendants.has( key ) ) {
    PERSEUS::Process *proc = m_descendants[ key ];

    if( proc != NULL ) {
      if( proc->is_running( ) ) { 
        if( force && !proc->kill( ) ) { return false; }
      }
      
      m_descendants.remove( key );
      delete proc;
      res = true;
    } 
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
  ReadConfig( );
  LuaInit( );
  LoadLibrary( );
   
  /* Docasne */
  m_term->name      = "xterm";
  m_term->exec      = "/usr/bin/xterm";
  m_term->p_run     = "-e";
  m_term->p_noclose = "+hold";
  m_term->p_workdir = FXString::null;

  m_initialized = true;
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

  PERSEUS::Process *proc = new PERSEUS::Process; 

  if( proc->run( cmd ) ) {
    pid = proc->id( );

    FXString key =  FXString::value( pid );
    m_descendants.insert( key , proc ); 

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
  else { 
    /* FIXME: Info in message box? */
    std::cout << "[WARNING]: Process " << cmd[ 0 ] << "is not running!" << std::endl; 
  }

  std::cout << "\n";
  std::cout.flush( );

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
    if( ( result = luaL_dofile( m_lua, script.text( ) ) ) != 0 ) { l_ErrorMessage( result ); }    
  }

  return result;
}

FXint Gorgona::wait( PERSEUS::Process *process, FXbool notify )
{
  FXint status = 0;

  if( process ) {
    process->wait( status );
    if( notify ) { sig_child_exit->emit( ); } 
  }
  
  return status;
}

/**************************************************************************************************/
long Gorgona::OnSig_ExitChild( FXObject *sender, FXSelector sel, void *data )
{ 
  FXint    status;
  FXString msg  = "";
  struct   rusage __usage;
  FXint    pid  = ( FXint ) wait3( &status, 0, &__usage );
   
  PERSEUS::Process *proc = findChild( pid );
  if( proc ) {
      proc->exited( status );
      sig_child_exit->emit( &pid );  
  
      msg += "Remaining number of registered processes: ";
      msg += FXString::value( m_descendants.used( ) );
  }
  else { 
    msg += "The UNKNOWN chidern process of the descendant ";
    msg += FXString::value( pid ) + ", which just finished with exit code " + FXString::value( status );
  }    
  
  std::cout << msg << std::endl;  
  return 1;
}

long Gorgona::OnCmd_Save( FXObject *sender, FXSelector sel, void *data )
{
  long res = 1;

  switch( FXSELID( sel ) ) {
    case Gorgona::SAVE_LIBRARY: 
    {
      Save_Library( );
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
    Save_Library( ); 
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
  FXString xmllist = reg( ).readStringEntry( "Profile", "Gamelist",  "gamelist" );
  
  m_gamelist = m_profiledir + "/data/" + xmllist + ".xml";
  
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

void Gorgona::LoadLibrary( )
{
/*
  if( ( m_gamelist.empty( ) != true ) && ( mx_document.LoadFile( m_gamelist.text( ) ) == XML_SUCCESS ) ) {
    std::cout << "[DEBUG - Gorgona::init] Loading Library..." << std::endl;
    if( ( mx_root = mx_document.RootElement( ) ) != NULL ) {
      m_library->load( mx_root->FirstChildElement( "Library" ) );
    }     
  }
  else {
    std::cout << "[Gorgona::init] XML read error - " << mx_document.ErrorName() << "(" << mx_document.ErrorID( ) << "): " << mx_document.ErrorStr( ) << std::endl;
    //gl_change = true;
  }
}

*/
  XMLDocument     x_document;  // XML instance of the games list
  XMLElement      *x_root;     // XML root element of the games list 

  if( ( m_gamelist.empty( ) != true ) && ( x_document.LoadFile( m_gamelist.text( ) ) == XML_SUCCESS ) ) {
    std::cout << "[DEBUG - Gorgona::Load_Library] Loading Library from xml-file..." << std::endl;
    if( ( x_root = x_document.RootElement( ) ) != NULL ) {
      m_library->load( x_root->FirstChildElement( "Library" ) );
    }     
  }
  else {
    std::cout << "[Gorgona::Load_Library] XML read error - " << x_document.ErrorName() << "(" << x_document.ErrorID( ) << "): " << x_document.ErrorStr( ) << std::endl;
    //m_change = true;
  }
}

void Gorgona::Save_Library( )
{ 
  XMLDocument x_document;  
  x_document.NewDeclaration( );

  // Specifically for TinyXML-2:
  // Set the element as the first in the document - i.e. the root element
  XMLElement *x_root = x_document.NewElement( reg( ).getAppKey( ).text( ) );
  x_document.InsertFirstChild( x_root );   

  XMLElement *x_library = x_root->InsertNewChildElement( "Library" );
  m_library->save( x_library );

  std::cout << "[Gorgona::Save_Library] Saving the menu xml-file" << std::endl;
  if( x_document.SaveFile( m_gamelist.text( ) ) != XML_SUCCESS ) { 
    std::cout << "[Gorgona::Save_Library] XML writting error - " << x_document.ErrorName() << "(" << x_document.ErrorID( ) << "): " << x_document.ErrorStr( ) << std::endl;
  }

  notify_changes( FSM_Changes::ID_DISCARD );
}

/*** END ******************************************************************************************/
