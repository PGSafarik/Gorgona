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
#ifndef __GORGONAWINDOW_H
#define __GORGONAWINDOW_H

/*************************************************************************
* GorgonaWindow.h                                                       *
*                                                                        *
* Hlavni okno aplikace                                                   *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
* ./FoxGHICP --name Gorgona --vendor GorgonaWindow                      * 
*************************************************************************/
#include<Gorgona.h>
#include<Boxes.h>
#include<FXGameItem.h>
#include<Perseus/Runnable.h>
#include<FXLaunchEditor.h>
#include<FXListPane.h>
#include<GO_Keywords.h>

class GorgonaWindow : public FXPrimaryWindow {
FXDECLARE( GorgonaWindow )
  Gorgona  *m_app;

  // Window Objects
  FindBar            *m_findbar;        // Vyhledavaci panel
  FXSwitcher         *gl_switcher;      // Prepinac aktivnich panelu
  FXListPane         *gl_pane;          // Panel seznamu polozek
  FXStatusBar        *gl_statusbar;     // Status bar
  IconsTheme         *gl_iconstheme;    // Tema icon
  FXIcon             *gl_iconFolder;    // Ikona slozky
  FXIcon             *gl_iconItem;      // Ikona obecne polozky

  /// FIXME GORGONA_WINDOW_006: Menu ma byt integrovano do vlastniho headerboxu. Presunout
  FXMenuPane *gl_mGorgona;
  FXMenuPane *gl_mGames;
  FXMenuPane *gl_mTools;
  FXMenuPane *gl_mHelp;

  // Settings
  FXbool    gl_hidegui;         // Skryt okno Gorgony
  FXString  gl_browser;         // Webovy prohlizec (Napoveda, web game client a web game distribution klient)
  FXString  gl_browser_args;    // Argumenty pro prohlizec
  FXString  gl_doubleclick_key; // Akce provedena pri dvojkliku na polozku
  FXString  gl_profile;         // Adresar s lokalnimi soubory Gorgony
  FXString  gl_view;            // S jakym typem seznamu aplikace nastaruje: list, icons
  FXString  gl_winmode;         // V jakem rezimu bude aplikace spustena: window, fullscreen, maximize
  FXPoint   gl_WinPos;          // Pozice okna - v okenim rezimu
  FXSize    gl_WinSize;         // Sizka okna - v okenim rezimu
  GO_KeyRoots gl_keywordsList;  // databaze klicovych slov a tagu

  /// FIXME GORGONA_WINDOW_005: Odstranit - moduly ma na starosti trida Gorgona
  // Moduly
  //- FXString gl_mlaunch_pth;
  //- FXString gl_toolkit_pth;
  //- FXbool   gl_mlaunch;

  // Application state
  FXbool      gl_change;            // Doslo ke zmenam v seznamech
  FXTreeItem *gl_gameroot;          // Koren herniho stromu

public :
  GorgonaWindow( Gorgona *app );
  virtual ~GorgonaWindow( );

  /* Access methods */
  virtual void create( );
  IconsTheme* get_IconsTheme( ) { return gl_iconstheme; }
  FXGameItem* get_ActiveItem( ) { return gl_pane->getCurrentItem( ); }

  /* Operations */
  virtual void layout( );

  /* Event messagess & handlers */
  enum {
    MAIN_ABOUT = FXPrimaryWindow::ID_LAST,
    MAIN_CONFIG,

    DATA_LOAD,         // Nacist seznam her
    DATA_SAVE,         // Ulozit seznam her
    DATA_SAVEAS,       // Ulozit senam her jako...
    DATA_CHANGED,      // Seznam her byl zmenen

    SYSTEM_RUN,        // Spustit hru

    CONF_SETUP,        // Nastaveni aplikace
    CONF_FOX,          // Nastaveni frameworku Fox

    LIST_EVENT,        //

    ID_LAST
  };
  long OnCmd_Main(FXObject *sender, FXSelector sel, void *data );
  long OnCmd_Data( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_System( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_Config( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_List( FXObject *sender, FXSelector sel, void *data );

protected :
  GorgonaWindow( ) { }

  void load( );
  void read_config( );
  void write_config( );
  void read_Keywords( const FXString &listfile, const FXString &rootname = "game" );
  void checkWindowState( );
  
  /* Process */
  /// FIXME GORGONA_WINDOW_007: Nebude lepsi vytvorit rovnou univerzalni event?
  FXbool run( FXGameItem *it = NULL );
};

#endif /* __GORGONAWINDOW_H */
/*** END ****************************************************************/
