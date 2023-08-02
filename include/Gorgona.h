/*************************************************************************
* This program is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU General Public License as published by   *
* the Free Software Foundation, either version 3 of the License, or      *
* (at your option) any later version.                                    *
*                                                                        *
* This program is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU General Public License      *
* along with this program.  If not, see <https://www.gnu.org/licenses/>. *
*************************************************************************/
#ifndef __GORGONA_H
#define __GORGONA_H

/*************************************************************************
* Gorgona.h                                                       *
*                                                                        *
* Trida reprezentuje instanci aplikace a komunikuje se systemem          *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
* ./FoxGHICP --name Gorgona --vendor FXGameLauncher                      * 
*************************************************************************/
#include<define.h>
#include<FXGameItem.h> 
#include<FSM_Changes.h>
#include<Perseus/Process.h>

class Gorgona : public FXApp {
FXDECLARE( Gorgona )
  FXbool    m_verbose;     // Verbose mod
  FXbool    m_created;     // Application created (finised the function call craete )
  FXbool    m_initialized; // Finisched the initialize proces this application

  FSM_Changes m_modify;      // Modify statemat 

  /* Child process managment */
  FXDictionaryOf<PERSEUS::Process>  m_descendants; // List of registered descendants of the Gorgona process 

  /* Lua */
  lua_State *m_lua;        // Instance of the Lua language interpreter
  FXbool     m_luaOk;      // True indicate succeful initialize the Lua language ;) 
  FXString   m_initscript; // Initialize script (lua)

  /* Data */
  FXString        m_profiledir; // Path of rofile directory 
  FXString        m_gamelist;   // Filename for list of games (xml)
  XMLDocument     mx_document;  // XML instance of the games list
  XMLElement      *mx_root;     // XML root element of the games list 
  Library         *m_library;   // Library of games ;)
  TermInfo        *m_term;      // Terminal profile
 
public:
  Gorgona( const FXString& name = "Gorgona", const FXString& vendor = FXString::null );
  virtual ~Gorgona( );
   
  /* Signals / Slots */
  GSignal *sig_child_exit; 

  /* Access methods */
  lua_State* getLua( )                                  { return m_lua; }
  FXbool     isLuaInit( )                               { return m_luaOk; } 
  FXbool     isCreated( )                               { return m_created; }
  FXbool     isInitialized( )                           { return m_initialized; }      
  Library*   getLibrary( )                              { return m_library; }
  FXString   getProfileDir( )                           { return m_profiledir; }
  void       setProfileDir( const FXString &directory ) { m_profiledir = directory; };
  FXString   getLibraryFilenme( )                       { return m_gamelist; }
  void       setLibraryFilenme( const FXString &name )  {  m_gamelist = name; }

  FXbool     removeChild( FXint pid, FXbool force = false );
  FXbool     hasChild( FXint pid = 0 );       // true - if its a process with the PID in internal list
  PERSEUS::Process* findChild( FXint pid );   // return Process object instance, if has a child process with the PID. Else NULL.

  TermInfo* getTerminal( )  { return m_term; }
  FXbool    hasTerminal( )  { return ( !m_term->exec.empty( ) ); }                        

  /* Operations methods */
  virtual void create( );
  virtual void init( int& argc, char** argv, FXbool connect = true );

  FXint exec( const FXArray<const FXchar*> &cmd, FXuint term_opts );
  FXint exec( const FXString &cmd, FXuint term_opts );
  FXint wait( PERSEUS::Process *process, FXbool notify = false );
  FXint execLuaFile( const FXString &script );

  long notify_changes( FXuint mesg ) { return m_modify.handle( this, FXSEL( SEL_COMMAND, mesg ), NULL ); }  
 
  /* GUI handlers & messages */
  enum {
   SIGNAL_CHLD = FXApp::ID_LAST,  // End of child proccess

   /* Explicit saving */
   SAVE_LIBRARY,                  
   SAVE_CONFIGURE,

   ID_LAST
  };
  long OnSig_ExitChild( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_Save( FXObject *sender, FXSelector sel, void *data );
  long onCmdQuit( FXObject *sender, FXSelector sel, void *data );

protected:
  void ParseCommand( const FXString &cmd, FXArray<const char*> *buffer );        // Split one substring from the command string on array
  void LuaInit( );                                                               // Initialize langugae interpret of Lua 
  void ReadConfig( );                                                            // Load configurations data
  void LoadLibrary( );                                                            // Load items library(ies)   
  void Save_Library( );
};

#endif /* __GORGONA_H */
