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

/*************************************************************************
* XMLUtils.h                                                             *
*                                                                        *
* Small objects for manipulation with Tiny XML                           *
* FXObjectListOf and FXObjectList                                        *
* Copyright (c) 05/05/2018 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
/* Policy xml stream element */
template<class T> struct XMLElementStream : public T 
{
  void operator <<( const XMLElement &source ) { T::read( source ); }
  void operator >>( XMLElement &target )       { T::write( target ); } 
};

