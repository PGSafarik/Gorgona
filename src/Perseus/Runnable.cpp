// Runnable.cpp Copyright (c) 08/05/2018;  D.A.Tiger; GNU GPL 3

#include<Gorgona.h>
#include<Utils.h>
//#include<FXGameItem.h>
#include<LuaAPI.h>

using namespace PERSEUS;

/// Callbacky pro volani funkci z Lua scriptu ///
FXString lua_Launcher_p( Gorgona *a, const FXString &p_id, const FXString &p_cmd ); // Spusteni prikazu pomoci scriptovaneho spoustece


/*************************************************************************************************/
//FXDEFMAP( Runnable ) RunnableMAP[ ] = { }
FXIMPLEMENT( Runnable, FXObject, NULL, 0 )

Runnable::Runnable( Gorgona *a, FXObject *tgt, FXSelector sel )
{
  m_app     = a;
  m_target  = tgt;
  m_message = sel;
  m_change  = false;
  m_notify  = false;
}

Runnable::Runnable( Gorgona *a, const FXString &cmd, const FXString &launcher, FXObject *tgt, FXSelector sel )
       : m_launchid( launcher ), m_notify( false ), m_target( tgt ), m_message( sel ), m_change( false )
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
    m_execute = lua_Launcher_p( m_app, m_launchid, m_command );
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
    TiXmlElement *re = parent->FirstChildElement( "Perseus:Runnable" );

    if( re ) {
      m_launchid = re->Attribute( "type" );
      m_workdir = re->Attribute( "workdir" );
      //m_bckg  = re->Attribute( "background" );

      set_command( re->Attribute( "exec" ) );

      Read( re );
      resh = true;
    }   
  }

  return resh;
}
 
FXbool Runnable::save( TiXmlElement *parent, FXbool force )
{
  FXbool resh = false; 
    #ifdef __DEBUG
    std::cout << "[DEBUG PERSEUS::Runnable::Save( ) ] changed: " << m_change << " force: " << force << std::endl;
    #endif 

  if( parent && ( m_change || force ) ) {

    TiXmlElement *re = parent->FirstChildElement( "Perseus:Runnable" );

    if( re == NULL ) { 
      re = new TiXmlElement( "Perseus:Runnable" );
      parent->LinkEndChild( re );
    }

    re->SetAttribute( "exec",       m_command.text( ) );
    re->SetAttribute( "type",       m_launchid.text( ) );
    re->SetAttribute( "workdir",    m_workdir.text( ) );
    //re->SetAttribute( "background", m_backg.text( ) );
    
    Write( re );

    #ifdef __DEBUG
    std::cout << "[DEBUG PERSEUS::Runnable::Save( ) ]  saved " << std::endl;
    #endif 
    
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
{ 
  m_used    = 0;
  m_total   = 0;
  m_last    = 0;
  m_time    = 0;
  m_longest = 0;
}

Game::~Game( )
{

}

FXint Game::run( )
{
  //std::cout << "\n=== " << ( *gp_item )( "Basic:title" ) << " ==============================================" << std::endl;
  //set_workdir( ( *gp_item )( "Basic:workdir" ) );
  FXint pid = Runnable::run( );

  if( pid > 0 ) {
    Counter( );
    set_change( true );
  }

  return pid;
}

void Game::Counter( )
{
  m_used += 1;

  /*
  FXDate date = FXDate::localDate( );
  ( *gp_item )( "Extra:lastPlayed", FXString::value( date.day( ) ) + "/" + FXString::value( date.month( ) ) + "/" + FXString::value( date.year( ) ) );
  */
}

void Game::Read( TiXmlElement *runelement )
{
  //bool res = false;

  //if( Runnable::load( parent ) ) {
  TiXmlElement *game_el= NULL;
 
  if( runelement && ( game_el = runelement->FirstChildElement( "Perseus:Game" ) ) != NULL ) {
      game_el->Attribute( "count", &m_used );
      //res = true;
  }

  //return res;
}
 
void Game::Write( TiXmlElement *runelement )
{
  //bool res = false;
  
  if( runelement ) {
    TiXmlElement *game_el = runelement->FirstChildElement( "Perseus:Game" );
    if( game_el == NULL ) { 
      game_el = new TiXmlElement( "Perseus:Game" );
      runelement->LinkEndChild( game_el ); 
    }

    game_el->SetAttribute( "count", m_used );

    #ifdef __DEBUG
    std::cout << "[DEBUG PERSEUS::Game::Save( ) ] saved" << std::endl;
    #endif 

    //res = true;
  }

  //return res;
}


/*************************************************************************************************/
FXString lua_Launcher_p( Gorgona *a, const FXString &p_id, const FXString &p_cmd )
{
/**
   Globalni Callback volani launcheru v Lua modulu
   p_id - typ launcheru (Dosbox, Wine, ... atd. Volani v lue je tedy napr.: Launcher( "Dosbox", cmd )
   p_cmd   - retezec prikazu ze spoutece
 **/
  FXString resh = FXString::null;

  lua_getglobal(  a->getLua( ), "launcher"  );
  lua_pushstring( a->getLua( ), convert_str( p_id ) );
  lua_pushstring( a->getLua( ), convert_str( p_cmd ) );

  if( lua_pcall( a->getLua( ), 2, 1, 0 ) != 0 ) { /// FIXME : FUNKCE PREJIMA DVA ARGUMENTY A VRACI JEDNU HODNOTU!
    std::cout << "Chyba spusteni callbacku \'run\' - " << lua_tostring( a->getLua( ), -1 ) << std::endl;
  }
  else {
    resh = lua_tostring( a->getLua( ), -1 );
    lua_pop( a->getLua( ), -1 );
  }
  
  #ifdef __DEBUG
  std::cout << "[ DEBUG __launcher( ) ] Module: \'" << p_id << "\' Arg: \'" << p_cmd << "\'" << std::endl;
  #endif
  
 return resh;
}

/*** END ******************************************************************************************/
