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
#pragma once
#include<Utils.h>

/*************************************************************************
* Process.h                                                              *
*                                                                        *
* Small classes representing an process child                            *
* Copyright (c) 05/05/2018 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
namespace PERSEUS {
  class Process: public FXProcess  {
    FXuint m_asid    = 0;     // Asociacni id polozky, ktera proces spustila
    FXuint m_runtime = 0;     // Cas startu behu procesu 
    FXint  m_retcode = 0;     // Navratovy kod

    /*** Control state-mat ***/ 
    FXuint m_control_state;         // Stav ve kterem se objekt prave nachazi 
    FXuint m_control_trans[ 4 ][ 4 ]; // Tabulka moznych prechodu z jednoho stavu do nasledujiciho
    FXuint m_control_ends;          // Stav ve kterem automat skoncil

  public:
    Process( );
    virtual ~Process( );
    
    /* Controls states */
    enum {
      ST_INIT = 0,  // Inicializace
      ST_RUNNING,   // Spusten
      ST_ERROR,     // Chyba
      ST_STOPPED,   // Zastaveno
      ST_LAST 
    }; 

    /* Access methods */
    FXuint asid( )                  { return m_asid; }
    FXuint runtime( )               { return m_runtime; }
    FXbool is_running( )            { return ( m_control_state == Process::ST_RUNNING ); }
    FXint  retcode( )               { return m_retcode; }  
    FXuint get_state( )             { return m_control_state; }
    void   set_asid( FXuint value ); 

    /* operations */
    FXbool run( const CharsList &cmd );
    void   exited( FXint code );

  protected:
    void   InitControlMat( );
    FXbool CheckStateMat( FXuint next_state, FXbool fast = false );

  };   /* Class Process     */
}      /* Namespace PERSEUS */
/*** END ****************************************************************/
