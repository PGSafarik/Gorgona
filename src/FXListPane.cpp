// FXListPane.cpp Copyright (c) 24/03/2019;  D.A.Tiger; GNU GPL 3
#include<FXListPane.h>
#include<FXGameLauncher.h>

FXDEFMAP( FXListPane ) LISTPANE_MAP[ ] = {
  FXMAPFUNC( SEL_COMMAND, FXListPane::TREE_SELECT, FXListPane::OnCmd_tree ),
  FXMAPFUNC( SEL_COMMAND, FXListPane::TREE_COLAPS, FXListPane::OnCmd_tree ),
  FXMAPFUNC( SEL_COMMAND, FXListPane::TREE_EXPAND, FXListPane::OnCmd_tree ),
  FXMAPFUNC( SEL_COMMAND, FXListPane::ITEM_SELECT, FXListPane::OnCmd_item ),
//  FXMAPFUNC( SEL_COMMAND, FXListPane::LIST_REFRESH, FXListPane::OnCmd_list ),
//  FXMAPFUNC( SEL_COMMAND, FXListPane::LIST_DETAIL, FXListPane::OnCmd_list ),
//  FXMAPFUNC( SEL_COMMAND, FXListPane::LIST_ICONS, FXListPane::OnCmd_list ),
  FXMAPFUNCS( SEL_COMMAND, FXListPane::LIST_REFRESH, FXListPane::LIST_FIND, FXListPane::OnCmd_list ),
  FXMAPFUNCS( SEL_COMMAND, FXListPane::GAME_INSERT, FXListPane::GAME_EDIT, FXListPane::OnCmd_game )
};

FXIMPLEMENT( FXListPane, FXHorizontalFrame, LISTPANE_MAP, ARRAYNUMBER( LISTPANE_MAP ) )

/*************************************************************************************************/
FXListPane::FXListPane( FXComposite *p, IconsTheme *icons, FXObject *tgt, FXSelector sel )
          : FXHorizontalFrame( p, FRAME_GROOVE | LAYOUT_FILL, 0, 0, 0, 0,  DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING,  1, 1 )
{
  gl_icth    = icons;
  ic_search  = gl_icth->getIcon( "Actions/system-search.png" );
  ic_tcolaps = gl_icth->getIcon( "Actions/view-left-close.png" );
  ic_texpand = gl_icth->getIcon( "Actions/view-list-tree.png" );
  ic_ldetail = gl_icth->getIcon( "Actions/view-list-details.png" );
  ic_licons  = gl_icth->getIcon( "Actions/view-list-icons.png" );
  ic_ginsert = gl_icth->getIcon( "Actions/list-add.png" );
  ic_gremove = gl_icth->getIcon( "Actions/list-remove.png" );
  ic_gedit   = gl_icth->getIcon( "Actions/document-edit.png" );

  // Game Tree
  gl_folderframe = new FXVerticalFrame( this, FRAME_LINE | LAYOUT_FILL_Y, 0, 0, 0, 0,  1, 1, 1, 1,  0, 0 );
  gl_foldertree  = new FXTreeList( gl_folderframe, this, FXListPane::TREE_SELECT, TREELIST_SHOWS_BOXES | TREELIST_SHOWS_LINES | LAYOUT_FILL_Y| LAYOUT_FIX_WIDTH, 0, 0, 200, 0 );
  gl_foldertree->setSortFunc( FXTreeList::ascending );
  /*
  new FXSeparator( gl_folderframe, SEPARATOR_GROOVE|LAYOUT_FILL_X );
  gl_folderbar   = new FXToolBar( gl_folderframe, FRAME_NONE | LAYOUT_FILL_X );
  new FXButton( gl_folderbar, "\t\t Rozbalit strom", ic_texpand, this, FXListPane::TREE_EXPAND, BUTTON_TOOLBAR | FRAME_RAISED );
  new FXButton( gl_folderbar, "\t\t Sbalit strom", ic_tcolaps, this, FXListPane::TREE_COLAPS, BUTTON_TOOLBAR | FRAME_RAISED );
  */
  // Game list
  gl_listframe = new FXVerticalFrame( this, FRAME_LINE | LAYOUT_FILL, 0, 0, 0, 0,  1, 1, 1, 1,  0, 0 );
  gl_itemslist = new FXIconList( gl_listframe, this, FXListPane::ITEM_SELECT, LAYOUT_FILL_X | LAYOUT_FILL_Y | ICONLIST_DETAILED/* ICONLIST_BIG_ICONS */ | ICONLIST_EXTENDEDSELECT |  ICONLIST_BROWSESELECT );
  new FXSeparator( gl_listframe, SEPARATOR_GROOVE|LAYOUT_FILL_X );
  gl_listbar   = new FXToolBar( gl_listframe, FRAME_NONE | LAYOUT_FILL_X );
  gl_itemslist->appendHeader("Titul",    NULL, 350);
  gl_itemslist->appendHeader("Spusteno", NULL, 100);
  gl_itemslist->appendHeader("Naposled", NULL, 125);
  gl_itemslist->setSortFunc( FXIconList::ascending );
  /*
  new FXButton( gl_listbar, "\t\t Detaily", ic_ldetail, this, FXListPane::LIST_DETAIL, BUTTON_TOOLBAR | FRAME_RAISED );
  new FXButton( gl_listbar, "\t\t Ikony", ic_licons, this, FXListPane::LIST_ICONS, BUTTON_TOOLBAR | FRAME_RAISED );
  new FXSeparator( gl_listbar, SEPARATOR_GROOVE|LAYOUT_FILL_Y );
  new FXButton( gl_listbar, "\t\t Pridat novou hru", ic_ginsert, this, FXListPane::GAME_INSERT, BUTTON_TOOLBAR | FRAME_RAISED );
  new FXButton( gl_listbar, "\t\t Editovat existujici hru", ic_gedit, this, FXListPane::GAME_EDIT, BUTTON_TOOLBAR | FRAME_RAISED );
  new FXButton( gl_listbar, "\t\t Odstarnit hru ze seznamu", ic_gremove, this, FXListPane::GAME_REMOVE, BUTTON_TOOLBAR | FRAME_RAISED );
  */
  // search
  FXHorizontalFrame *searchbar = new FXHorizontalFrame( gl_listbar, FRAME_NONE | LAYOUT_RIGHT | LAYOUT_SIDE_RIGHT, 0, 0, 0, 0,  1, 1, 1, 1,  1, 0  );
  //new FXLabel( searchbar, " ", ic_search, LABEL_NORMAL  );
  new FXButton( searchbar, "\t\t Hledat", ic_search, this, FXListPane::LIST_FIND, BUTTON_TOOLBAR | FRAME_RAISED );
  gl_searchfield = new FXTextField( searchbar, 25, NULL, 0, TEXTFIELD_NORMAL );

  gl_rootfd   = NULL;
  gl_activefd = NULL;

  ic_ofolder = NULL;
  ic_cfolder = NULL;
  ic_bitem   = NULL;
  ic_sitem   = NULL;

  gl_nitems   = gl_nfolders = 0;
  gl_target   = tgt;
  gl_selector = sel;
}

FXListPane::~FXListPane( )
{
  delete gl_foldertree;
  delete gl_itemslist;
}

/*************************************************************************************************/
void FXListPane::create( )
{
  FXHorizontalFrame::create( );
  gl_foldertree->create( );
  gl_itemslist->create( );

  if( ic_ofolder != NULL ) { ic_ofolder->create( ); }
  if( ic_cfolder != NULL ) { ic_cfolder->create( ); }
  if( ic_bitem   != NULL ) { ic_bitem->create( ); }
  if( ic_sitem   != NULL ) { ic_sitem->create( ); }
  //if( ic_search   != NULL ) { ic_search->create( ); }
}

FXGameItem* FXListPane::getCurrentItem( )
{
  FXGameItem* it = NULL;
  FXint pos = gl_itemslist->getCurrentItem( );
  if( pos >= 0 ) { it = ( FXGameItem* ) gl_itemslist->getItem( pos )->getData( ); }

  return it;
}

void FXListPane::insertItem( FXGameItem *item )
{
  FXTreeItem *treeit;
  FXString name = FXString::null;

  // Vyhledani, vytvoreni, nebo urceni odpovidajici slozky
  if( item != NULL ) {
    name = item->read( "Basic:genre" );
    if( name.empty( ) == true ) {
      if( ( treeit = gl_foldertree->getCurrentItem( ) ) == NULL ) { treeit = gl_rootfd; }
      name = treeit->getText( );
      item->write( "Basic:genre", name );
    } else { treeit = folder( name ); }

    // Vlozeni polozky do seznamu ve slozce
    ( ( FXGameItemArray* )treeit->getData( ) )->push( item );
    gl_nitems++;
  }
}

void FXListPane::removeItem( FXint id )
{
  FXGameItem *data;
  FXint pos = ( ( id >= 0 ) ? id : gl_itemslist->getCurrentItem( ) );
  if( pos <= gl_itemslist->getNumItems( ) ) {
    data = ( FXGameItem* ) gl_itemslist->getItem( pos )->getData( );
    std::cout << "Removing item " << data->read( "Basic:title" ).text( ) << std::endl;
    std::cout << "From items list: id = " << pos << std::endl;
    gl_itemslist->removeItem( pos );
    erase_f( data );
    if( data != NULL ) {
      std::cout << "From memory" << std::endl;
      data->clear( FXString::null );
      delete data;
    }
    data = NULL;
  }
  std::cout.flush( );
}

void FXListPane::erase_f( FXGameItem *item )
{
  FXTreeItem *f = folder( item->read( "Basic:genre" ) );
  FXGameItemArray *l = ( FXGameItemArray* ) f->getData( );
  FXint pos;

  for( pos = 0; pos != l->no( ); pos++ ) {
    if( item == l->at( pos ) ) {
      l->erase( pos );
      std::cout << "From folder: id = " << pos << std::endl;
      break;
    }
  }
}

void FXListPane::showItem( FXGameItem *item )
{
  FXint index  = -1;
  FXString sep = "\t";
  FXIcon *ic_small, *ic_big;
  FXString text = item->read( "Basic:title" );
  text += sep + FXString::value( item->nop );
  text += sep + item->read( "Extra:lastPlayed" );
  ic_small = ic_big = NULL;
  ic_small = ( ( item->SmallIcon == NULL ) ? ic_sitem : item->SmallIcon );
  ic_big   = ( ( item->BigIcon == NULL ) ? ic_bitem : item->BigIcon );

  if( ( index = gl_itemslist->findItem( text ) ) < 0 ) {
    gl_itemslist->appendItem( text, ic_big, ic_small, item );
  }
  else { gl_itemslist->setItemText( index, text ); }
}

void FXListPane::showFolder( FXTreeItem *__folder, FXbool sub )
{
  FXTreeItem *tit = ( ( __folder == NULL ) ? gl_rootfd : __folder );
  if( tit != NULL  ) {
    FXGameItemArray *itl = ( FXGameItemArray* )tit->getData( );
    FXint num = itl->no( );
    for( FXint i = 0; i != num; i++ ) {
      FXGameItem *item = itl->at( i );
      showItem( item );
    }

    if( sub == true ) {
      FXTreeItem *f = tit->getFirst( );
      while( f != NULL ) {
        showFolder( f, sub );
        f = f->getNext( );
      }
    }
    gl_itemslist->sortItems( );
  }
}

FXTreeItem* FXListPane::folder( const FXString &name, FXTreeItem *parent )
{
  FXArray<FXString>  t_buff;
  FXTreeItem        *resh = NULL;
  FXTreeItem        *p    = ( ( parent != NULL )? parent : gl_rootfd );

  split( name, &t_buff );       // Vyhledani subretezcu odpovidajicich subslozkam
  if( t_buff.no( ) == 0 ) {     // Jmeno neobsahuje zadne subslozky
    resh = gl_foldertree->findItem( name, p );
    if( resh == NULL ) {
      resh = gl_foldertree->appendItem( p, new FXTreeItem( name, ic_ofolder, ic_cfolder, new FXGameItemArray ) );
      gl_nfolders++;
    }
  }
  else {
    //FXTreeItem *n = NULL;
    FXTreeItem *f = NULL;
    for( FXint i = 0; i != t_buff.no( ); i++ ) {
      FXString sect = t_buff[ i ];
      f = gl_foldertree->findItem( sect, p );
      if( f == NULL ) {
        p = gl_foldertree->appendItem( p, new FXTreeItem( sect, ic_ofolder, ic_cfolder, new FXGameItemArray ) );
        gl_nfolders++;
      }
      else { p = f; }
    }
    resh = p;
  }
  /// FIXME : SMAZAT
  if( ( parent == NULL ) && ( gl_rootfd == NULL ) ) {
    gl_rootfd = resh;
    if( gl_activefd == NULL ) { gl_activefd = gl_rootfd; }
  }
  return resh;
}

void FXListPane::aktiveItem( FXint id )
{
  if( (id >= 0 ) && ( id < gl_itemslist->getNumItems( ) ) ) {
    gl_itemslist->killSelection( );
    gl_itemslist->setCurrentItem( id );
    gl_itemslist->selectItem( id );
    gl_itemslist->makeItemVisible( id );
  }
}

/*************************************************************************************************/
FXint FXListPane::numItems( FXTreeItem *folder, FXbool sub )
{

  return 0;
}

FXint FXListPane::numFolders( FXTreeItem *folder, FXbool sub )
{

  return 0;
}

FXint FXListPane::getItemList( FXTreeItem *ti, FXGameItemArray *buffer, FXbool sub )
{
  FXint resh = 0;
  FXTreeItem *trit = ( ( ti == NULL )? gl_rootfd : ti );


  if( ( trit != NULL ) && ( buffer != NULL) ) {
    FXGameItemArray *itl = ( FXGameItemArray* ) trit->getData( );
    if( ( resh = itl->no( ) ) > 0 ) { buffer->append( itl->data( ), resh ); }
  }

  if( sub == true ) {
    FXTreeItem *f = trit->getFirst( );
    while( f != NULL ) {
      resh += this->getItemList( f, buffer, sub );
      f = f->getNext( );
    }
  }

  return resh;
}



/*************************************************************************************************/
long FXListPane::OnCmd_tree( FXObject *sender, FXSelector sel, void *data )
{
  FXTreeItem *trit = ( FXTreeItem* )data;
  long        resh = 1;

  switch( FXSELID( sel ) )  {
    case FXListPane::TREE_SELECT : {
      gl_itemslist->clearItems( );
      showFolder( trit, ( ( trit == gl_rootfd ) ? true : false ) );
      gl_itemslist->sortItems( );
      gl_activefd = trit;
      break;
    }
    case FXListPane::TREE_COLAPS : {
      gl_foldertree->collapseTree( gl_rootfd );
      break;
    }
    case FXListPane::TREE_EXPAND : {
      gl_foldertree->expandTree( gl_rootfd );
      break;
    }
  }
  return resh;
}

long FXListPane::OnCmd_item( FXObject *sender, FXSelector sel, void *data )
{
  FXuint mtype = FXSELTYPE( sel );

  switch( mtype ) {
    case SEL_DOUBLECLICKED : {
      std::cout << "[FXListPane] Item event doubleclick" << std::endl;
      if( gl_target ) { gl_target->handle( this, FXSEL( SEL_COMMAND, FXGameLauncher::SYSTEM_RUN ), NULL ); }
      break;
    }
    default : {
      if( gl_target != NULL ) {
        std::cout << "[FXListPane] Item event other" << std::endl;
        gl_target->handle( this, FXSEL( mtype, gl_selector), NULL );
      }
    }
  }
  return 1;
}

long FXListPane::OnCmd_list( FXObject *sender, FXSelector sel, void *data )
{
  long resh = 0;

  switch( FXSELID( sel ) ) {
    case FXListPane::LIST_REFRESH :
    {
      std::cout << "Refresh list..." << std::endl;
      gl_itemslist->clearItems( );
      showFolder( ( ( gl_activefd != NULL ) ? gl_activefd : gl_rootfd ), ( ( gl_activefd == gl_rootfd )? true : false ) );
      gl_currentit = NULL;
      resh = 1;
      break;
    }
    case FXListPane::LIST_DETAIL :
    {
      gl_itemslist->setListStyle( ICONLIST_DETAILED );
      break;
    }
    case FXListPane::LIST_ICONS :
    {
      gl_itemslist->setListStyle( ICONLIST_BIG_ICONS );
      break;
    }
    case FXListPane::LIST_FIND :
    {
      FXint    pos  = -1;
      //FXint    curr = gl_itemslist->getCurrentItem( );

      FXString text = gl_searchfield->getText( );
      if( !text.empty( ) ) {
        pos = gl_itemslist->findItem( text );
        if( pos > -1 ) { /*/ Nalezeno
          gl_itemslist->killSelection( );
          gl_itemslist->setCurrentItem( pos );
          gl_itemslist->selectItem( pos );
          gl_itemslist->makeItemVisible( pos );
          */
          aktiveItem( pos );
        }
        else {
          FXString _msg = "Nelze najit polozku s pozadovanym textem ";
          FXMessageBox::information( this, MBOX_OK, "Vyhledavani", ( _msg + text ).text( ) );
        }
        gl_searchfield->setText( FXString::null );
      }
      else { FXMessageBox::question( this, MBOX_OK, "EHM...", "Coze to mam hledat?" ); }
      break;
    }

  }

  return resh;
}

long FXListPane::OnCmd_game( FXObject *sender, FXSelector sel, void *data )
{
  long resh = 0;
  FXuint _placement = PLACEMENT_SCREEN;
  FXSelector __sel = FXSEL( SEL_CHANGED, gl_selector );
  FXGameItem *item;

  switch( FXSELID( sel ) ) {
    case FXListPane::GAME_INSERT :
    {
      //gl_currentit = NULL;
      FXString name;
      if( FXInputDialog::getString( name, this, "Pridat novou hru do zbirky", "Zadejte nazev pridavaneho titulu: " ) == true ) {
        item = new FXGameItem( name );
        FXTreeItem *f = getCurrentFolder( );
        if( f ) { item->write( "Basic:genre", f->getText( ) ); }
        FXLaunchEditor *edit = new FXLaunchEditor( this, gl_icth, item );
        if( edit->execute( _placement ) == true ) {
          this->insertItem( item );
          this->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_REFRESH ), NULL );
          gl_target->handle( this, __sel, NULL );
          resh = 1;
        }
      }
      break;
    }
    case FXListPane::GAME_EDIT :
    {
      if( ( item = getCurrentItem( ) ) != NULL ) {
        FXLaunchEditor *edit = new FXLaunchEditor( this, gl_icth, item );
        if( edit->execute( _placement ) == true ) {
          this->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_REFRESH ), NULL );
          gl_target->handle( this, __sel, NULL );
          resh = 1;
        }
      }
      break;
    }
    case FXListPane::GAME_REMOVE :
    {
      if( ( item = getCurrentItem( ) ) != NULL ) {
        FXString msg = "Opravdu si prejete titul ";
        msg += item->read( "Basic:title" ) + " odstranit ze senamu ?";
        if( FXMessageBox::question( this, MBOX_YES_NO, "Smazat titul: ", msg.text( )  ) == MBOX_CLICKED_YES ) {
          this->removeItem( );
          this->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_REFRESH ), NULL );
          gl_target->handle( this, __sel, NULL );
          resh = 1;
        }
      }
      break;
    }

  }

  return resh;
}

/*** END ******************************************************************************************/
