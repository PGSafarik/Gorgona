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
#ifndef __TERMINFO_H
#define __TERMINFO_H

/*************************************************************************
* TermInfo.h                                                             *
*                                                                        *
* Small classes representing a terminal emulator (TE) profile            *
* Copyright (c) 27/04/2024 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<define.h>
namespace PERSEUS {
  
  struct TermProfile {
    FXString name;      // Name (identifier) on TE
    FXString exec;      // Runnable file for TE
    FXString p_run;     // Parameter selling command to run in TE
    FXString p_noclose; // Parameter preventing termination of TE after execution of the command
    FXString p_workdir; // A parameter that allows (for some) TE to change the working directory (Gorgona then does not use it)

    TermProfile( const FXString ident = FXString::null );
    ~TermProfile( );

    FXbool load( const FXSettings &conf, const FXString ident = FXString::null );
    FXbool save( FXSettings &conf, const FXString ident = FXString::null );
  };
  
}      /* Namespace PERSEUS */
#endif /* __TERMINFO_H */
