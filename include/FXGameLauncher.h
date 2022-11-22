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
#ifndef __FXGAMELAUNCHER_H
#define __FXGAMELAUNCHER_H

/*************************************************************************
* FXGameLauncher.h                                                       *
*                                                                        *
* Hlavni okno aplikace                                                   *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
* ./FoxGHICP --name Gorgona --vendor FXGameLauncher                      * 
*************************************************************************/
// Syscalls and system types
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

// The LUA interpreter
#include<lua5.1/lualib.h>
#include<lua5.1/lauxlib.h>

// Foxlib, the GUI library
#include<fx.h>
#include<fxghi.h>

// Tiny xml - xml support
#include<tinyxml.h>

// Internal headers
#include<Utils.h>
#include<IconsTheme.h>
#include<FXGameItem.h>
#include<GorgonaProcess/Process.h>
#include<FXLaunchEditor.h>
#include<FXLauncherLua.h>
#include<FXListPane.h>
#include<GO_Keywords.h>

#include<../version.h>


class FXGameLauncher : public FXPrimaryWindow {
FXDECLARE( FXGameLauncher )
  // Window Objects
  //FXMenuBar          *gl_menubar;       // Menu
  FXSwitcher         *gl_switcher;      // Prepinac aktivnich panelu
  FXListPane         *gl_pane;          // Panel seznamu polozek
  FXArray<FXLabel*>   gl_actions;       // Seznam tlacitek
  FXText             *gl_text;          // Pole pro popisek
  FXStatusBar        *gl_statusbar;     // Status bar
  IconsTheme         *gl_iconstheme;    // Tema icon
  FXIcon             *gl_iconFolder;    // Ikona slozky
  FXIcon             *gl_iconItem;      // Ikona obecne polozky

  FXMenuPane *gl_mGorgona;
  FXMenuPane *gl_mGames;
  FXMenuPane *gl_mTools;
  //FXMenuPane *gl_mView;
  FXMenuPane *gl_mHelp;

  // Settings
  FXbool    gl_autosave;        // Automaticky ukladat zmeny
  FXbool    gl_hidegui;         // Skryt okno Gorgony
  FXString  gl_browser;         // Webovy prohlizec (Napoveda, web game client a web game distribution klient)
  FXString  gl_browser_args;    // Argumenty pro prohlizec
  FXString  gl_doubleclick_key; // Akce provedena pri dvojkliku na polozku
  FXString  gl_datafile;        // Plna cesta k indexacnimu souboru
  FXString  gl_profile;         // Adresar s lokalnimi soubory Gorgony
  FXString  gl_gamelist;        // Nazev indexacniho xml souboru
  FXString  gl_view;            // S jakym typem seznamu aplikace nastaruje: list, icons
  FXString  gl_winmode;         // V jakem rezimu bude aplikace spustena: window, fullscreen, maximize
  FXPoint   gl_WinPos;          // Pozice okna - v okenim rezimu
  FXSize    gl_WinSize;         // Sizka okna - v okenim rezimu
  GO_KeyRoots gl_keywordsList;  // databaze klicovych slov a tagu

  // Moduly
  FXString gl_mlaunch_pth;
  FXString gl_toolkit_pth;
  FXbool   gl_mlaunch;

  // Application state
  FXbool      gl_created;           // Indikkuje vytvoreni okna na strane serveru. Nutne kvuli kontrole statusu okna.
  FXbool      gl_change;            // Doslo ke zmenam v seznamech
  FXTreeItem *gl_gameroot;          // Koren herniho stromu

  // Zprava podprocesu
  FXint       gl_childPID;          // PID potomka, ktery byl prave ukoncen
  FXint       gl_childRV;           // Navratova hodnota ukonceneho potomka
  //FXGameProcessList gl_processlist; // Seznam se spustenymi procesy-potomky

  // LUA
  FXString   gl_lscript;

public :
  FXGameLauncher( FXApp *a );
  virtual ~FXGameLauncher( );

  ////////////////////////////////////////////////
  ///
  ///
  virtual void create( );
  IconsTheme* get_IconsTheme( ) { return gl_iconstheme; }
  FXGameItem* get_ActiveItem( ) { return gl_pane->getCurrentItem( ); }

  ///////////////////////////////////////////////
  /// Operace
  ///
  FXbool exec( const FXArray<const FXchar*> &args, FXbool wait = true, FXbool ver = false );
  virtual void layout( );

  ////////////////////////////////////////////////
  ///
  ///
  enum {
    MAIN_ABOUT = FXPrimaryWindow::ID_LAST,
    MAIN_CONFIG,

    DATA_LOAD,         // Nacist seznam her
    DATA_SAVE,         // Ulozit seznam her
    DATA_SAVEAS,       // Ulozit senam her jako...
    DATA_CHANGED,      // Seznam her byl zmenen

    SYSTEM_RUN,        // Spustit hru
    SIGNAL_CHLD,        // Hra je ukoncen (skoncil proces potomka)

    CONF_SETUP,        // Nastaveni aplikace
    CONF_FOX,          // Nastaveni frameworku Fox

    LIST_EVENT,        //

    ID_LAST
  };
  long OnCmd_Main(FXObject *sender, FXSelector sel, void *data );
  long OnCmd_Data( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_System( FXObject *sender, FXSelector sel, void *data );
  long OnSig_ExitChild( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_Config( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_List( FXObject *sender, FXSelector sel, void *data );

protected :
  FXGameLauncher( ) { }

  void load( );
  void save( );
  void lua_init( );
  void read_config( );
  void write_config( );
  void read_Keywords( const FXString &listfile, const FXString &rootname = "game" );
  FXbool run( FXGameItem *it = NULL );
  FXbool __exec( );
  void get_arguments( StringList *list );
  void read_argument( );
  static FXbool parse_params( FXArray<const FXchar*> *buffer, const FXString &str, FXbool dump = false );
  void checkWindowState( );
  FXint WaitOnGame( FXProcess *proc );
  void version( );
};

#endif /* __FXGAMELAUNCHER_H */
/*** END ****************************************************************/
