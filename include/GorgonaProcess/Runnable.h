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
#ifndef __PERSEUS_H
#define __PERSEUS_H

/*************************************************************************
* Runnable.h                                                              *
*                                                                        *
* Trida reprezentujici instanci herni aplikace(procesu)                  *
* Copyright (c) 05/05/2018 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<iostream>

#include<fox-1.7/fx.h>
#include<tinyxml.h>

#include<Gorgona.h>
#include<Utils.h>
#include<FXGameItem.h>
#include<LuaAPI.h>

namespace PERSEUS {

  /// Obecna trida procesu Gorgony ///
  class Runnable: public FXObject  {
  FXDECLARE( Runnable)
    Gorgona             *m_app;
    FXString             p_launchid;    // Identifikator spoustece (pro moduly, jinak "native", nebo prazdny )
    FXString             m_workdir;     // Pozadovany pracovni adresare
    FXString             p_command;     // Retezec s prikazem
    FXbool               p_notify;      // Indikator notifikace opreaci

    FXObject   *p_target;               // Cilovy objekt notifikaci
    FXSelector  p_selector;             // notifikacni zprava

  public :
    Runnable( Gorgona *a, const FXString &cmd, const FXString &launcher = "native", FXObject *tgt = NULL, FXSelector sel = 0 );
    virtual ~Runnable( );

    /* Access methods */
    void     set_notify( FXbool value = true )  { p_notify = value; }
    FXbool   get_notify( )                      { return p_notify;  }
    void     set_workdir( const FXString &dir ) { m_workdir = dir;  }
    FXString get_workdir( )                     { return m_workdir; }  

    /* Operations methods */
    virtual FXint run( );
    
  protected:
    Runnable( ) { }
    //void ParseCommand( const FXString &cmd );
    FXbool is_native( ) { return ( p_launchid.empty( ) || p_launchid == "native" ); }
    FXString ChangeWorkDir( );
  };

  /// Trida urcena k spousteni a rizeni procesu her ///
  class Game : public Runnable {
  FXDECLARE( Game )
    FXGameItem *gp_item;

  public:
    Game( Gorgona *a, FXGameItem *game, FXObject *tgt = NULL, FXSelector sel = 0 );
    virtual ~Game( );

    //////////////////////////////////////////////
    // Metody operaci
    //
    virtual FXint run( );
    void counter( );

  protected:
    Game( ) { }
  };

}      /* Namespace PERSEUS */
#endif /* __PERSEUS_H */
/*** END ****************************************************************/
