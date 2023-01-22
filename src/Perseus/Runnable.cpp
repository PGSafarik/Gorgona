// Runnable.cpp Copyright (c) 08/05/2018;  D.A.Tiger; GNU GPL 3

#include<Gorgona.h>
#include<Utils.h>
//#include<FXGameItem.h>
#include<LuaAPI.h>

using namespace PERSEUS;

/// Callbacky pro volani funkci z Lua scriptu ///
FXString lua_Launcher_p( const FXString &p_id, const FXString &p_cmd ); // Spusteni prikazu pomoci scriptovaneho spoustece


/*************************************************************************************************/
//FXDEFMAP( Runnable ) RunnableMAP[ ] = { }
FXIMPLEMENT( Runnable, FXObject, NULL, 0 )

Runnable::Runnable( Gorgona *a, FXObject *tgt, FXSelector sel )
{
  m_app     = a;
  m_target  = tgt;
  m_message = sel;

  m_notify = false;

}

Runnable::Runnable( Gorgona *a, const FXString &cmd, const FXString &launcher, FXObject *tgt, FXSelector sel )
       : m_launchid( launcher ), m_notify( false ), m_target( tgt ), m_message( sel )
{
  m_app = a;
  set_command( cmd );
}

Runnable::~Runnable( )
{ }

FXint Runnable::run( )
{
  FXint pid = 0; 
  FXString chwd = ChangeWorkDir( );
  

  if( ( pid = m_app->exec( m_execute, 0, 0, 0 ) ) <= 0 ) { 
    std::cout << "[ERROR Runnable]: Command " << m_command << " is not running!" << std::endl; 
  }
  
  if( !chwd.empty( ) ) { FXSystem::setCurrentDirectory( chwd ); }
  return pid;
}

void Runnable::Command( const FXString &cmd )
{
  m_command = cmd;

  if( !m_command.empty( ) && !IsNative( ) ) {
    m_execute = lua_Launcher_p( m_launchid, m_command );
  }
  else { m_execute = m_command; } 
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

FXbool Runnable::load( TiXmlElement *parent )
{
  FXbool resh = false; 

  if( parent ) {
    TiXmlElement *re = parent->FirstChildElement( "Runnable" );

    if( re ) {
      m_launchid = re->Attribute( "type" );
      m_workdir = re->Attribute( "workdir" );
      //m_bckg  = re->Attribute( "background" );

      set_command( re->Attribute( "exec" ) );
      resh = true;
    }   
  }

  return resh;
}
 
FXbool Runnable::save( TiXmlElement *parent )
{
  FXbool resh = false; 

  if( parent ) {
    TiXmlElement *re = parent->FirstChildElement( "Runnable" );

    if( re == NULL ) { 
      re = new TiXmlElement( "Runnable" );
      parent->LinkEndChild( re );
    }

    re->SetAttribute( "exec",       m_command.text( ) );
    re->SetAttribute( "type",       m_launchid.text( ) );
    re->SetAttribute( "workdir",    m_workdir.text( ) );
    //re->SetAttribute( "background", m_backg.text( ) );

    resh = true;
  }

  return resh;
}

FXbool Runnable::validation( ) 
{
  /* Im working on this ... */
  return true;
}


void Runnable::dump( ) 
{
  FXString text = "   Runnable = { \n";
  text += "     type: "       + m_launchid + "\n";   
  text += "     command: "    + m_command  + "\n";
  text += "     workdir: "    + m_workdir  + "\n";
  text += "     executable: " + m_execute  + "\n";
  text += "   } \n";

  std::cout << text;  
}

/*************************************************************************************************/
//FXDEFMAP( Game ) GameMAP[ ] = { }
FXIMPLEMENT( Game, Runnable, NULL, 0 )

Game::Game( Gorgona *a, FXObject *tgt, FXSelector sel )
           : Runnable( a, tgt, sel )
{ }

Game::~Game( )
{

}

FXint Game::run( )
{
  //std::cout << "\n=== " << ( *gp_item )( "Basic:title" ) << " ==============================================" << std::endl;
  //set_workdir( ( *gp_item )( "Basic:workdir" ) );
  FXint pid = Runnable::run( );
  counter( );

  return pid;
}

void Game::counter( )
{
  /*
  /// FIXME GAME_001: Pridat vypocet nejdelsi doby v kuse a celkove doby hrani

  gp_item->nop += 1;

  FXDate date = FXDate::localDate( );
  ( *gp_item )( "Extra:lastPlayed", FXString::value( date.day( ) ) + "/" + FXString::value( date.month( ) ) + "/" + FXString::value( date.year( ) ) );
  */
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