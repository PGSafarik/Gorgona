// Runnable.cpp Copyright (c) 08/05/2018;  D.A.Tiger; GNU GPL 3
#include<GorgonaProcess/Runnable.h>

using namespace PERSEUS;

/// Callbacky pro volani funkci z Lua scriptu ///
FXString lua_Launcher_p( const FXString &p_id, const FXString &p_cmd ); // Spusteni prikazu pomoci scriptovaneho spoustece


/*************************************************************************************************/
//FXDEFMAP( Runnable ) RunnableMAP[ ] = { }
FXIMPLEMENT( Runnable, FXObject, NULL, 0 )

Runnable::Runnable( Gorgona *a, const FXString &cmd, const FXString &launcher, FXObject *tgt, FXSelector sel )
       : p_launchid( launcher ), p_notify( false ), p_target( tgt ), p_selector( sel )
{
  m_app = a;
  p_command = ( is_native( ) ? cmd : lua_Launcher_p( launcher, cmd ) );
}

Runnable::~Runnable( )
{ }

FXint Runnable::run( )
{
  FXint pid = 0; 
  FXString chwd = ChangeWorkDir( );
  
  if( ( pid = m_app->exec( p_command, 0, 0, 0 ) ) == 0 ) { 
    std::cout << "[ERROR Runnable]: Command " << p_command << " is not running!" << std::endl; 
  }
  
  if( !chwd.empty( ) ) { FXSystem::setCurrentDirectory( chwd ); }
  return pid;
}

FXString Runnable::ChangeWorkDir( )
{
  FXString orig = FXSystem::getCurrentDirectory( );

  if( !m_workdir.empty( ) && m_workdir != orig ) {
    FXSystem::setCurrentDirectory( m_workdir );
    return orig;
  }

  return FXString::null;
}


/*************************************************************************************************/
//FXDEFMAP( Runnable ) RunnableMAP[ ] = { }
FXIMPLEMENT( Game, Runnable, NULL, 0 )

Game::Game( Gorgona *a, FXGameItem *game, FXObject *tgt, FXSelector sel )
           : Runnable( a, ( *game )( "Basic:exec" ), ( *game )( "Basic:type" ), tgt, sel ), gp_item( game )
{ }

Game::~Game( )
{

}

FXint Game::run( )
{
  std::cout << "\n=== " << ( *gp_item )( "Basic:title" ) << " ==============================================" << std::endl;
  set_workdir( ( *gp_item )( "Basic:workdir" ) );
  FXint pid = Runnable::run( );
  counter( );

  return pid;
}

void Game::counter( )
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

  #ifdef __DEBUG
  std::cout << "[ DEBUG __launcher( ) ] Module: \'" << p_id << "\' Arg: \'" << p_cmd << "\'" << std::endl;
  #endif
  
 return resh;
}

/*** END ******************************************************************************************/
