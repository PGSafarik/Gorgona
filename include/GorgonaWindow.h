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
* GorgonaWindow.h                                                        *
*                                                                        *
* Main window                                                            *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
* ./FoxGHICP --name Gorgona --vendor GorgonaWindow                       * 
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
  FindBar            *m_findbar;       // Vyhledavaci panel
  FXSwitcher         *m_switcher;      // Prepinac aktivnich panelu
  FXListPane         *m_pane;          // Panel seznamu polozek
  FXStatusBar        *m_statusbar;     // Status bar
  IconsTheme         *m_iconstheme;    // Tema icon
  FXIcon             *m_iconFolder;    // Ikona slozky
  FXIcon             *m_iconItem;      // Ikona obecne polozky

  /// FIXME GORGONA_WINDOW_006: Menu ma byt integrovano do vlastniho headerboxu. Presunout
  FXMenuPane *m_mGorgona;
  FXMenuPane *m_mGames;
  FXMenuPane *m_mTools;
  FXMenuPane *m_mHelp;

  // Settings
  FXbool    m_hidegui;         // Skryt okno Gorgony
  FXString  m_browser;         // Webovy prohlizec (Napoveda, web game client a web game distribution klient)
  FXString  m_browser_args;    // Argumenty pro prohlizec
  FXString  m_doubleclick_key; // Akce provedena pri dvojkliku na polozku
  FXString  m_profile;         // Adresar s lokalnimi soubory Gorgony
  FXString  m_view;            // S jakym typem seznamu aplikace nastaruje: list, icons
  FXString  m_winmode;         // V jakem rezimu bude aplikace spustena: window, fullscreen, maximize
  FXPoint   m_WinPos;          // Pozice okna - v okenim rezimu
  FXSize    m_WinSize;         // Sirka okna - v okenim rezimu
  GO_KeyRoots m_keywordsList;  // databaze klicovych slov a tagu

  // Application state
  FXbool      m_change;            // Doslo ke zmenam v seznamech
  FXTreeItem *m_gameroot;          // Koren herniho stromu

public :
  GorgonaWindow( Gorgona *app );
  virtual ~GorgonaWindow( );

  /* Access methods */
  virtual void create( );
  IconsTheme* get_IconsTheme( ) { return m_iconstheme; }
  //FXGameItem* get_ActiveItem( ) { return m_pane->getCurrentItem( ); }

  /* Operations */
  virtual void layout( );

  /* Event messagess & handlers */
  enum {
    MAIN_ABOUT = FXPrimaryWindow::ID_LAST,
    MAIN_CONFIG,
    SYSTEM_RUN,        // Spustit hru
    CONF_SETUP,        // Nastaveni aplikace
    CONF_FOX,          // Nastaveni frameworku Fox
    LIST_EVENT,        //
    ID_LAST
  };
  long OnCmd_Main(FXObject *sender, FXSelector sel, void *data );
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
