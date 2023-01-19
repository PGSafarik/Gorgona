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
#ifndef __PROCESS_H
#define __PROCESS_H

/*************************************************************************
* Process.h                                                              *
*                                                                        *
* Trida reprezentujici instanci herni aplikace(procesu)                  *
* Copyright (c) 05/05/2018 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<iostream>

#include<fox-1.7/fx.h>
#include<tinyxml.h>

#include<Utils.h>
#include<FXGameItem.h>
#include<LuaAPI.h>

namespace GORGONA {

  /// Obecna trida procesu Gorgony ///
  class Process : public FXObject {
  FXDECLARE( Process )
    FXArray<const char*> p_params;      // Seznam parametru i s prikazem (id 0) pro FXProcess
    StringList           p_childlist;   // Seznam potomku daneho procesu
    FXProcess            p_process;     // Objekt prvne spusteneho procesu
    FXString             p_launchid;    // Identifikator spoustece (pro moduly, jinak "native", nebo prazdny )
    FXString             p_command;     // Retezec s prikazem
    FXbool               p_status;      // Status spusteni prvniho procesu
    FXbool               p_notify;      // Indikator notifikace opreaci
    FXbool               p_parse;       // Indikator, zda je prikaz prezpracovan

    FXObject   *p_target;               // Cilovy objekt notifikaci
    FXSelector  p_selector;             // notifikacni zprava

  public :
    Process( const FXString &cmd, const FXString &launcher = "native", FXObject *tgt = NULL, FXSelector sel = 0 );
    virtual ~Process( );

    //////////////////////////////////////////////
    // Metody operaci
    //
    virtual FXbool   start( const FXString &workdir, FXbool waiting = false );
    virtual void     counter( );
    virtual FXint    waitForMy( FXbool value );
    virtual FXString changeWorkDir( const FXString &nwd );

    //////////////////////////////////////////////
    // Pristupove metody
    //
    void   set_notify( FXbool value = true ) { p_notify = value; }
    FXbool get_notify( )                     { return p_notify; }

  protected:
    Process( ) { }
    void ParseCommand( const FXString &cmd );
    FXbool is_native( ) { return ( p_launchid.empty( ) || p_launchid == "native" ); }
  };

  /// Trida urcena k spousteni a rizeni procesu her ///
  class GameProcess : public Process {
  FXDECLARE( GameProcess )
    FXGameItem *gp_item;

  public:
    GameProcess( FXGameItem *game, FXObject *tgt = NULL, FXSelector sel = 0 );
    virtual ~GameProcess( );

    //////////////////////////////////////////////
    // Metody operaci
    //
    virtual FXbool start( );
    virtual void   counter( );

  protected:
    GameProcess( ) { }
  };

}      /* Namespace GORGONA */
#endif /* __PROCESS_H */
/*** END ****************************************************************/
