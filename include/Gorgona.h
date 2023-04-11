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
#include<Perseus/Process.h>

class Gorgona : public FXApp {
FXDECLARE( Gorgona )
  FXObject *m_tgt;         // Notify target
  FXuint    m_message;     // ID of notifikacation message
  FXbool    m_created;     // Application created (finised the function call craete )
  FXbool    m_initialized; // Finisched the initialize proces this application

  /* Child process managment */
  FXDictionaryOf<PERSEUS::Process>  m_descendants; // List of registered descendants of the Gorgona process 

  /* Lua */
  lua_State *m_lua;        // Instance of the Lua language interpreter
  FXbool     m_luaOk;      // True indicate succeful initialize the Lua language ;) 
  FXString   m_initscript; // Initialize script (lua)

  /* Data */
  Library         *m_library;   // Library of games ;)
  TermInfo        *m_term;      // Terminal profile
 
  /* Configurations */
  FXbool    m_verbose;     // Verbose mod
  FXString  m_profiledir;  // Path of rofile directory 
  FXString  m_gamelist;    // Filename for list of games (xml)
  FXbool    m_autosave;    // Enable/desable autosave of the changes

public:
  Gorgona( const FXString& name = "Gorgona", const FXString& vendor = FXString::null );
  virtual ~Gorgona( );

  /* Access methods */
  void setNotify( FXObject *tgt, FXuint msg )           { m_tgt = tgt; m_message = msg; }
  lua_State* getLua( )                                  { return m_lua; }
  FXbool     isLuaInit( )                               { return m_luaOk; } 
  FXbool     isCreated( )                               { return m_created; }
  FXbool     isInitialized( )                           { return m_initialized; }      
  Library*   getLibrary( )                              { return m_library; }
  FXString   getProfileDir( )                           { return m_profiledir; }
  void       setProfileDir( const FXString &directory ) { m_profiledir = directory; };
  FXString   getLibraryFilenme( )                       { return m_gamelist; }
  void       setLibraryFilenme( const FXString &name )  {  m_gamelist = name; }
  FXbool     hasChild( FXint pid = 0 );  
  PERSEUS::Process* findChild( FXint pid );

  TermInfo* getTerminal( )  { return m_term; }
  FXbool    hasTerminal( )  { return ( !m_term->exec.empty( ) ); }

  /* Operations methods */
  virtual void create( );
  virtual void init( int& argc, char** argv, FXbool connect = true );

  FXint exec( const FXArray<const FXchar*> &cmd, FXuint term_opts );
  FXint exec( const FXString &cmd, FXuint term_opts );
  FXint wait( PERSEUS::Process *process, FXbool notify = false );
  FXint execLuaFile( const FXString &script );

  long notify( FXuint mtype, void *mdata );
 
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
  long Notify( FXbool enable, FXuint mtype = SEL_CHANGED, void *mdata = NULL );  // Send of notification message, if anyone 
  void ParseCommand( const FXString &cmd, FXArray<const char*> *buffer );        // Split one substring from the command string on array
  void LuaInit( );                                                               // Initialize langugae interpret of Lua 
  void Read_Config( );                                                           // Load configurations data
  void Write_Config( );                                                          // Write configurations 
  void Load_Library( );                                                          // Load items library(ies)   
  void Save_Library( );                                                          // Save items library(ies)   
};

#endif /* __GORGONA_H */
