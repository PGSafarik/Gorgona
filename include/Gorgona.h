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
#include<FXGameItem.h>
#include<GorgonaProcess/Process.h>
#include<FXLaunchEditor.h>
#include<LuaAPI.h>
#include<FXListPane.h>
#include<GO_Keywords.h>

class Gorgona : public FXApp {
FXDECLARE( Gorgona )

FXObject *m_tgt;           // Cil notifikacnich zprav
FXuint    m_message;       // ID notifikacni zpravy 

public:
   Gorgona( const FXString& name = "Gorgona", const FXString& vendor = FXString::null );
   virtual ~Gorgona( );

   /* Access methods */


  /* Operations methods */
  virtual void create( );
  virtual void init( int& argc, char** argv, FXbool connect = true );

  FXint exec( const FXArray<const FXchar*> &cmd, FXuint term_opts, FXuint sudo_opts, FXuint proc_opts );
  FXint wait( FXProcess *process, FXbool notify = false );
 
  /* GUI handlers & messages */


protected:
  long Notify( FXbool enable, FXuint mtype = SEL_CHANGED, void *mdata = NULL );


};

#endif /* __GORGONA_H */
