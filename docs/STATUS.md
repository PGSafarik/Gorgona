# Project gorgon status and future plans 
    = 11/08/23 =
  
*Content:*  
  1. [News](#1-News)
  2. [Known bugs and flaws](#2-Known-bugs-and-flaws)
  3. [Plans for the future](#3-Plans-for-the-future)

---
## 1. News
#### 23/11/2024 - Update the FoxGHI default version 1.6.6.3
*IMPORTANT: Transition to a new version of the FoxGHI library (24.11 0.4.4.1 ALPHA). This version - among other things - allows client programs to detect the libFXGHI version directly, and is backward incompatible (at the source code level. Already compiled older versions of Gorgona should continue to work after the FoxGHI upgrade).

#### 13/05/2024 Oprava - 1.6.6.0
* FIXED - Fixed duplication of the "description" XML element in the game title library.
* FIXED - Fixed a bug with a bad return value returning a Lua function to directly start a process. 
* WORKAROUND - The Steam module temporarily allows the Steam client to run when loaded into memory. This will prevent Steam games from crashing when restarted.

#### 13/03/2023 update 2 - 1.6.0.0
* Reopen the project Gorgona. Removed the project status "Stopped"
* Realization of the PERSEUS internal module and redesign of the mechanism for launching new processes.
* The first changes in the internal mechanics of the Lua language API
* Rework of application initialization procedures
* Added module for Morfeus application
* Added option to run games from terminal emilator (xterm only for now)
* Adjustments in displaying the list of games
* The list game can now be started by double-clicking on the given item
* Gorgona registers the number of launches and the date of the last launch of the given game
* In the "details" view mode, the date of the last run and the genre of the game are also displayed
* Revision and revision of the basic user documentation (for now only in the Czech language). 
* Preparatory work for the implementation of new functionalities and minor adjustments

#### 11/01/2023 Update 1 - 1.5.0.1
* Default version of the foxlib is 1.7.81
* Gorgon's top-level windows are now created and managed by the FoxGHI library. The default version is 0.4.1  
* Toolbars are replaced by FoxGHI Header Boxes
* Modified the layout of top-level windows following the change in top-level window management
* Search moved from item list to Header Bar 
* Added a module to support launching games using the Steam client (for now, you need to manually search for the Steam app id and enter it in the command field)
* Added a module to support the launch of games installed using the Legendary tool (Epic games and GOG.com. Here too the game id must be entered manually)
* A simple outline of item validation. So far, it checks (and not quite correctly) validity against the existence of an executable file for native items. Entries that evaluate to invalid are written to stdout.
* XML tags representing individual game list items are now grouped into the "Library" xml tag. This change is currently incompatible with previous ones versions and the original data xml file will not load correctly. You need to fix it manually or create a new one. 
* Temporary deactivation of Gorgon's direct process launch from the LUA module. Of course, the standard os.execute() routine still works. 
* A few minor fixes and changes

## 2. Known bugs and flaws
  
## 3. Plans for the future  
### Project versioning notes

### Next development version: 1.7.X.X

### Next stable release: 2.0.0.0

### More distant future

---

    = END =
