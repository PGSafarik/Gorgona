# Gorgona - Game collection manager and launcher for Linux OS 
-22/03/2025-

*Content:*
  1. [General info](#1-general-info)
  2. [Compilation and installation](#2-compilation-and-installation)
  3. [Known errors and bugs](#3-known-errors-and-bugs)
  4. [The project state and plans for the future](#4-project-state-and-plans-for-the-futhure)
  5. [Author and project licencion](#5-Author-and-project-licencion)
  6. [Internet](#6-internet)
  
<p align="center"><img src="docs/images/screenshot.png?raw=true" width="70%" />  

## 1. GENERAL INFO:

The Gorgona program is used to maintain a library of installed games under the Linux operating system, sort them into categories and run them. Development Gorgona began sometime around 2015 as a replacement for Parallel Realities' Java application JGameLauncher. After a pause of several years, development of the project has resumed. 

The Gorgon program is basically developed in C++ and the GUI is provided by the Fox framework. It also includes a Lua interpreter and defines simple APIs that allow you to write plugins to serve specific game requirements. The library of game titles is implemented using a markup language
XML, which the program interacts with via the C++ DOM parser TinyXML. 

Please note that Gorgona is constantly evolving and therefore - outside of the stable releases - may contain unexpected bugs, some of which features of the program may change without warning, or may be discontinued outright. Please refer to the user documentation for more information. 

For this project and its parts, names from Greek mythology are used
  * Gorgona – Game manager
  * Morfeus – Univerzal GUI game launch.
  
## 2. FEATURES
  * Insert, edit and removed a game titles on The game library
  * Assigning a library item to a user-defined category
  * The game title library is implemented using XML, so no special tools are required to browse and edit it.
  * Gorgona offers plugins to handle the launch of specific games. 


## 2. COMPILATION AND INSTALLATION:
*Dependecies:*
  * [Foxtoolkit 1.7.76](http://fox-toolkit.org/ftp/fox-1.7.76.tar.gz)
  * [TinyXml](https://sourceforge.net/projects/tinyxml)
  * [Lua 5.1](https://www.lua.org)

*Aditional dependencies for compilation:*
  * cmake >= 3.10
  * g++
  
After downloading the project from Github (and possibly unzipping it), go to the folder with the project and enter the following command sequence:
```
  mkdir build
  cd build 
  cmake .. && make
  sudo make install
```
After successfully compiling and building the project, the resulting program, including the Lua language launcher, 
will be installed in the directory /opt/Gorgona

Note:
The foxlib library MUST be installed on your system at compile time. Version 1.7 is a development version, so it is possible that you will not have it on your system and it will not be available in the repository, so you will have to install it manually. More information on how to do this can be found on the foxtoolkit website. The following lines refer to any CMakeLists modifications.

If you have the library installed in a location other than the standard one, this fact must be entered in the CMakeLists.txt file in the project directory. Find and change the lines setting the library path:
```
    INCLUDE_DIRECTORIES( "/Cesta/vasi/instalaci/foxlib/include" )   #Foxlib headers
    LINK_DIRECTORIES( "/Cesta/vasi/instalaci/foxlib/lib" )          #Foxlib library
```
These lines are commented out by default, so remove the leading '#' characters after editing, and then save the file. Next, follow the instructions for building and installing the project above as normal.

## 3. KNOWN ERRORS AND BUGS
* ### 28/03/2019 - Bad return value from lua launchers - opened 
Lua launchers do not correctly return a return value after a game launched in this way ends. It is probably a wrongly fetched value from the lua buffer.  

* ### 11/01/2023 - Need to manually enter path to modules in ModManager.lua - Open 
The cause is in the design of Lua script integration in Gorgona. For now, a workaround to the C->LUA API is being considered.  

## 4. PROJECT STATE AND PLANS FOR THE FUTHURE
Project Morfeus, which was developed in parallel with Gorgon, was separated from it as an independent utility and continues to be developed independently

11/01/2023 Aktualize:  
* Default Foxlib version is 1.7.81
* Gorgon's top-level windows are now managed by the FoxGHI library. The default version is 0.4.1
* The Window toolbars is managed with FoxGHI header Boxes
* Modified top-level window layout following the change in top-level window management
* The item find frame moved on Header bar
* Insert a new module for support run games from Steam. For now, you have to manually search for the Steam app id and enter it in the command field
* Insert a new module for support an games installed with the Legendary utility (Epic games and Gog.com). Again, you must manually enter the game id.
* A simple outline of item validation. So far it checks (and not correctly) validity by the existence of an executable file for native items. Items that are evaluated as not valid are dumped to stdout.
* The XML tags representing the individual game list items are now grouped into the xml tag "Library". This change is currently incompatible with the previous versions and the original xml data file will not load correctly. You need to manually fix it or create a new one.
* A few minor corrections and changes

## 5. AUTHOR AND PROJECT LICENCION:
Copyright (C) 2015 - 2022 Pavel Šafařík <drakarax@seznam.cz>
All rights reseved.

This file is part of Gorgona.

Gorgona is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gorgona is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gorgona. If not, see 
<https://www.gnu.org/licenses/>.

## 6. INTERNET
  * [Author's blog (CZ)](http://bfuplusplus.blogspot.cz/)
  * [Foxtoolkit home page](http://fox-toolkit.org/)
  * [Morfeus](https://github.com/PGSafarik/Morfeus)
  * [FoxGHI](https://github.com/PGSafarik/FoxGHI)
  * [Legendary](https://github.com/derrod/legendary)
  * [SteamDB](https://steamdb.info/)
  * [Parallel Realities (EN)](https://www.parallelrealities.co.uk/)
  * [Lutris (EN)](https://lutris.net/)
  * [GameHub (EN)](https://tkashkin.github.io/projects/gamehub/)

=== END ===

