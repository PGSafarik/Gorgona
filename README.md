# Gorgona - Game collection manager and launcher for Linux OS 
-23/09/2022-

*Content:*
  1. [General info](#1-general-info)
  2. [Compilation and installation](#2-compilation-and-installation)
  3. [Known errors and bugs](#3-known-errors-and-bugs)
  4. [The project state and plans for the future](#4-project-state-and-plans-for-the-futhure)
  5. [Author and project licencion](#5-Author-and-project-licencion)
  6. [Internet](#6-internet)
  
<p align="center"><img src="docs/images/screenshot.png?raw=true" width="50%" />  

## 1. GENERAL INFO:
!THIS PROJECT IS STOPPED! - see on the part 4

The Gorgona project is intended as a manager and launcher of games under the Linux OS. It is written in C++ using the Fox GUI framework. Its primary task was to replace the Java application JGameLauncher by Parallel Realities. The main motivation was the need to write shell launchers for many games, or the impossibility of sorting games according to certain criteria (e.g. genre).

At this point, the Gorgon has more or less fulfilled its original goals and in some ways surpassed its original design. It includes a Lua interpreter and several built-in functions that allow you to write universal game launchers. At the moment, it only allows you to manually insert the game into the list and enter the necessary data about the game. Gorgona then places the game in the corresponding section, from where it can then also be launched. The list of games can be displayed in two variants (icons and a detailed list). 

For this project and its parts, names from Greek mythology are used
  Gorgona – Game manager
  Morfeus – Univerzal GUI game launch.
  
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
    
## 4. PROJECT STATE AND PLANS FOR THE FUTHURE

## 5. AUTHOR AND PROJECT LICENCION:
Copyright (C) 2015 - 2022 Pavel Šafařík <drakarax@seznam.cz>
All rights reseved.

This file is part of Gorgona.

Gorgona is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gorgona is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with foxGHI. If not, see 
<https://www.gnu.org/licenses/>.

## 6. INTERNET
  * [Author's blog (CZ)](http://bfuplusplus.blogspot.cz/)
  * [Morfeus]
  * [Parallel Realities (EN)](https://www.parallelrealities.co.uk/)
  * [Lutris (EN)](https://lutris.net/)
  * [GameHub (EN)](https://tkashkin.github.io/projects/gamehub/)

## === END ===  
