// Runnable.cpp Copyright (c) 08/05/2018;  D.A.Tiger; GNU GPL 3

#include<Gorgona.h>
#include<Perseus/Runnable.h>

using namespace PERSEUS;

/*************************************************************************************************/
FXDEFMAP( Runnable ) RUNMAP[ ] = { 
  FXMAPFUNC( SEL_SIGNAL, Runnable::PROC_EXIT, Runnable::OnSig_Process )
};
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
  if( ( pid = m_app->exec( m_execute, 0 ) ) > 0 ) { 
    m_pid = pid; 
    m_app->sig_child_exit->connect( this, Runnable::PROC_EXIT );
  }
  
  if( !chwd.empty( ) ) { FXSystem::setCurrentDirectory( chwd ); }
  
  return pid;
}

void Runnable::Command( const FXString &cmd )
{
  m_command = ( !cmd.empty( ) ? cmd : m_command );

  if( !IsNative( ) ) {
    if( !m_command.empty( ) ) {
      FXArray<FXString> prms;
      prms.push( m_command );
      m_execute = luams_launch( m_launchid, prms );
    }
    else {
      m_execute = luams_launch( m_launchprms );
    }
  }
  else { 
    m_execute = m_command; 
  } 

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

FXbool Runnable::load( XMLElement *parent )
{
  FXbool resh = false; 

  if( parent ) {
    XMLElement *re = parent->FirstChildElement( "Perseus:Runnable" );

    if( re ) {
      XMLElement *le = re->FirstChildElement( "Perseus:Launcher" );
      if( le ) {
        for( const XMLAttribute *attr = le->FirstAttribute( ); attr; attr = attr->Next( ) ) { 
          FXString _name  = attr->Name( );
          FXString _value = attr->Value( );
          if( _name == "key" ) { m_launchid = _value; }
          m_launchprms.insert( _name, _value );
          
        }
        Command( ); 
      }
      else {
        m_launchid = re->Attribute( "type" );
        set_command( re->Attribute( "exec" ) );
      }

      m_workdir = re->Attribute( "workdir" );
      FXString term_str = re->Attribute( "Terminal" );
      if( !term_str.empty( ) ) { m_terminal = term_str.toInt( ); }
       
      Read( re );
      resh = true;
    }   
  }

  return resh;
}
 
FXbool Runnable::save( XMLElement *parent, FXbool force )
{
  FXbool resh = false; 
  if( parent && ( m_change || force ) ) {
    
    // Check or create the Runnable element  
    XMLElement *re = parent->FirstChildElement( "Perseus:Runnable" );
    if( re == NULL ) { re = parent->InsertNewChildElement( "Perseus:Runnable" ); }
 
    /*
    if( ( !m_launchid.empty( ) && m_launchid != "native" )  {
      // For items used a module service 'Launch'
      XMLElement *le = re->FirstChildElement( "Perseus:Launcher" );
      if( le == NULL ) {
        le = new XMLElement( "Perseus:Launcher" );
        re->LinkEndChild( le );
      } 
      
      le->SetAttribute( "key", m_launchid );

    }
    else {
      // For native applications
      re->SetAttribute( "exec", m_command.text( ) );
    }
    */
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

 long Runnable::OnSig_Process( FXObject *tgt, FXSelector sel, void  *data ) 
 {
  FXString msg  = "";
  FXint pid = *((FXint*) data);
   
  if ( pid == m_pid ) { 
    Process *proc = m_app->findChild( m_pid );
    if( proc ) { 
      FXint status = proc->retcode( ); 
      if( m_app->removeChild( m_pid ) ) {
        m_pid = 0;
        m_app->sig_child_exit->disconnect( this );

        if( status == 0 ) {
          msg = "Unregister the process of the descendant "; 
          msg += FXString::value( pid ) + ", which just finished with exit code " + FXString::value( status ) + "\n";
        } 
        else { 
          // Information of the user of bad a process termaination 
          FXString head = "Non-standard process termination"; 
          FXString msg = "The process " + FXString::value( pid ) + " terminated with an error. \n";
          msg += "Exit code : " + FXString::value( status );   
    
          FXMessageBox::warning( m_app, MBOX_OK, head.text( ), msg.text( ) );
        }
        std::cout << msg << std::endl;   
      }
    } 
    //else { std::cout << "running!! \n"; }    

  }
  return 0;
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
    //set_change( true );
    get_app( )->notify_changes( FSM_Changes::ID_CHANGE );
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

void Game::Read( XMLElement *runelement )
{
  XMLElement *game_el= NULL;
 
  if( runelement && ( game_el = runelement->FirstChildElement( "Perseus:Game" ) ) != NULL ) {
      //game_el->Attribute( "count", &m_used );
      m_used = game_el->IntAttribute( "count", 0 );
      FXString value = game_el->Attribute( "last" );
      if( !value.empty( ) ) { m_last = value.toLong( ); } else { m_last = 0; }
  }
}
 
void Game::Write( XMLElement *runelement )
{
  if( runelement ) {
    XMLElement *game_el = runelement->FirstChildElement( "Perseus:Game" );
    if( game_el == NULL ) { game_el = runelement->InsertNewChildElement( "Perseus:Game" ); }

    game_el->SetAttribute( "count", m_used );
    FXString value = FXString::value( m_last );
    game_el->SetAttribute( "last", value.text( ) );
  }
}

/*** END ******************************************************************************************/
