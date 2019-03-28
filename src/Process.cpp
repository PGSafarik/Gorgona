// Process.cpp Copyright (c) 08/05/2018;  D.A.Tiger; GNU GPL 3
#include<GorgonaProcess/Process.h>

using namespace GORGONA;
//#define DEBUG_DUMP_P true /// FIXME PROCESS_01 : Po skonceni ladeni odstranit priznak pro vypis ladicich informaci

/// Callbacky pro volani funkci z Lua scriptu ///
FXString lua_Launcher_p( const FXString &p_id, const FXString &p_cmd ); // Spusteni prikazu pomoci scriptovaneho spoustece


/*************************************************************************************************/
//FXDEFMAP( Process ) PROCESSMAP[ ] = { }
FXIMPLEMENT( Process, FXObject, NULL, 0 )

Process::Process( const FXString &cmd, const FXString &launcher, FXObject *tgt, FXSelector sel )
       : p_launchid( launcher ), p_status( false ), p_notify( false ), p_parse( false ), p_target( tgt ), p_selector( sel )
{
  FXString p_command = ( is_native( ) ? cmd : lua_Launcher_p( launcher, cmd ) );
  ParseCommand( p_command );
}

Process::~Process( )
{
  clear_string_buffer( &p_params );
}


void Process::ParseCommand( const FXString &cmd )
{
  // Parametry se oddeluji znakem mezery (" ")
  // Je-li to vyzdovano aplikaci musi byt dodreno i jejich poradi
  // mezera za, nebo pred strednikem se predava jako soucast parametru
  // Specialni znaky ( napr uvozovky) je nutno uvadet v zastupne notaci dle XML standardu

  FXint    start, nargs;  // Aktualni sekce, ktera predstavuje argument; Pocet parametru v retezci
  FXString section_str;   // Subsekce retezce

  clear_string_buffer( &p_params );
  if( !cmd.empty( ) && ( cmd != "" ) ) {
    nargs = cmd.contains( " " );
    if( nargs > 0 ) {
      nargs++;
      start = 0;
      while( start < nargs ){
        section_str = cmd.section( " ", start );
        if( !section_str.empty( ) ) {
          p_params.append( convert_str( section_str ) );
          start++;
        } else { break; }
      }
    }
    else {
      start = nargs = 1;
      p_params.append( convert_str( cmd ) );
    }
  }

  if( nargs == start ) {
    p_params.append( NULL );
    p_parse = true;
  }
  else {
    p_parse = false;
    clear_string_buffer( &p_params );
  }

  #ifdef DEBUG_DUMP_P
  // Vypis obsahu pole prikazu po parsovani
  FXint num = p_params.no( );
  std::cout << "[Perseus."<< getClassName( ) << "::ParseCommand( )] The params array listing of " << p_params.at( 0 )  << " command p_process: " << std::endl;
  for( FXint i = 0; i != num; i++  ) { std::cout << "\n" << i << ". " << p_params.at( i ) << " " ; }
  std::cout << "\n" << std::endl;
  std::cout.flush( );
  #endif
}

FXbool Process::start( const FXString &workdir, FXbool waiting )
{
  FXString chwd;

  std::cout << "Perseus starting the command " << p_command.text( ) << std::endl;
  chwd = changeWorkDir( workdir );
  if( p_parse == true ) {
    if( ( p_status = p_process.start( p_params[ 0 ], p_params.data( ) ) ) == true ) {
      waitForMy( waiting );
      counter( );
    }
    else { std::cout << "RUN FATAL ERROR: Command " << p_params[ 0 ] << " is not running!" << std::endl; }
  }
  if( !chwd.empty( ) ) { FXSystem::setCurrentDirectory( chwd ); }

  #ifdef DEBUG_DUMP_P
  std::cout << "[Perseus." << getClassName( ) << "::start( )] Debug process info for this command:" << std::endl;
  std::cout << "Launcher:  " << ( p_launchid.empty( ) ? "native" : p_launchid.text( ) ) << std::endl;
  std::cout << "Work dir:  " << ( workdir.empty( ) ? "native" : workdir.text( ) ) << std::endl;
  std::cout << "Executing: " << ( p_status ? "true" : "false" ) << std::endl;
  #endif // DEBUG_DUMP_P

  std::cout << "\n========================================================" << std::endl;
  std::cout.flush( );

  return p_status;
}

void Process::counter( )
{
  // Prozatim nic
}

FXint Process::waitForMy( FXbool value )
{
  FXint stat = 0;
  #ifdef DEBUG_DUMP_P
  std::cout << "[Perseus." << getClassName( ) << "::waitForMy( )] Waiting on process " << p_params[ 0 ] << std::endl;
  std::cout << "Type: " << ( value ? "enable" : "disable" ) << std::endl;
  #endif // DEBUG_DUMP_P

  if( value == true ) { this->p_process.wait( stat ); }

  #ifdef DEBUG_DUMP_P
  std::cout << "Status : " <<  stat  << std::endl;
  #endif // DEBUG_DUMP_P


  return stat;
}

FXString Process::changeWorkDir( const FXString &nwd )
{
  FXString orig = FXSystem::getCurrentDirectory( );

  if( nwd != "" && nwd != orig ) {
    FXSystem::setCurrentDirectory( nwd );
    return orig;
  }

  return FXString::null;
}


/*************************************************************************************************/
//FXDEFMAP( Process ) PROCESSMAP[ ] = { }
FXIMPLEMENT( GameProcess, Process, NULL, 0 )

GameProcess::GameProcess( FXGameItem *game, FXObject *tgt, FXSelector sel )
           : Process( ( *game )( "Basic:exec" ), ( *game )( "Basic:type" ), tgt, sel ), gp_item( game )
{ }

GameProcess::~GameProcess( )
{

}

FXbool GameProcess::start( )
{
  std::cout << "\n=== " << ( *gp_item )( "Basic:title" ).text( ) << " ==============================================" << std::endl;
  return Process::start( ( *gp_item )( "Basic:workdir" ) );
}

void GameProcess::counter( )
{
  gp_item->nop += 1;

  FXDate date = FXDate::localDate( );
  ( *gp_item )( "Extra:lastPlayed", FXString::value( date.day( ) ) + "/" + FXString::value( date.month( ) ) + "/" + FXString::value( date.year( ) ) );
}

/*************************************************************************************************/
FXString lua_Launcher_p( const FXString &p_id, const FXString &p_cmd )
{
/**
   Globalni Callback volani launcheru v Lua modulu
   p_id - typ launcheru (Dosbox, Wine, ... atd. Volani v lue je tedy napr.: Launcher( "Dosbox", cmd )
   p_cmd   - retezec prikazu ze spoutece
 **/
  FXString resh = FXString::null;

  lua_getglobal(  l_parser( ), "launcher"  );
  lua_pushstring( l_parser( ), convert_str( p_id ) );
  lua_pushstring( l_parser( ), convert_str( p_cmd ) );

  if( lua_pcall( l_parser( ), 2, 1, 0 ) != 0 ) { /// FIXME : FUNKCE PREJIMA DVA ARGUMENTY A VRACI JEDNU HODNOTU!
    std::cout << "Chyba spusteni callbacku \'run\' - " << lua_tostring( l_parser( ), -1 ) << std::endl;
  }
  else {
    resh = lua_tostring( l_parser( ), -1 );
    lua_pop( l_parser( ), -1 );
  }

  #ifdef DEBUG_DUMP_P
  std::cout << "__launcher( )" << std::endl;
  std::cout << "Spoustec \'" << p_id.text( ) << "\' Prikaz \'" << p_cmd.text( ) << "\'" << std::endl;
  std::cout << "Vysledek: " << resh.text( ) << std::endl;
  #endif

 return resh;
}

/*** END ******************************************************************************************/
