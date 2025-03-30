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
#ifndef GORGONA_PROCESESSION_H
#define GORGONA_PROCESESSION_H

/*************************************************************************
* ProcSession.h                                                          *
*                                                                        *
* Small class representing a process session                             *
* Copyright (c) 21/01/2025 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<fcntl.h>

#include<map>
#include<string>
#include<vector>
#include<cstdio>

#include<define.h>
#include<Perseus/Process.h>

namespace PERSEUS {
  
  class ProcSession : public std::map<FXint, PERSEUS::Process* > {
    FXString m_name;       // Name of the Gorgona sesion
    FXString m_id;         // ID string of the Gorgona sesion
    pid_t    m_sid = -1;   // A identifier of then system process session
    FXint    m_actjob = 0; // Actual set job group

    std::vector<FXint>  m_maintable; // Maintable ALL descedants pid runned in this session
    std::vector<FXint>  m_jobs;      // Table of job groups

    FXString m_grpcheck; // Path to the group check utility
  public:
    ProcSession( const FXString &name = "GORGONA_SESSION", const FXString &id = FXString::null );
    virtual ~ProcSession( );

    /* Access methods */
    FXString id( )            { return m_id; } 
    FXint    sid( )           { return static_cast<FXint>( m_sid ); }
    FXString name( )          { return m_name; }
    FXbool   has( FXint pid ) { auto it = find( pid ); return it != end( ); }
    FXint    set_job( FXuint type );
 
    /* opeartions */
    FXbool  start( const FXString &check_utility );                               // Start to the process session
    FXbool  terminate( FXbool fast = false);        // End to the process session     
    FXbool  join( FXint pid, FXint group_id = 0 );  // Add pid to main table
    FXint   check( );                               // Checking processes in Session

  protected:
  };
  
}      /* Namespace PERSEUS     */
#endif /* GORGONA_PROCESESION_H */
