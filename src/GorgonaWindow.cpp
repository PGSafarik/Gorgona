// GorgonaWindow.cpp Copyright (c) %date%;  D.A.Tiger; GNU GPL 3
#include<GorgonaWindow.h>

FXDEFMAP( GorgonaWindow ) LAUNCHERMAP[ ] = {
  FXMAPFUNC( SEL_COMMAND,   GorgonaWindow::SYSTEM_RUN,  GorgonaWindow::OnCmd_System ),
  FXMAPFUNC( SEL_CHANGED,   GorgonaWindow::LIST_EVENT,  GorgonaWindow::OnCmd_List ),
  FXMAPFUNC( SEL_CONFIGURE, GorgonaWindow::MAIN_CONFIG, GorgonaWindow::OnCmd_Main ),
  FXMAPFUNC( SEL_COMMAND,   GorgonaWindow::CONF_SETUP,  GorgonaWindow::OnCmd_Config ),
  FXMAPFUNC( SEL_COMMAND,   GorgonaWindow::CONF_FOX,    GorgonaWindow::OnCmd_Config ),
};

FXIMPLEMENT( GorgonaWindow, FXPrimaryWindow, LAUNCHERMAP, ARRAYNUMBER( LAUNCHERMAP ) )

/**************************************************************************************************/
GorgonaWindow::GorgonaWindow( Gorgona *app )
              : FXPrimaryWindow( app, "Gorgona", NULL, NULL, WINDOW_MAIN | CONTROLS_NORMAL, 0, 0, 800, 700 ) // (Fox Game Launcher - BETA.00.01)
{ 
  m_app = app; 
 
  gl_iconstheme = new IconsTheme( getApp( ), "/usr/share/icons/oxygen/base/" );
  gl_iconstheme->insert( "Actions", "16x16/actions" );
  gl_iconstheme->insert( "Actions_big", "32x32/actions" );
  gl_iconstheme->insert( "Places", "16x16/places" );
  gl_iconstheme->insert( "Status", "16x16/status" );
  FXIcon *ic_run    = gl_iconstheme->getIcon( "Actions/system-run.png" );
  if( !ic_run ) { std::cout << "Icon nothing" << std::endl; }

  FXVerticalFrame *contend = new FXVerticalFrame( this, FRAME_NONE | LAYOUT_FILL );

  // Create Pane Switcher
  gl_switcher = new FXSwitcher( contend, FRAME_NONE | LAYOUT_FILL, 0, 0, 0, 0,  0, 0, 0, 0 );

  // Create the item list
  gl_pane = new FXListPane( gl_switcher, gl_iconstheme, this, GorgonaWindow::LIST_EVENT );
  gl_pane->setClosedFolderIc( gl_iconstheme->getIcon( "Places/folder.png" ) );
  gl_pane->setOpenFolderIc(   gl_iconstheme->getIcon( "Status/folder-open.png" ) );
  gl_pane->setSmallItemIc(    gl_iconstheme->getIcon( "Actions/roll.png" ) );
  gl_pane->setBigItemIc(      gl_iconstheme->getIcon( "Actions_big/roll.png" ) );
  gl_pane->folder( "Categories"/*Genre List"*/ );

  // Status bar
  gl_statusbar = new FXStatusBar( contend, FRAME_NONE | LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X );
  gl_change = false;

  // Aplication menu
  FXMenuPane *gl_menu = new FXMenuPane( this );
  gl_mGorgona = new FXMenuPane( gl_menu );
    new FXMenuCommand( gl_mGorgona, "Pohled Detaily", gl_iconstheme->getIcon( "Actions/view-list-details.png" ), gl_pane, FXListPane::LIST_DETAIL );
    new FXMenuCommand( gl_mGorgona, "Pohled Ikony", gl_iconstheme->getIcon( "Actions/view-list-icons.png" ), gl_pane, FXListPane::LIST_ICONS );
    new FXMenuSeparator( gl_mGorgona, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( gl_mGorgona, "Rozbalit strom", gl_iconstheme->getIcon( "Actions/view-list-tree.png" ), gl_pane, FXListPane::TREE_EXPAND );
    new FXMenuCommand( gl_mGorgona, "Sbalit strom", gl_iconstheme->getIcon( "Actions/view-left-close.png" ), gl_pane, FXListPane::TREE_COLAPS );
    new FXMenuSeparator( gl_mGorgona, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( gl_mGorgona, "Volby", gl_iconstheme->getIcon( "Actions/configure.png" ), this, GorgonaWindow::CONF_SETUP );
    new FXMenuCommand( gl_mGorgona, "Fox toolkit", gl_iconstheme->getIcon( "Actions/code-class.png" ), this, GorgonaWindow::CONF_FOX );
    new FXMenuSeparator( gl_mGorgona, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( gl_mGorgona, "Ukoncit", gl_iconstheme->getIcon( "Actions/window-close.png" ), getApp( ), FXApp::ID_QUIT );
  new FXMenuCascade( gl_menu, "Gorgona", NULL, gl_mGorgona );
  gl_mGames = new FXMenuPane( gl_menu );
    new FXMenuCommand( gl_mGames, "Spustit", gl_iconstheme->getIcon( "Actions/system-run.png" ), this, GorgonaWindow::SYSTEM_RUN );
    new FXMenuSeparator( gl_mGames, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( gl_mGames, "Pridat",  gl_iconstheme->getIcon( "Actions/list-add.png" ),    gl_pane, FXListPane::GAME_INSERT );
    new FXMenuCommand( gl_mGames, "Editovat",  gl_iconstheme->getIcon( "Actions/document-edit.png" ),    gl_pane,  FXListPane::GAME_EDIT );
    new FXMenuCommand( gl_mGames, "Odebrat",  gl_iconstheme->getIcon( "Actions/list-remove.png" ),    gl_pane, FXListPane::GAME_REMOVE );
    new FXMenuSeparator( gl_mGames, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( gl_mGames, "Ulozit",  gl_iconstheme->getIcon( "Actions/document-save.png" ), m_app, Gorgona::SAVE_LIBRARY );
  new FXMenuCascade( gl_menu, "Library", NULL, gl_mGames );
  gl_mTools = new FXMenuPane( gl_menu );
  new FXMenuCascade( gl_menu, "Nastroje", NULL, gl_mTools );
  new FXMenuSeparator( gl_menu, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
  gl_mHelp = new FXMenuPane( gl_menu );
  new FXMenuCascade( gl_menu, "Napoveda", NULL, gl_mHelp );
  new FXMenuCommand( gl_menu, "O aplikaci", NULL, getApp( ), FXApp::ID_QUIT );

  new MainBar( this, gl_iconstheme, gl_menu );

  /* Tool bars */ 
  ToolBar *mb = new ToolBar( this, gl_iconstheme );
  mb->makeButton( "\t\tPridat",  "Actions_big/list-add.png",      gl_pane, FXListPane::GAME_INSERT    );
  mb->makeButton( "\t\tUpravit", "Actions_big/document-edit.png", gl_pane, FXListPane::GAME_EDIT      );
  mb->makeButton( "\t\tOdebrat", "Actions_big/list-remove.png",   gl_pane, FXListPane::GAME_REMOVE    );
 
  
  ToolBar *lb = new ToolBar( this, gl_iconstheme );
  lb->makeButton( "\t\tSpustit", "Actions_big/system-run.png", this, GorgonaWindow::SYSTEM_RUN );

  ToolBar *vb = new ToolBar( this, gl_iconstheme );
  vb->makeButton( "\t\tPohled Detaily", "Actions_big/view-list-details.png", gl_pane, FXListPane::LIST_DETAIL );  
  vb->makeButton( "\t\tPohled Ikony",   "Actions_big/view-list-icons.png",   gl_pane, FXListPane::LIST_ICONS ); 

  
  
  /* Search Bar */
  m_findbar = new FindBar( this, gl_iconstheme, gl_pane, FXListPane::LIST_FIND, LAYOUT_RIGHT );

  // Configure application & load data
  read_config( );

  // If has non-stable version, show she in the window Header bar
  if( AutoVersion::STATUS_SHORT != 'S' ) {
    FXString wtitle = getHeader( )->getTitle( ) + " (" + AutoVersion::STATUS + ")";
    getHeader( )->setTitle( wtitle ); 
    getHeader( )->setText( AutoVersion::FULLVERSION_STRING ); 
  }
}

GorgonaWindow::~GorgonaWindow( )
{
  write_config( );
}

void GorgonaWindow::create( )
{
  FXPrimaryWindow::create( );
  gl_pane->create( );
  show( PLACEMENT_SCREEN );
  checkWindowState( );

  load( );

  std::cout.flush( );
}

/*************************************************************************************************/
long GorgonaWindow::OnCmd_System( FXObject *sender, FXSelector sel, void *data )
{
  FXlong resh = 0;

  switch( FXSELID( sel ) ) {
    /// FIXME GORGONA_WINDOW_003: Musi to tu byt dvakrat?
    case GorgonaWindow::SYSTEM_RUN : {
      std::cout << "[DEBUG - GorgonaWindow::OnCmd_System] Launch active game item ..." << std::endl;
      resh = ( ( this->run( ) == true ) ? 1 : 0 );
      break;
    }
  }

  return resh;
}

long GorgonaWindow::OnCmd_List( FXObject *sender, FXSelector sel, void *data )
{
  FXlong resh       = 0;

  switch( FXSELTYPE( sel ) ) {
    case SEL_CHANGED :
    {
      //std::cout << "[GorgonaWindow::OnCmd_List] List must be updated ..." << std::endl; // #
      FXString numinfo = "View ";
      numinfo += FXString::value( gl_pane->numItems( NULL, false ) ) + " game entries"; //!
      gl_statusbar->getStatusLine( )->setText( numinfo );

      //gl_change = true;
      m_app->getLibrary( )->setChange( );
      resh = 1;
      break;
    }

    case SEL_DOUBLECLICKED :
    {
       /// FIXME GORGONA_WINDOW_003: Musi to tu byt dvakrat?  
       // std::cout << "[GorgonaWindow::OnCmd_List] Launch active item" << std::endl; // #
       resh = ( ( this->run( ) == true ) ? 1 : 0 );
       break;
    }
  }

  std::cout.flush( );
  return resh;
}
 
long GorgonaWindow::OnCmd_Config( FXObject *sender, FXSelector sel, void *data )
{
  this->write_config( );

  /** FXIXME GORGONA_WINDOW_002: Kompletne odstranit, navrhnout radne konfiguracni rozhrani a prepracovat */
  switch( FXSELID( sel ) ) {
    case GorgonaWindow::CONF_SETUP : {
      //- cmd = gl_toolkit_pth + "bin/adie " + FXPath::expand( getApp( )->reg( ).getUserDirectory( ) + "/" + getApp( )->getVendorName( ) ) + "/" + getApp( )->getAppName( ) + ".rc";
      //- command.push( convert_str( gl_toolkit_pth + "bin/adie" ) );
      //- command.push( convert_str( getApp( )->reg( ).getUserDirectory( ) + "/" + getApp( )->getVendorName( ) + "/" + getApp( )->getAppName( ) + ".rc" ) );
      //
      //- cmd = FXPath::expand( cmd );
      //- std::cout << cmd.text( ) << std::endl;
      break;
    }
    case GorgonaWindow::CONF_FOX : {
      //- cmd = gl_toolkit_pth + "bin/ControlPanel " + getApp( )->getAppName( ) + " " + getApp( )->getVendorName( );
      break;
    }
  }

  getApp( )->reg( ).clear( );
  getApp( )->reg( ).parseFile( getApp( )->reg( ).getUserDirectory( ) + "/" + getApp( )->getVendorName( ) + "/" + getApp( )->getAppName( ) + ".rc" );
  getApp( )->reg( ).read( );
  this->read_config( );
  getApp( )->refresh( );
  getApp( )->repaint( );

  return 1;

}

long GorgonaWindow::OnCmd_Main( FXObject *sender, FXSelector sel, void *data )
{
  switch( FXSELID( sel ) ) {
    case GorgonaWindow::MAIN_CONFIG : {
      // std::cout << "Checking window configuration: mode-" << gl_winmode.text( ) << " " << this->getX( ) << "x" << this->getY( ) << " " << this->getWidth( ) << ", " << this->getHeight( ) << std::endl; // #
      gl_WinPos.set( this->getX( ), this->getY( ) );
      gl_WinSize.set( this->getWidth( ), this->getHeight( ) );
      break;
    }
  }

  return 1;
}

/*************************************************************************************************/
/* Pomocne funkce pro interni operace tridy                                                      */
/*************************************************************************************************/
void GorgonaWindow::load( )
{
  Library *games = m_app->getLibrary( );
  FXint num = games->no( );
  for( FXint i = 0; i != num ; i++ ) {
    gl_pane->insertItem( games->at( i ) );
  };

  gl_pane->showFolder( NULL, true );
  gl_pane->aktiveItem( );
  gl_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::TREE_EXPAND ), NULL );

  FXString status_info = "Ready (";
  status_info += " You have registered " + FXString::value( gl_pane->numItemsAll( ) );
  status_info += " game items in " + FXString::value( gl_pane->numFoldersAll( ) );
  status_info += " folders )";
  gl_statusbar->getStatusLine( )->setNormalText( status_info );
}
 
void GorgonaWindow::read_config( )
{
  FXString as, hg;

  //- gl_toolkit_pth     = getApp( )->reg( ).readStringEntry( "Modules", "toolkitpath", "/usr/" );
  //- gl_mlaunch_pth     = getApp( )->reg( ).readStringEntry( "Modules", "launchers", "/usr/share/Gorgona/modules/Launchers.lua" );
/**/  gl_profile         = getApp( )->reg( ).readStringEntry( "Profile", "Directory", ( FXSystem::getHomeDirectory( ) + "/.config/GorgonaWindow" ).text( ) );
  gl_browser         = getApp( )->reg( ).readStringEntry( "Profile", "browsercommand",    FXString::null );
  gl_browser_args    = getApp( )->reg( ).readStringEntry( "Profile", "browserargs",       FXString::null );
  gl_doubleclick_key = getApp( )->reg( ).readStringEntry( "Profile", "doubleclickaction", FXString::null );
/*
<<<<<<< HEAD
  as                 = getApp( )->reg( ).readStringEntry( "Profile", "autosave",          "false" );
=======
>>>>>>> devel2 */
  hg                 = getApp( )->reg( ).readStringEntry( "Profile", "hidegui",           "false" );
  gl_view            = getApp( )->reg( ).readStringEntry( "Window", "OpenView",          "icons" );  /// icons, list
  gl_winmode         = getApp( )->reg( ).readStringEntry( "Window", "MainMode",  "window" );  /// window, maximize, fulscreen
/*
<<<<<<< HEAD
  gl_autosave        = ( ( as.empty( ) or ( as == "false" ) ) ? false : true );
=======
>>>>>>> devel2 */
  gl_hidegui         = ( ( hg.empty( ) or ( hg == "false" ) ) ? false : true );

  if( gl_view == "icons" ) { gl_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_ICONS ), NULL ); }
  if( gl_view == "list"  ) { gl_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_DETAIL ), NULL ); }

  // std::cout << "{ read_config } OK" << std::endl; // #
}

void GorgonaWindow::write_config( )
{
/*
  if( FXStat::isDirectory( gl_profile ) == false ) {
    FXDir::create( gl_profile );
    FXDir::create( gl_profile + "/data" );
  }

  //- getApp( )->reg( ).writeStringEntry( "Modules", "toolkitpath",     gl_toolkit_pth.text( ) );
  //- getApp( )->reg( ).writeStringEntry( "Modules", "launchers",       gl_mlaunch_pth.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "Directory",       gl_profile.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "DoubleClickKey",  gl_doubleclick_key.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "WebBrowser",      gl_browser.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "WebBrowserArgs",  gl_browser_args.text( ) );
<<<<<<< HEAD
  getApp( )->reg( ).writeStringEntry( "Profile", "autosave",       ( ( gl_autosave == true ) ? "true" : "false" ) );
=======
>>>>>>> devel2
  getApp( )->reg( ).writeStringEntry( "Profile", "hidegui",        ( ( gl_hidegui  == true ) ? "true" : "false" ) );
*/
  getApp( )->reg( ).writeStringEntry( "Window",  "OpenView",        gl_view.text( ) );
  
  this->handle( this, FXSEL( SEL_CONFIGURE, GorgonaWindow::MAIN_CONFIG ), NULL );
  if( gl_winmode == "window" ) {
    FXint wx, wy;
    wx = this->getX( );
    wy = this->getY( );
    this->translateCoordinatesTo( wx, wy, getRoot( ), 0, 0 );
    //std::cout << "Window config: x = " << wx << " y = " << wy << std::endl; //#
    getApp( )->reg( ).writeIntEntry( "Window", "Position_X", gl_WinPos.x );
    getApp( )->reg( ).writeIntEntry( "Window", "Position_Y", gl_WinPos.y );
    getApp( )->reg( ).writeIntEntry( "Window", "Size_W", gl_WinSize.w );
    getApp( )->reg( ).writeIntEntry( "Window", "Size_H", gl_WinSize.h );

  }
  getApp( )->reg( ).writeStringEntry( "Window", "MainMode", gl_winmode.text( ) );

  if( getApp( )->reg( ).isModified( ) == true ) {
    getApp( )->reg( ).write( );
    //std::cout << "{ write_config } OK" << std::endl; // #
  }

}

void GorgonaWindow::read_Keywords( const FXString &listfile, const FXString &rootname )
{
  FXArray<GO_Keywords> *rlist;

  // Maping root of keyvords list?
  if( !gl_keywordsList.has( rootname ) ) {
    rlist = new FXArray<GO_Keywords>;
    gl_keywordsList.insert( rootname, rlist );
  }
  else { rlist = gl_keywordsList.data( gl_keywordsList.find( rootname ) ); }

  XMLDocument kwdoc;
  if( ( listfile.empty( ) != true ) && ( kwdoc.LoadFile( listfile.text( ) ) != false ) ) {
    XMLElement *kwelem = kwdoc.RootElement( )->FirstChildElement( "Keyword" );
    while( kwelem != NULL ) {
      GO_Keywords *kwl = new GO_Keywords;
      rlist->append( *kwl );

      // DUMPING
      std::cout << "LOAD Keywords " << kwelem->Attribute( "value" ) << std::endl;
      for( FXint i = 0; i != kwl->no( ); i++  ) {
         FXString key = kwl->key( i );
         if( !key.empty( ) ) {
           std::cout << "\t" << key.text( ) << std::endl;
         }
      }

      kwelem = kwelem->NextSiblingElement( "Keyword" );
    }
  }
  else {
    std::cout << "Chyba : neni zadan keywords soubor, nebo ma chybny format" << std::endl;
    gl_change = true;
  }
  std::cout << "{ read_keywords } OK" << std::endl;
  std::cout.flush( );
}


void GorgonaWindow::checkWindowState( )
{
  //std::cout << "{ checkWindowState }" << std::endl; // #
  if( gl_winmode == "window" ) {
    //std::cout << "Application Gorgona starting in window mode" << std::endl; // #
    gl_WinPos.x = getApp( )->reg( ).readIntEntry( "Window", "Position_X", 0 );
    gl_WinPos.y = getApp( )->reg( ).readIntEntry( "Window", "Position_Y", 0 );
    gl_WinSize.w = getApp( )->reg( ).readIntEntry( "Window", "Size_W", 800 );
    gl_WinSize.h = getApp( )->reg( ).readIntEntry( "Window", "Size_H", 700 );
    this->position( gl_WinPos.x, gl_WinPos.y, gl_WinSize.w, gl_WinSize.h );
    //std::cout << "Window config: x = " << gl_WinPos.x << " y = " << gl_WinPos.y << " w = " << gl_WinSize.h << " h = " << gl_WinSize.h << std::endl; // #
  }
  if( gl_winmode == "maximize" )   {
    //std::cout << "Application Gorgona starting in mximize window mode" << std::endl; // #
    if( this->maximize( true ) != true ) { std::cout << "Window mode maximize is not possible" << std::endl; }
  }
  if( gl_winmode == "fullscreen" ) {
    //std::cout << "Application Gorgona starting in fullscreen mode" << std::endl; // #
    if( this->fullScreen( true ) != true ) { std::cout << "Window mode fullscreen is not possible" << std::endl;}
  }
}

void GorgonaWindow::layout( )
{
  //std::cout << "{ layout }" << std::endl;
  FXPrimaryWindow::layout( );
  if( m_app->isCreated( )  && !this->isMinimized( ) ) {
  // Kontrola stavu modu okna muze byt uzita teprve az po ukonceni konfigurace aplikace a
  // predevsim tvorby samotneho okna. Dale nema smysl provadet kontrolu v pripade ze je hlavni
  // okno aplikace minimalizovano. Pri obnoveni sveho stavu se stejne (vetsinou) navrati do
  // sveho puvodniho stavu...
    if( this->isMaximized( ) == true ) { gl_winmode = "maximize"; }
    else if( this->isFullScreen( ) == true ) { gl_winmode = "fullscreen"; }
    else { gl_winmode = "window"; }
  }
  //std::cout << "winmode:" << gl_winmode.text( ) << std::endl;
}

/*
/// FIXME GORGONA_WINDOW_008: Argumenty ma zpracovat trida Gorgona. Pryc s tim
void GorgonaWindow::get_arguments( StringList *list )
{
  if( list != NULL ) {
    const char *const *__args = getApp( )->getArgv( );
    for( FXint i = 0; i != getApp( )->getArgc( ); i++ ) { list->push( __args[ i ] ); }
  }
}
*/

FXbool GorgonaWindow::run( FXGameItem *it )
{
  FXGameItem *item = ( ( it != NULL ) ? it : get_ActiveItem( ) );

  if( item != NULL ) {
    if( (*item)( ) > 0 ) { 
      gl_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_REFRESH ), NULL ); 
      // gl_change = true;
      m_app->getLibrary( )->setChange( );
    }
  }
  else {
    FXMessageBox::error( this, MBOX_OK, "Run error", "This item does not exists!" );
    std::cerr << "[ERROR - GorgonaWindow::Run]: " << "This item does not exists! " << std::endl;
    return false;
  }

  return true;
}

/*** END ******************************************************************************************/

