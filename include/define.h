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
/* asan;-fsanitize=address > project */

// The standard routine & objects, syscalls and system types
#include <cstdio>
#include <iostream>
#include <string>
#include <memory>

// The LUA interpreter
extern "C" {
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include<lua5.1/lualib.h>
#include<lua5.1/lauxlib.h>
}

// TinyXML - XML support
#include<tinyxml2.h>

// Foxlib & FoxGHI - GUI libraries
#include<fox-1.7/fx.h>
#include<fxghi.h>

/* Externam namespaces */
using namespace tinyxml2;
using namespace std;
using namespace FX;
using namespace FXGHI;

namespace PERSEUS { }
namespace ECHIDNA { }

// Internal shared header files
#include<../version.h>
//#include<Gorgona.h>
#include<IconsTheme.h>
#include<LuaAPI.h>
#include<FSM_Changes.h>
#include<Perseus/Runnable.h>
#include<Utils.h>

/* Types of the Gorgona jobs */
#define JOB_NORMAL   1
#define JOB_RESERVED 2
#define JOB_SYSTEM   3

#define ID_NORMAL_MAX 99
#define ID_RESERVED_MAX 10

/* Data separators */
#define ENVPATHSEPSTRING ":"

/* System and predefined data files */
#define DEF_CFG_FILE  "default.cfg" // The name of the file with an default configurations
//#define MIMESLISTFILE "mime/types"  // The relative path of mime types list file

/* Aktive the DEBUG mode */
#define __DEBUG 1

#endif // DEFINE_H_INCLUDED

/*** END *****************************************************************************************/
