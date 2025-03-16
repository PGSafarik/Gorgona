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

#ifndef PERSEUS_DESKTOP_H
#define PERSEUS_DESKTOP_H
/*************************************************************************
* DesktopServices.h                                                      *
*                                                                        *
*                                                                        *
* Copyright (c) 15/09/2025 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
//#include<core.h>
#include <define.h>

namespace PERSEUS {
/*** Environment Variables ***********************************************************************/
  FXString CheckEnv( const FXString &env, const FXString &def_val = FXString::null );
  FXuint CheckEnvList( const FXString &env, const FXString &def_val, FXStringList &list  );

/*** User directories ****************************************************************************/
  FXString HomeConfigDirectory( );
  FXString HomeDataDirectory( );

/*** System directories **************************************************************************/
  FXuint SystemConfigDirectories( FXStringList &list );
  FXuint SystemDataDirectories( FXStringList &list );

/*** Mime-type ************************************************************************************/
  FXint getMimeList( FXStringList &store, const FXString &file );

}      /* namespace PERSEUS */
#endif /* PERSEUS_DESKTOP_H */
