/***************************************************************************************************
* File:      define.h                                                                              *
* Project:   Gorgona                                                                               *
* Copyright: Copyriright (C) 2023 by D.A.Tiger GNU GPL v3                                          *
* Author(s): D.A.Tiger ( drakarax@seznam.cz )                                                      *
* Note:      The internal definitions for Gorgona application                                      *
***************************************************************************************************/
#pragma once
/* asan;-fsanitize=address > project */

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

#include<lua5.4/lualib.h>
#include<lua5.4/lauxlib.h>
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

/* Internal shared header files */
#include<Utils.h>

/* Definitions of Global types */
class Gorgona;      // The Application object
class IconsTheme;   // Icons theme manager
class FXGameItem;   // Game object representation

typedef FXArray<FXGameItem*> FXGameItemArray;

/* Types of the Gorgona jobs */
#define JOB_NORMAL   1
#define JOB_RESERVED 2
#define JOB_SYSTEM   3

#define ID_NORMAL_MAX 99
#define ID_RESERVED_MAX 10

/* Data separators */
#define ENVPATHSEPSTRING ":"

/* System directories */
#define SCRIPTS_DIR "scripts"
#define UTILS_DIR   "utils"
#define MODULES_DIR "modules"
#define LUALIBS_DIR "libs"

/* Utils filenames */
#define GROUP_CHECKER   "grpchecker.sh"
#define MODULES_MANAGER "init.lua"

/* System and predefined data files */
#define DEF_CFG_FILE  "default.cfg" // The name of the file with an default configurations
//#define MIMESLISTFILE "mime/types"  // The relative path of mime types list file

/* Active the DEBUG mode */
// #define __DEBUG 1
//////////////////////////////////////////////////
/* Print debug log                               */
#ifdef __DEBUG
#define DEBUG_OUT(text) std::cout << "[DEBUG " << __FILE__ << " " << __LINE__ << "]: " << text << std::endl;
#else
#define DEBUG_OUT(text)
#endif

/*** END *****************************************************************************************/
