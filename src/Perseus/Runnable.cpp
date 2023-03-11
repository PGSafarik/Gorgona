// Runnable.cpp Copyright (c) 08/05/2018;  D.A.Tiger; GNU GPL 3

#include<Gorgona.h>
#include<Utils.h>
//#include<FXGameItem.h>
#include<LuaAPI.h>

using namespace PERSEUS;

/// Callbacky pro volani funkci z Lua scriptu ///
FXString lua_Launcher_p( Gorgona *a, const FXString &p_id, const FXString &p_cmd ); // Spusteni prikazu pomoci scriptovaneho spoustece

/*************************************************************************************************/
FXDEFMAP( Runnable ) RUNMAP[ ] = { };
FXIMPLEMENT( Runnable, FXObject, RUNMAP, ARRAYNUMBER( RUNMAP ) )

Runnable::Runnable( Gorgona *a, FXObject *tgt, FXSelector sel )
{
  m_app      = a;
  m_target   = tgt;
  m_message  = sel;
  m_change   = false;
  m_notify   = false;
  m_terminal = false;
}

Runnable::Runnable( Gorgona *a, const FXString &cmd, const FXString &launcher, FXObject *tgt, FXSelector sel )
{
  m_app      = a;
  m_target   = tgt;
  m_message  = sel;
  m_change   = false;
  m_notify   = false;
  m_launchid = launcher;
  m_terminal = false; 
  set_command( cmd );
}

Runnable::~Runnable( )
{ }

FXint Runnable::run( )
{
  FXint pid = 0; 

  if( m_pid > 0 ) {
    if( !m_app->hasChild( m_pid ) ) { m_pid = 0; }
    else { return -1; }
  }

  FXString chwd = ChangeWorkDir( );
  
  if( ( pid = m_app->exec( m_execute, 0 ) ) <= 0 ) { 
    std::cout << "[ERROR Runnable]: Command " << m_command << " is not running!" << std::endl; 
  }
  else { m_pid = pid; }
  
  if( !chwd.empty( ) ) { FXSystem::setCurrentDirectory( chwd ); }
  return pid;
}

void Runnable::Command( const FXString &cmd )
{
  m_command = ( !cmd.empty( ) ? cmd : m_command );

  if( !m_command.empty( ) && !IsNative( ) ) {
    
    FXArray<FXString> prms;
    prms.push( m_command );
    m_execute = luams_launch( m_launchid, prms );
  }
  else { m_execute = m_command; } 

  CheckTerminal( );
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
      FXString term_str = re->Attribute( "Terminal" );
      if( !term_str.empty( ) ) { m_terminal = term_str.toInt( ); }
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
  if( parent && ( m_change || force ) ) {

    TiXmlElement *re = parent->FirstChildElement( "Perseus:Runnable" );

    if( re == NULL ) { 
      re = new TiXmlElement( "Perseus:Runnable" );
      parent->LinkEndChild( re );
    }

    re->SetAttribute( "exec",       m_command.text( ) );
    re->SetAttribute( "type",       m_launchid.text( ) );
    re->SetAttribute( "workdir",    m_workdir.text( ) );
    re->SetAttribute( "Terminal",   m_terminal );

    Write( re );
    
    resh = true;
  }

  return resh;
}

FXbool Runnable::validation( ) 
{
  /* Im working on this ... */
  return true;
}

void Runnable::CheckTerminal( )
{
  if( m_terminal && m_app->hasTerminal( ) ) {
    TermInfo *tnfo = m_app->getTerminal( );
    FXString  tcmd = tnfo->exec + " ";
    if( !tnfo->p_noclose.empty( ) ) { tcmd += tnfo->p_noclose + " "; }
    m_execute = tcmd + tnfo->p_run + " " + m_execute;
  } 
}

void Runnable::dump( ) 
{
  FXString text = "   Runnable = { \n";
  text += "     type: "       + m_launchid + "\n";   
  text += "     command: "    + m_command  + "\n";
  text += "     workdir: "    + m_workdir  + "\n";
  text += "     terminal: "   + FXString::value( m_terminal ) + "\n";
  text += "     executable: " + m_execute  + "\n";
  text += "   } \n";

  std::cout << text;  
}

/*************************************************************************************************/
FXDEFMAP( Game ) GAMEMAP[ ] = { };
FXIMPLEMENT( Game, Runnable, GAMEMAP, ARRAYNUMBER( GAMEMAP ) )

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

FXString Game::last( )
{
  FXString last_str = FXString::null;
  if( m_last > 0 ) {
    FXDate date; 
    date.setTime( m_last );
    last_str = FXString::value( date.day( ) ) + "/" + FXString::value( date.month( ) ) + "/" + FXString::value( date.year( ) );
  }
  return last_str;
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

void Game::dump( )
{ 
  FXString text = "   Runnable <- Game = { \n";
  text += "     Last run: "      + last( ) + "\n";   
  text += "     Number of run: " + FXString::value( m_used ) + "\n";
  text += "   } \n";

  std::cout << text;   
  
  Runnable::dump( );
}

void Game::Counter( )
{
  m_used += 1;
  m_last = FXDate::localDate( ).getTime( );
}

void Game::Read( TiXmlElement *runelement )
{
  TiXmlElement *game_el= NULL;
 
  if( runelement && ( game_el = runelement->FirstChildElement( "Perseus:Game" ) ) != NULL ) {
      game_el->Attribute( "count", &m_used );
      FXString value = game_el->Attribute( "last" );
      if( !value.empty( ) ) { m_last = value.toLong( ); } else { m_last = 0; }
  }
}
 
void Game::Write( TiXmlElement *runelement )
{
  if( runelement ) {
    TiXmlElement *game_el = runelement->FirstChildElement( "Perseus:Game" );
    if( game_el == NULL ) { 
      game_el = new TiXmlElement( "Perseus:Game" );
      runelement->LinkEndChild( game_el ); 
    }

    game_el->SetAttribute( "count", m_used );
    FXString value = FXString::value( m_last );
    game_el->SetAttribute( "last", value.text( ) );
  }
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
