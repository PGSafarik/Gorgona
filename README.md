# Gorgona - Game collection manager and game launcher for Linux OS 
-08/09/2022-

*Content:*
  1. [General info](#1-general-info)
  2. [Compilation and installation](#2-compilation-and-installation)
  3. [Known errors and bugs](#3-known-errors-and-bugs)
  4. [The project state and plans for the future](#4-project-state-and-plans-for-the-futhure)
  5. [Author and project licencion](#5-Author-and-project-licencion)
  6. [Internet](#6-internet)
  
<p align="center"><img src="doc/images/screenshot.png?raw=true" width="50%" />  

## 1. GENERAL INFO:
!THIS PROJECT IS STOPPED! - see on the part 4

## 2. COMPILATION AND INSTALLATION:
*Dependecies:*
  [Foxtoolkit 1.7.76](http://fox-toolkit.org/ftp/fox-1.7.76.tar.gz)
  [TinyXml]          (https://sourceforge.net/projects/tinyxml)
  [Lua 5.1]          (https://www.lua.org)

*Aditional dependencies for compilation:*
  cmake >= 3.10
  g++
  
After downloading the project from Github (and possibly unzipping it), go to the folder with the project and enter the following command sequence:
```
  mkdir build
  cd build 
  cmake .. && make
  sudo make install
```
After successfully compiling and building the project, the resulting program, including the Lua language launcher, 
will be installed in the directory /opt/Gorgona

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
