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
* FXGameLauncher.h                                                       *
*                                                                        *
* Trida reprezentuje instanci aplikace a komunikuje se systemem          *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
* ./FoxGHICP --name Gorgona --vendor FXGameLauncher                      * 
*************************************************************************/
// Syscalls and system types
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

// The LUA interpreter
#include<lua5.1/lualib.h>
#include<lua5.1/lauxlib.h>

// Foxlib, the GUI library
#include<fox-1.7/fx.h>
#include<fxghi.h>

// Tiny xml - xml support
#include<tinyxml.h>

// Internal headers
#include<Utils.h>
#include<Boxes.h>
#include<IconsTheme.h>
//#include<FXGameItem.h>
#include<FXLaunchEditor.h>
#include<LuaAPI.h>
#include<FXListPane.h>
#include<GO_Keywords.h>

class Gorgona : public FXApp {
FXDECLARE( Gorgona )

  FXObject            *m_tgt;           // Cil notifikacnich zprav
  FXuint               m_message;       // ID notifikacni zpravy 
  FXArray<FXProcess*>  m_descendants;   // Seznam spustenych procesu (potomku Gorgony)

  FXbool m_verbose;                      // Ukecany mod

  /* Lua */
  lua_State *m_lua;                      // Instance interpreteru jazyka Lua
  FXString   m_initscript;               // Inicializacni script 

public:
   Gorgona( const FXString& name = "Gorgona", const FXString& vendor = FXString::null );
   virtual ~Gorgona( );

   /* Access methods */
   void setNotify( FXObject *tgt, FXuint msg ) { m_tgt = tgt; m_message = msg; }
   lua_State* getLua( ) { return m_lua; }

  /* Operations methods */
  virtual void create( );
  virtual void init( int& argc, char** argv, FXbool connect = true );

  FXint exec( const FXArray<const FXchar*> &cmd, FXuint term_opts, FXuint sudo_opts, FXuint proc_opts );
  FXint exec( const FXString &cmd, FXuint term_opts, FXuint sudo_opts, FXuint proc_opts );
  FXint wait( FXProcess *process, FXbool notify = false );
  FXint execLuaFile( const FXString &script );

  long notify( FXuint mtype, void *mdata );
 
  /* GUI handlers & messages */
  enum {
   SIGNAL_CHLD = FXApp::ID_LAST,  // End of child proccess
   ID_LAST
  };

  long OnSig_ExitChild( FXObject *sender, FXSelector sel, void *data );

protected:
  long Notify( FXbool enable, FXuint mtype = SEL_CHANGED, void *mdata = NULL );  // Odesle notifikacni zpravu
  void ParseCommand( const FXString &cmd, FXArray<const char*> *buffer );        // Rozdeli jednotlive slozky retezce prikazu do pole
  FXbool LuaInit( );                                                             // Inicializace interpreteru jazyka Lua 
  void ReadConfig( );                                                            // Nacte konfiguracni data
};

#endif /* __GORGONA_H */
