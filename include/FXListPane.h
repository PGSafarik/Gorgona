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
#ifndef __FXGAMELIST_H
#define __FXGAMELIST_H

/*************************************************************************
* FXListPane.h                                                           *
*                                                                        *
* Panel zobrazujici herni polozky                                        *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<define.h>
#include<FXGameItem.h>
#include<FXLaunchEditor.h>


class FXListPane : public FXHorizontalFrame {
FXDECLARE( FXListPane )

  FXVerticalFrame *gl_folderframe;
  FXToolBar       *gl_folderbar;
  FXTreeList      *gl_foldertree; // Adresarova struktura sekci

  FXVerticalFrame *gl_listframe;
  FXToolBar       *gl_listbar;
  FXIconList      *gl_itemslist;  // Seznam polozek
  FXTextField     *gl_searchfield;

  FXTreeItem *gl_rootfd;    // Korenovy adresar
  FXTreeItem *gl_activefd;  // Aktualni adresar
  FXListItem *gl_currentit; // Aktualni polozka v seznamu

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


  IconsTheme  *gl_icth;
  FXObject    *gl_target;
  FXSelector   gl_selector;
  
  FXint m_actinum;     // Pocet aktualne zobrazovanych polozek
  FXint gl_nitems; 
  FXint gl_nfolders;


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

  //void showActiveFolder( FXbool sub = false ) { showFolder( ( ( gl_activefd != NULL ) ? gl_activefd : gl_rootfd ), sub ); }
  FXTreeItem* folder( const FXString &name = "Game list", FXTreeItem *parent = NULL );  // Vrati (vytvori) polozku ve stromu slozek


  ///////////////////////////////////////////////
  /// Access Methods
  ///
  FXint numItems( FXTreeItem *folder, FXbool sub = false );                         // Spocita polozky v zadane slozce
  FXint numItemsAll( ) { return gl_nitems; }
  FXint numFolders( FXTreeItem *folder, FXbool sub = false );                       // Spocita podslozky v zadane slozce
  FXint numFoldersAll( ) { return gl_nfolders; }
  FXint getItemList( FXTreeItem *ti, FXGameItemArray *buffer, FXbool sub = false );    // Vlozi do senamu polozky ze zadane slozky

  void    setOpenFolderIc( FXIcon *ic )   { ic_ofolder = ic;   }
  FXIcon* getOpenFolderIc( )              { return ic_ofolder; }
  void    setClosedFolderIc( FXIcon *ic ) { ic_cfolder = ic;   }
  FXIcon* getClosedFolderIc( )            { return ic_cfolder; }
  void    setBigItemIc( FXIcon *ic )      { ic_bitem = ic;     }
  FXIcon* getBigItemIc( )                 { return ic_bitem;   }
  void    setSmallItemIc( FXIcon *ic )    { ic_sitem = ic;     }
  FXIcon* getSmallItemIc( )               { return ic_sitem;   }

  FXGameItem* getCurrentItem( );
  FXTreeItem* getCurrentFolder( ) { return gl_foldertree->getCurrentItem( ); }
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
  FXlong Notify( FXuint type_message );
  FXlong SaveNotify( );
  /*
  inline FXIcon* loadExternalIcon( const FXString &ic_file, FXint width = 16, FXint height = 16 )
  {
     FXIcon *ic = NULL;
     FXFileStream fstr;

     if( fstr.open( ic_file ) == true ) {
       std::cout << "Open the external icon file " << ic_file.text( ) << std::endl;
       ic = new FXPNGIcon( getApp( ), NULL, 0, IMAGE_KEEP | IMAGE_OWNED | IMAGE_SHMI | IMAGE_ALPHACOLOR | IMAGE_ALPHAGUESS );
       if( ic != NULL ) {
         if( ic->loadPixels( fstr ) == true ) {
           std::cout << "Load the external file" << std::endl;
           ic->scale( width, height );
           ic->create( );
           //ic->restore( );
           //ic->render( );
         }
         else { std::cout << "NOT Loading the external file" << std::endl; }
       }
       fstr.close( );
     }
     return ic;
  }
*/
};


#endif
/*** END ****************************************************************/
