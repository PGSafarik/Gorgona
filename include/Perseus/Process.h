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
* Small classes representing an process child                            *
* Copyright (c) 05/05/2018 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<define.h>

namespace PERSEUS {

  class Process: public FXProcess  {
    FXuint m_asid    = 0;     // Asociacni id polozky, ktera proces spustila
    FXuint m_runtime = 0;     // Cas startu behu procesu 
    FXint  m_retcode = 0;     // Navratovy kod
    FXbool m_run     = false; // indikace beziciho procesu

  public:
    Process( );
    virtual ~Process( );

    /* Access methods */
    void   set_asid( FXuint value ) { m_asid = value; } 
    FXuint asid( )                  { return m_asid; }
    FXuint runtime( )               { return m_runtime; }
    FXbool is_running( )            { return m_run; }
    FXint  retcode( )               { return m_retcode; }  
    
    /* operations */
    FXbool run( const EntriesList &cmd );
    void   exited( FXint code ) { m_run = false; m_retcode = code; };

  protected:
  
  };   /* Class Process     */
}      /* Namespace PERSEUS */
#endif /* __PROCESS_H       */

/*** END ****************************************************************/
