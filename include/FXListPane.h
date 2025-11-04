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
#pragma once
#include<define.h>

/*************************************************************************
* FXListPane.h                                                           *
*                                                                        *
* Panel zobrazujici herni polozky                                        *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
class FXListPane : public FXHorizontalFrame {
FXDECLARE( FXListPane )
  ::Gorgona *m_app;

  FXVerticalFrame *m_folderframe;
  FXToolBar       *m_folderbar;
  FXTreeList      *m_foldertree; // Adresarova struktura sekci

  FXVerticalFrame *m_listframe;
  FXToolBar       *m_listbar;
  FXIconList      *m_itemslist;  // Seznam polozek
  FXTextField     *m_searchfield;

  FXTreeItem *m_rootfd;    // Korenovy adresar
  FXTreeItem *m_activefd;  // Aktualni adresar
  FXListItem *m_currentit; // Aktualni polozka v seznamu

  FXIcon *ic_cfolder;   // Obecna ikona zavrene slozky (closed folder)
  FXIcon *ic_ofolder;   // Obecna ikona otevrene slozky (opened folder)
  FXIcon *ic_sitem;     // Obecna ikona polozky - mala (small item)
  FXIcon *ic_bitem;     // Obecna ikona polozky - velka (big item)
  FXIcon *ic_search;    // Ikona pro vyhledavani
  FXIcon *ic_tcolaps;
  FXIcon *ic_texpand;
  FXIcon *ic_ldetail;
  FXIcon *ic_licons;
  FXIcon *ic_ginsert;
  FXIcon *ic_gremove;
  FXIcon *ic_gedit;


  IconsTheme  *m_icth;
  FXObject    *m_target;
  FXSelector   m_selector;
  
  FXint m_actinum;     // Pocet aktualne zobrazovanych polozek
  FXint m_nitems; 
  FXint m_nfolders;
  
  FXString m_activeid; 

public :
  FXListPane( FXComposite *p, IconsTheme *icons, FXObject *tgt, FXSelector sel );
  virtual ~FXListPane( );

  ///////////////////////////////////////////////
  ///  Operations methods
  ///
  virtual void create( );

  void insertItem( FXGameItem *item );                                                  // Vlozit polozku do seznamu
  void removeItem( FXint id = -1 );
  void removeItem( FXGameItem *item );                                                  // Odebrat polozku ze seznamu
  void showItem( FXGameItem *item );                                                    // Zobrazi polozku v seznamu (nezavisle na stromu )
  void showFolder( FXTreeItem *folder, FXbool sub = false );                            // Zobrazi polozky vybrane slozky stromu
  void aktiveItem( FXint id = 0 );
  void signedItem( FXint id = 0, FXint state = 0 );                                     // Oznaci polozku

  //void showActiveFolder( FXbool sub = false ) { showFolder( ( ( m_activefd != NULL ) ? m_activefd : m_rootfd ), sub ); }
  FXTreeItem* folder( const FXString &name = "Game list", FXTreeItem *parent = NULL );  // Vrati (vytvori) polozku ve stromu slozek

  ///////////////////////////////////////////////
  /// Access Methods
  ///
  FXint numItems( FXTreeItem *folder, FXbool sub = false );                         // Spocita polozky v zadane slozce
  FXint numItemsAll( ) { return m_nitems; }
  FXint numFolders( FXTreeItem *folder, FXbool sub = false );                       // Spocita podslozky v zadane slozce
  FXint numFoldersAll( ) { return m_nfolders; }
  FXint getItemList( FXTreeItem *ti, FXGameItemArray *buffer, FXbool sub = false );    // Vlozi do senamu polozky ze zadane slozky

  void    setOpenFolderIc( FXIcon *ic )   { ic_ofolder = ic;   }
  FXIcon* getOpenFolderIc( )              { return ic_ofolder; }
  void    setClosedFolderIc( FXIcon *ic ) { ic_cfolder = ic;   }
  FXIcon* getClosedFolderIc( )            { return ic_cfolder; }
  void    setBigItemIc( FXIcon *ic )      { ic_bitem = ic;     }
  FXIcon* getBigItemIc( )                 { return ic_bitem;   }
  void    setSmallItemIc( FXIcon *ic )    { ic_sitem = ic;     }
  FXIcon* getSmallItemIc( )               { return ic_sitem;   }

  FXString    getActiveItemId( )          { return m_activeid; } 
  FXGameItem* getCurrentItem( );
  FXTreeItem* getCurrentFolder( ) { return m_foldertree->getCurrentItem( ); }
  ///////////////////////////////////////////////
  ///
  ///
  enum {
    TREE_SELECT = FXHorizontalFrame::ID_LAST,
    TREE_COLAPS,
    TREE_EXPAND,
    ITEM_SELECT,
    LIST_REFRESH,
    LIST_DETAIL,
    LIST_ICONS,
    LIST_FIND,
    GAME_INSERT,
    GAME_REMOVE,
    GAME_EDIT,
    ID_LAST
  };
  long OnCmd_tree( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_item( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_list( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_game( FXObject *sender, FXSelector sel, void *data );

protected :
  FXListPane( ) { }

  ///////////////////////////////////////////////
  ///
  ///
  void erase_f( FXGameItem *item ); // Funkce odstrani polozku z dat slozky
  FXlong Notify( FXuint type_message, void *data = NULL );
  void SaveNotify( ); // !
};

/*** END ****************************************************************/
