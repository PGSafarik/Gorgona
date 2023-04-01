/***************************************************************************************************
* File:      define                                                                                *
* Project:   Gorgona                                                                               *
* Copyright: Copyriright (C) 2023 by D.A.Tiger GNU GPL v3                                   *
* Author(s): D.A.Tiger ( drakarax@seznam.cz )                                                      *
* Note:      The internal definitions for Morfeus application                                      *
***************************************************************************************************/
#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED
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

// The standard routine & objects, syscalls and system types
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <iostream>

// The LUA interpreter
#include<lua5.1/lualib.h>
#include<lua5.1/lauxlib.h>

// Tiny xml - xml support
#include<tinyxml.h>
//#include<tinyxml2.h>

// Foxlib & FoxGHI - GUI libraries
#include<fox-1.7/fx.h>
#include<fxghi.h>

// Internal shared header files
#include<IconsTheme.h>
#include<Utils.h>

//#define __DEBUG

using namespace tinyxml2;
using namespace std;
using namespace FX;
using namespace FXGHI;

#endif // DEFINE_H_INCLUDED

/*** END *****************************************************************************************/
