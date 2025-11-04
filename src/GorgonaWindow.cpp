// GorgonaWindow.cpp Copyright (c) %date%;  D.A.Tiger; GNU GPL 3
#include<GorgonaWindow.h>
#include<../version.h>
#include<Gorgona.h>
#include<Library.h>
#include<IconsTheme.h>
#include<Boxes.h>
#include<FXGameItem.h>
#include<FXListPane.h>

FXDEFMAP( GorgonaWindow ) LAUNCHERMAP[ ] = {
  FXMAPFUNC( SEL_COMMAND,   GorgonaWindow::LIST_EVENT,  GorgonaWindow::OnCmd_List ),
  FXMAPFUNC( SEL_CHANGED,   GorgonaWindow::LIST_EVENT,  GorgonaWindow::OnCmd_List ),
  FXMAPFUNC( SEL_CONFIGURE, GorgonaWindow::MAIN_CONFIG, GorgonaWindow::OnCmd_Main ),
  FXMAPFUNC( SEL_COMMAND,   GorgonaWindow::CONF_SETUP,  GorgonaWindow::OnCmd_Config ),
  FXMAPFUNC( SEL_COMMAND,   GorgonaWindow::CONF_FOX,    GorgonaWindow::OnCmd_Config ),
};

FXIMPLEMENT( GorgonaWindow, FXPrimaryWindow, LAUNCHERMAP, ARRAYNUMBER( LAUNCHERMAP ) )

/**************************************************************************************************/
GorgonaWindow::GorgonaWindow( Gorgona *app )
              : FXPrimaryWindow( app, "Gorgona", NULL, NULL, WINDOW_MAIN | CONTROLS_NORMAL, 0, 0, 800, 700 ) 
{ 
  m_app = app; 
 
  m_iconstheme = new IconsTheme( getApp( ), "/usr/share/icons/oxygen/base/" );
  m_iconstheme->insert( "Actions", "16x16/actions" );
  m_iconstheme->insert( "Actions_big", "32x32/actions" );
  m_iconstheme->insert( "Places", "16x16/places" );
  m_iconstheme->insert( "Status", "16x16/status" );
  FXIcon *ic_run    = m_iconstheme->getIcon( "Actions/system-run.png" );
  if( !ic_run ) { std::cout << "Icon nothing" << std::endl; }

  FXVerticalFrame *contend = new FXVerticalFrame( this, FRAME_NONE | LAYOUT_FILL );

  // Create Pane Switcher
  m_switcher = new FXSwitcher( contend, FRAME_NONE | LAYOUT_FILL, 0, 0, 0, 0,  0, 0, 0, 0 );

  // Create the item list
  m_pane = new FXListPane( m_switcher, m_iconstheme, this, GorgonaWindow::LIST_EVENT );
  m_pane->setClosedFolderIc( m_iconstheme->getIcon( "Places/folder.png" ) );
  m_pane->setOpenFolderIc(   m_iconstheme->getIcon( "Status/folder-open.png" ) );
  m_pane->setSmallItemIc(    m_iconstheme->getIcon( "Actions/roll.png" ) );
  m_pane->setBigItemIc(      m_iconstheme->getIcon( "Actions_big/roll.png" ) );
  m_pane->folder( "Categories"/*Genre List"*/ );

  // Status bar
  m_statusbar = new FXStatusBar( contend, FRAME_NONE | LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X );
  m_change = false;

  // Aplication menu
  FXMenuPane *m_menu = new FXMenuPane( this );
  m_mGorgona = new FXMenuPane( m_menu );
    new FXMenuCommand( m_mGorgona, "Pohled Detaily", (*m_iconstheme)[ "Actions/view-list-details.png" ], m_pane, FXListPane::LIST_DETAIL );
    new FXMenuCommand( m_mGorgona, "Pohled Ikony", m_iconstheme->getIcon( "Actions/view-list-icons.png" ), m_pane, FXListPane::LIST_ICONS );
    new FXMenuSeparator( m_mGorgona, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( m_mGorgona, "Rozbalit strom", m_iconstheme->getIcon( "Actions/view-list-tree.png" ), m_pane, FXListPane::TREE_EXPAND );
    new FXMenuCommand( m_mGorgona, "Sbalit strom", m_iconstheme->getIcon( "Actions/view-left-close.png" ), m_pane, FXListPane::TREE_COLAPS );
    new FXMenuSeparator( m_mGorgona, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( m_mGorgona, "Volby", m_iconstheme->getIcon( "Actions/configure.png" ), this, GorgonaWindow::CONF_SETUP );
    new FXMenuCommand( m_mGorgona, "Fox toolkit", m_iconstheme->getIcon( "Actions/code-class.png" ), this, GorgonaWindow::CONF_FOX );
    new FXMenuSeparator( m_mGorgona, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( m_mGorgona, "Ukoncit", m_iconstheme->getIcon( "Actions/window-close.png" ), getApp( ), FXApp::ID_QUIT );
  new FXMenuCascade( m_menu, "Gorgona", NULL, m_mGorgona );
  m_mGames = new FXMenuPane( m_menu );
    new FXMenuCommand( m_mGames, "Spustit", m_iconstheme->getIcon( "Actions/system-run.png" ), this, GorgonaWindow::LIST_EVENT );
    new FXMenuSeparator( m_mGames, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( m_mGames, "Pridat",  m_iconstheme->getIcon( "Actions/list-add.png" ),    m_pane, FXListPane::GAME_INSERT );
    new FXMenuCommand( m_mGames, "Editovat",  m_iconstheme->getIcon( "Actions/document-edit.png" ),    m_pane,  FXListPane::GAME_EDIT );
    new FXMenuCommand( m_mGames, "Odebrat",  m_iconstheme->getIcon( "Actions/list-remove.png" ),    m_pane, FXListPane::GAME_REMOVE );
    new FXMenuSeparator( m_mGames, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( m_mGames, "Ulozit",  m_iconstheme->getIcon( "Actions/document-save.png" ), m_app, Gorgona::SAVE_LIBRARY );
  new FXMenuCascade( m_menu, "Library", NULL, m_mGames );
  m_mTools = new FXMenuPane( m_menu );
  new FXMenuCascade( m_menu, "Nastroje", NULL, m_mTools );
  new FXMenuSeparator( m_menu, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
  m_mHelp = new FXMenuPane( m_menu );
  new FXMenuCascade( m_menu, "Napoveda", NULL, m_mHelp );
  new FXMenuCommand( m_menu, "O aplikaci", NULL, getApp( ), FXApp::ID_QUIT );

  new MainBar( this, m_iconstheme, m_menu );

  /* Tool bars */ 
  ToolBar *mb = new ToolBar( this, m_iconstheme );
  mb->makeButton( "\t\tPridat",  "Actions_big/list-add.png",      m_pane, FXListPane::GAME_INSERT    );
  mb->makeButton( "\t\tUpravit", "Actions_big/document-edit.png", m_pane, FXListPane::GAME_EDIT      );
  mb->makeButton( "\t\tOdebrat", "Actions_big/list-remove.png",   m_pane, FXListPane::GAME_REMOVE    );
 
  
  ToolBar *lb = new ToolBar( this, m_iconstheme );
  lb->makeButton( "\t\tSpustit", "Actions_big/system-run.png", this, GorgonaWindow::LIST_EVENT );

  ToolBar *vb = new ToolBar( this, m_iconstheme );
  vb->makeButton( "\t\tPohled Detaily", "Actions_big/view-list-details.png", m_pane, FXListPane::LIST_DETAIL );  
  vb->makeButton( "\t\tPohled Ikony",   "Actions_big/view-list-icons.png",   m_pane, FXListPane::LIST_ICONS ); 

  
  
  /* Search Bar */
  m_findbar = new FindBar( this, m_iconstheme, m_pane, FXListPane::LIST_FIND, LAYOUT_RIGHT );

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
  m_pane->create( );
  show( PLACEMENT_SCREEN );
  checkWindowState( );

  load( );

  std::cout.flush( );
}

/*************************************************************************************************/
long GorgonaWindow::OnCmd_List( FXObject *sender, FXSelector sel, void *data )
{
  FXlong resh       = 0;

  switch( FXSELTYPE( sel ) ) {
    case SEL_CHANGED :
    {
      // FIXME : Zkotrolovat jak, kdy, proc a kym je odesilana tato zprava.
      //DEBUG_OUT( "[GorgonaWindow::OnCmd_List] SEL_CHANGED" ) // #
      FXString numinfo = "View ";
      numinfo += FXString::value( m_pane->numItems( NULL, false ) ) + " game entries"; //!
      m_statusbar->getStatusLine( )->setText( numinfo );

      m_app->getLibrary( )->setChange( );
      resh = 1;
      break;
    }

    case SEL_COMMAND :
    {
      DEBUG_OUT( "[GorgonaWindow::OnCmd_List] SEL_COMMAND" ) // # 
      FXString _id = m_pane->getActiveItemId( ); 
      if( !_id.empty( ) ) {
        if( m_app->getLibrary( )->run( _id ) ) {
          m_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_REFRESH ), NULL );
        }
      }
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
      //- cmd = m_toolkit_pth + "bin/adie " + FXPath::expand( getApp( )->reg( ).getUserDirectory( ) + "/" + getApp( )->getVendorName( ) ) + "/" + getApp( )->getAppName( ) + ".rc";
      //- command.push( convert_str( m_toolkit_pth + "bin/adie" ) );
      //- command.push( convert_str( getApp( )->reg( ).getUserDirectory( ) + "/" + getApp( )->getVendorName( ) + "/" + getApp( )->getAppName( ) + ".rc" ) );
      //
      //- cmd = FXPath::expand( cmd );
      //- std::cout << cmd.text( ) << std::endl;
      break;
    }
    case GorgonaWindow::CONF_FOX : {
      //- cmd = m_toolkit_pth + "bin/ControlPanel " + getApp( )->getAppName( ) + " " + getApp( )->getVendorName( );
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
      m_WinPos.set( this->getX( ), this->getY( ) );
      m_WinSize.set( this->getWidth( ), this->getHeight( ) );
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
    m_pane->insertItem( games->at( i ) );
  };

  m_pane->showFolder( NULL, true );
  m_pane->aktiveItem( );
  m_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::TREE_EXPAND ), NULL );

  FXString status_info = "Ready (";
  status_info += " You have registered " + FXString::value( m_pane->numItemsAll( ) );
  status_info += " game items in " + FXString::value( m_pane->numFoldersAll( ) );
  status_info += " folders )";
  m_statusbar->getStatusLine( )->setNormalText( status_info );
}
 
void GorgonaWindow::read_config( )
{
  FXString as, hg;

  m_profile         = getApp( )->reg( ).readStringEntry( "Profile", "Directory", ( FXSystem::getHomeDirectory( ) + "/.config/GorgonaWindow" ).text( ) );
  m_browser         = getApp( )->reg( ).readStringEntry( "Profile", "browsercommand",    FXString::null );
  m_browser_args    = getApp( )->reg( ).readStringEntry( "Profile", "browserargs",       FXString::null );
  m_doubleclick_key = getApp( )->reg( ).readStringEntry( "Profile", "doubleclickaction", FXString::null );
  hg                 = getApp( )->reg( ).readStringEntry( "Profile", "hidegui",           "false" );
  m_view            = getApp( )->reg( ).readStringEntry( "Window", "OpenView",          "icons" );  /// icons, list
  m_winmode         = getApp( )->reg( ).readStringEntry( "Window", "MainMode",  "window" );  /// window, maximize, fulscreen
  m_hidegui         = ( ( hg.empty( ) or ( hg == "false" ) ) ? false : true );
/*
  m_toolkit_pth     = getApp( )->reg( ).readStringEntry( "Modules", "toolkitpath", "/usr/" );
  m_mlaunch_pth     = getApp( )->reg( ).readStringEntry( "Modules", "launchers", "/usr/share/Gorgona/modules/Launchers.lua" );  
  FXbool as          = getApp( )->reg( ).readStringEntry( "Profile", "autosave",          "false" );
  m_autosave        = ( ( as.empty( ) or ( as == "false" ) ) ? false : true );
 */  

  if( m_view == "icons" ) { m_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_ICONS ), NULL ); }
  if( m_view == "list"  ) { m_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_DETAIL ), NULL ); }

  DEBUG_OUT( "[GorgonaWindow::read_config] OK" ) // #
}

void GorgonaWindow::write_config( )
{
/*
  if( FXStat::isDirectory( m_profile ) == false ) {
    FXDir::create( m_profile );
    FXDir::create( m_profile + "/data" );
  }

  //- getApp( )->reg( ).writeStringEntry( "Modules", "toolkitpath",     m_toolkit_pth.text( ) );
  //- getApp( )->reg( ).writeStringEntry( "Modules", "launchers",       m_mlaunch_pth.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "Directory",       m_profile.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "DoubleClickKey",  m_doubleclick_key.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "WebBrowser",      m_browser.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "WebBrowserArgs",  m_browser_args.text( ) );

  getApp( )->reg( ).writeStringEntry( "Profile", "autosave",       ( ( m_autosave == true ) ? "true" : "false" ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "hidegui",        ( ( m_hidegui  == true ) ? "true" : "false" ) );
*/
  getApp( )->reg( ).writeStringEntry( "Window",  "OpenView",        m_view.text( ) );
  
  this->handle( this, FXSEL( SEL_CONFIGURE, GorgonaWindow::MAIN_CONFIG ), NULL );
  if( m_winmode == "window" ) {
    FXint wx, wy;
    wx = this->getX( );
    wy = this->getY( );
    this->translateCoordinatesTo( wx, wy, getRoot( ), 0, 0 );
    
    getApp( )->reg( ).writeIntEntry( "Window", "Position_X", m_WinPos.x );
    getApp( )->reg( ).writeIntEntry( "Window", "Position_Y", m_WinPos.y );
    getApp( )->reg( ).writeIntEntry( "Window", "Size_W", m_WinSize.w );
    getApp( )->reg( ).writeIntEntry( "Window", "Size_H", m_WinSize.h );

  }
  getApp( )->reg( ).writeStringEntry( "Window", "MainMode", m_winmode.text( ) );

  if( getApp( )->reg( ).isModified( ) == true ) {
    getApp( )->reg( ).write( );
    DEBUG_OUT( "[GorgonaWindow::write_config] OK" ) // #
  }

}

void GorgonaWindow::read_Keywords( const FXString &listfile, const FXString &rootname )
{/*
  FXArray<GO_Keywords> *rlist;

  // Maping root of keyvords list?
  if( !m_keywordsList.has( rootname ) ) {
    rlist = new FXArray<GO_Keywords>;
    m_keywordsList.insert( rootname, rlist );
  }
  else { rlist = m_keywordsList.data( m_keywordsList.find( rootname ) ); }

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
    m_change = true;
  }
  std::cout << "{ read_keywords } OK" << std::endl;
  std::cout.flush( );
  */
}


void GorgonaWindow::checkWindowState( )
{
  if( m_winmode == "window" ) {
    //DEBUG_OUT( "[GorgonaWindow::checkWindowState] "Application Gorgona starting in window mode" << std::endl; // #
    m_WinPos.x = getApp( )->reg( ).readIntEntry( "Window", "Position_X", 0 );
    m_WinPos.y = getApp( )->reg( ).readIntEntry( "Window", "Position_Y", 0 );
    m_WinSize.w = getApp( )->reg( ).readIntEntry( "Window", "Size_W", 800 );
    m_WinSize.h = getApp( )->reg( ).readIntEntry( "Window", "Size_H", 700 );
    this->position( m_WinPos.x, m_WinPos.y, m_WinSize.w, m_WinSize.h );
    //DEBUG_OUT( "Window config: x = " << m_WinPos.x << " y = " << m_WinPos.y << " w = " << m_WinSize.h << " h = " << m_WinSize.h ) // #
  }
  if( m_winmode == "maximize" )   {
    //DEBUG_OUT( "[GorgonaWindow::checkWindowState] Application Gorgona starting in mximize window mode" ) // #
    if( this->maximize( true ) != true ) { std::cout << "Window mode maximize is not possible" << std::endl; }
  }
  if( m_winmode == "fullscreen" ) {
    //DEBUG_OUT( "[GorgonaWindow::checkWindowState] Application Gorgona starting in fullscreen mode" ) // #
    if( this->fullScreen( true ) != true ) { std::cout << "Window mode fullscreen is not possible" << std::endl;}
  }
}

void GorgonaWindow::layout( )
{
  FXPrimaryWindow::layout( );
  if( m_app->isCreated( )  && !this->isMinimized( ) ) {
  // Kontrola stavu modu okna muze byt uzita teprve az po ukonceni konfigurace aplikace a
  // predevsim tvorby samotneho okna. Dale nema smysl provadet kontrolu v pripade ze je hlavni
  // okno aplikace minimalizovano. Pri obnoveni sveho stavu se stejne (vetsinou) navrati do
  // sveho puvodniho stavu...
    if( this->isMaximized( ) == true ) { m_winmode = "maximize"; }
    else if( this->isFullScreen( ) == true ) { m_winmode = "fullscreen"; }
    else { m_winmode = "window"; }
  }
  // DEBUG_OUT( "[GorgonaWindow::layout] winmode:" << m_winmode.text( ) )
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
  FXGameItem *item = ( ( it != NULL ) ? it : m_pane->getCurrentItem( ) ); 

  if( item != NULL ) {
    if( (*item)( ) > 0 ) { 
      m_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_REFRESH ), NULL ); 
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

