// FXGameLauncher.cpp Copyright (c) %date%;  D.A.Tiger; GNU GPL 3
#include<FXGameLauncher.h>

using namespace GORGONA;

FXDEFMAP( FXGameLauncher ) LAUNCHERMAP[ ] = {
  FXMAPFUNC( SEL_COMMAND, FXGameLauncher::SYSTEM_RUN, FXGameLauncher::OnCmd_System ),
  FXMAPFUNC( SEL_COMMAND, FXGameLauncher::LIST_EVENT, FXGameLauncher::OnCmd_List ),
  FXMAPFUNC( SEL_CHANGED, FXGameLauncher::LIST_EVENT, FXGameLauncher::OnCmd_List ),
  FXMAPFUNC( SEL_CONFIGURE, FXGameLauncher::MAIN_CONFIG, FXGameLauncher::OnCmd_Main ),
  FXMAPFUNC( SEL_COMMAND, FXGameLauncher::CONF_SETUP, FXGameLauncher::OnCmd_Config ),
  FXMAPFUNC( SEL_COMMAND, FXGameLauncher::CONF_FOX,   FXGameLauncher::OnCmd_Config ),
  FXMAPFUNC( SEL_COMMAND, FXGameLauncher::DATA_SAVE,  FXGameLauncher::OnCmd_Data ),
  FXMAPFUNC( SEL_SIGNAL, FXGameLauncher::SIGNAL_CHLD,  FXGameLauncher::OnSig_ExitChild )
};

FXIMPLEMENT( FXGameLauncher, FXGWindow, LAUNCHERMAP, ARRAYNUMBER( LAUNCHERMAP ) )

/**************************************************************************************************/
FXGameLauncher::FXGameLauncher( FXApp *a )
              : FXGWindow( a, "Gorgona", NULL, NULL, WINDOW_PRIMARY | CONTROLS_NORMAL, 0, 0, 800, 700 ) // (Fox Game Launcher - BETA.00.01)
{
  std::cout << "=== Gorgona ========================================" << std::endl;
  this->version( );
  std::cout << "=== Message =========================================" << std::endl;
  std::cout.flush( );

  StringList arglist;
  get_arguments( &arglist );

   getApp( )->addSignal( SIGCHLD, this, FXGameLauncher::SIGNAL_CHLD, false, 0 );
  //this->setX( ( a->getRootWindow( )->getWidth( ) / 2 ) - ( this->getWidth( ) / 2 ) );
  //this->setY( ( a->getRootWindow( )->getHeight( ) / 2 ) - ( this->getHeight( ) / 2 ) );
  gl_created= false;

  gl_iconstheme = new IconsTheme( getApp( ), "/usr/share/icons/oxygen/base/" );
  gl_iconstheme->insert( "Actions", "16x16/actions" );
  gl_iconstheme->insert( "Actions_big", "32x32/actions" );
  gl_iconstheme->insert( "Places", "16x16/places" );
  gl_iconstheme->insert( "Status", "16x16/status" );
  FXIcon *ic_run    = gl_iconstheme->getIcon( "Actions/system-run.png" );
  //FXIcon *ic_quit   = gl_iconstheme->getIcon( "Actions/window-close.png" );
  if( !ic_run ) { std::cout << "Icon nothing" << std::endl; }
  //gl_iconstheme->dump( );

  FXVerticalFrame *contend = new FXVerticalFrame( this, FRAME_NONE | LAYOUT_FILL );


  // Create Pane Switcher
  gl_switcher = new FXSwitcher( contend, FRAME_NONE | LAYOUT_FILL, 0, 0, 0, 0,  0, 0, 0, 0 );
  // Create the item list
  gl_pane = new FXListPane( gl_switcher, gl_iconstheme, this, FXGameLauncher::LIST_EVENT );
  gl_pane->setClosedFolderIc( gl_iconstheme->getIcon( "Places/folder.png" ) );
  gl_pane->setOpenFolderIc(   gl_iconstheme->getIcon( "Status/folder-open.png" ) );
  gl_pane->setSmallItemIc(    gl_iconstheme->getIcon( "Actions/roll.png" ) );
  gl_pane->setBigItemIc(      gl_iconstheme->getIcon( "Actions_big/roll.png" ) );
  gl_pane->folder( "Game List" );

  // Infobox
  FXGroupBox *infogroup = new FXGroupBox( contend, "", GROUPBOX_NORMAL| FRAME_LINE | LAYOUT_FILL_X | LAYOUT_FIX_HEIGHT, 0, 0, 0, 150,  0, 0, 0, 0 );
  gl_text = new FXText( infogroup, NULL, 0, FRAME_NONE | TEXT_WORDWRAP | TEXT_READONLY | LAYOUT_FILL );

  // Status bar
  gl_statusbar = new FXStatusBar( contend, FRAME_NONE | LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X );
  gl_change = false;

  // Aplication menu
  FXMenuPane *gl_menu = new FXMenuPane( this );
  FXMenuButton *menubutton = new FXMenuButton( getHeader( ), "\t\tMenu",  gl_iconstheme->getIcon( "Actions_big/run-build-install-root.png" ), gl_menu, FRAME_RAISED|ICON_ABOVE_TEXT|LAYOUT_LEFT );
  new FXVerticalSeparator( getHeader( ) );  

  gl_mGorgona = new FXMenuPane( gl_menu );
    new FXMenuCommand( gl_mGorgona, "Pohled Detaily", gl_iconstheme->getIcon( "Actions/view-list-details.png" ), gl_pane, FXListPane::LIST_DETAIL );
    new FXMenuCommand( gl_mGorgona, "Pohled Ikony", gl_iconstheme->getIcon( "Actions/view-list-icons.png" ), gl_pane, FXListPane::LIST_ICONS );
    new FXMenuSeparator( gl_mGorgona, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( gl_mGorgona, "Rozbalit strom", gl_iconstheme->getIcon( "Actions/view-list-tree.png" ), gl_pane, FXListPane::TREE_EXPAND );
    new FXMenuCommand( gl_mGorgona, "Sbalit strom", gl_iconstheme->getIcon( "Actions/view-left-close.png" ), gl_pane, FXListPane::TREE_COLAPS );
    new FXMenuSeparator( gl_mGorgona, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( gl_mGorgona, "Volby", gl_iconstheme->getIcon( "Actions/configure.png" ), this, FXGameLauncher::CONF_SETUP );
    new FXMenuCommand( gl_mGorgona, "Fox toolkit", gl_iconstheme->getIcon( "Actions/code-class.png" ), this, FXGameLauncher::CONF_FOX );
    new FXMenuSeparator( gl_mGorgona, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( gl_mGorgona, "Ukoncit", gl_iconstheme->getIcon( "Actions/window-close.png" ), getApp( ), FXApp::ID_QUIT );
  new FXMenuCascade( gl_menu, "Gorgona", NULL, gl_mGorgona );
  gl_mGames = new FXMenuPane( gl_menu );
    new FXMenuCommand( gl_mGames, "Spustit", gl_iconstheme->getIcon( "Actions/system-run.png" ), this, FXGameLauncher::SYSTEM_RUN );
    new FXMenuSeparator( gl_mGames, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( gl_mGames, "Pridat",  gl_iconstheme->getIcon( "Actions/list-add.png" ),    gl_pane, FXListPane::GAME_INSERT );
    new FXMenuCommand( gl_mGames, "Editovat",  gl_iconstheme->getIcon( "Actions/document-edit.png" ),    gl_pane,  FXListPane::GAME_EDIT );
    new FXMenuCommand( gl_mGames, "Odebrat",  gl_iconstheme->getIcon( "Actions/list-remove.png" ),    gl_pane, FXListPane::GAME_REMOVE );
    new FXMenuSeparator( gl_mGames, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
    new FXMenuCommand( gl_mGames, "Ulozit",  gl_iconstheme->getIcon( "Actions/document-save.png" ),    this, FXGameLauncher::DATA_SAVE );
  new FXMenuCascade( gl_menu, "Arena", NULL, gl_mGames );
  gl_mTools = new FXMenuPane( gl_menu );
  new FXMenuCascade( gl_menu, "Nastroje", NULL, gl_mTools );
  new FXMenuSeparator( gl_menu, NULL, 0, FRAME_GROOVE | LAYOUT_FILL_X );
  gl_mHelp = new FXMenuPane( gl_menu );
  new FXMenuCascade( gl_menu, "Napoveda", NULL, gl_mHelp );
  new FXMenuCommand( gl_menu, "O aplikaci", NULL /*gl_iconstheme->getIcon( "Actions/window-close.png" )*/, getApp( ), FXApp::ID_QUIT );


  // Tools buttons for window Header
  gl_actions.append( menubutton );

  gl_actions.append( new FXButton( getHeader( ),  "\t\tSpustit", gl_iconstheme->getIcon( "Actions_big/system-run.png" ),    this, FXGameLauncher::SYSTEM_RUN, BUTTON_TOOLBAR | ICON_ABOVE_TEXT | LAYOUT_FILL_Y ) );
  //gl_actions.append( new FXLabel(  _titlefr, "Gorgona \nThe Games manager",  NULL, FRAME_NONE|ICON_ABOVE_TEXT|LAYOUT_FILL_Y | LAYOUT_CENTER_X ) );
  gl_actions.append( new FXButton( getHeader( ), "\t\tPridat",  gl_iconstheme->getIcon( "Actions_big/list-add.png" ),      gl_pane, FXListPane::GAME_INSERT, BUTTON_TOOLBAR | ICON_ABOVE_TEXT | LAYOUT_RIGHT ) );
  gl_actions.append( new FXButton( getHeader( ), "\t\tUpravit", gl_iconstheme->getIcon( "Actions_big/document-edit.png" ), gl_pane,  FXListPane::GAME_EDIT, BUTTON_TOOLBAR | ICON_ABOVE_TEXT | LAYOUT_RIGHT ) );
  gl_actions.append( new FXButton( getHeader( ), "\t\tOdebrat", gl_iconstheme->getIcon( "Actions_big/list-remove.png" ),   gl_pane, FXListPane::GAME_REMOVE, BUTTON_TOOLBAR | ICON_ABOVE_TEXT | LAYOUT_RIGHT ) );

  //gl_actions.append( new FXButton( _windowfr, "Zavrit",  gl_iconstheme->getIcon( "Actions_big/window-close.png" ),   getApp( ), FXApp::ID_QUIT, FRAME_RAISED|ICON_ABOVE_TEXT|LAYOUT_FILL_Y | LAYOUT_RIGHT ) );

  // - lua initialize
  if( l_open( this ) ) { std::cout << "Lua initialized" << std::endl; }

  // Configure application & load data
  read_config( );
  //this->handle( NULL, FXSEL( SEL_CONFIGURE, FXGameLauncher::MAIN_CONFIG ), NULL );
}

FXGameLauncher::~FXGameLauncher( )
{
  write_config( );
  if( ( gl_autosave == true ) || ( gl_change == true ) ) { save( ); }
}

void FXGameLauncher::create( )
{
  FXGWindow::create( );
  gl_pane->create( );
  show( PLACEMENT_SCREEN );
  checkWindowState( );

  gl_mlaunch = l_Script( gl_mlaunch_pth );
  load( );

  gl_created= true;
}

/*************************************************************************************************/
long FXGameLauncher::OnCmd_System( FXObject *sender, FXSelector sel, void *data )
{
  FXlong resh = 0;

  switch( FXSELID( sel ) ) {
    case FXGameLauncher::SYSTEM_RUN : {
      resh = ( ( this->run( ) == true ) ? 1 : 0 );
      break;
    }
  }

  return resh;
}

long FXGameLauncher::OnCmd_List( FXObject *sender, FXSelector sel, void *data )
{
  FXlong resh       = 0;

  switch( FXSELTYPE( sel ) ) {
    case SEL_COMMAND :
    {
      //std::cout << "[FXGameLauncher::OnCmd_List]New Select item in list ..." << std::endl;
      FXGameItem *gl_selected = gl_pane->getCurrentItem( );
      if( ( gl_selected ) != NULL ) { gl_text->setText( gl_selected->read( "Description" ) ); }
      resh = 1;
      break;
    }

    case SEL_CHANGED :
    {
      //std::cout << "[FXGameLauncher::OnCmd_List]List must be changed ..." << std::endl;
      gl_change = true;
      resh = 1;
      break;
    }

    case SEL_DOUBLECLICKED :
    {
       //std::cout << "[FXGameLauncher::OnCmd_List]Active item running ..." << std::endl;
       resh = ( ( this->run( ) == true ) ? 1 : 0 );
       break;
    }
  }

  std::cout.flush( );
  return resh;
}

long FXGameLauncher::OnCmd_Data( FXObject *sender, FXSelector sel, void *data )
{
  FXlong resh = 0;

  switch( FXSELID( sel ) ) {
    case FXGameLauncher::DATA_CHANGED : {
      gl_change = true;
      resh = 1;
      break;
    }

    case FXGameLauncher::DATA_SAVE : {
      if( gl_change == true ) {
        save( );
        resh = 1;
        gl_change = false;
      }
      break;
    }
  }

  return resh;
}

long FXGameLauncher::OnCmd_Config( FXObject *sender, FXSelector sel, void *data )
{
  FXArray<const FXchar*> command;
  FXString cmd;

  this->write_config( );
  switch( FXSELID( sel ) ) {
    case FXGameLauncher::CONF_SETUP : {
      cmd = gl_toolkit_pth + "bin/adie " + FXPath::expand( getApp( )->reg( ).getUserDirectory( ) + "/" + getApp( )->getVendorName( ) ) + "/" + getApp( )->getAppName( ) + ".rc";
      //command.push( convert_str( gl_toolkit_pth + "bin/adie" ) );
      //command.push( convert_str( getApp( )->reg( ).getUserDirectory( ) + "/" + getApp( )->getVendorName( ) + "/" + getApp( )->getAppName( ) + ".rc" ) );
      //
      //cmd = FXPath::expand( cmd );
      std::cout << cmd.text( ) << std::endl;
      break;
    }
    case FXGameLauncher::CONF_FOX : {
      cmd = gl_toolkit_pth + "bin/ControlPanel " + getApp( )->getAppName( ) + " " + getApp( )->getVendorName( );
      break;
    }
  }
  //system( ( cmd + "&" ).text( ) );
  parse_params( &command, cmd );
  command.append( NULL );
  exec( command, false, false );

  getApp( )->reg( ).clear( );
  getApp( )->reg( ).parseFile( getApp( )->reg( ).getUserDirectory( ) + "/" + getApp( )->getVendorName( ) + "/" + getApp( )->getAppName( ) + ".rc" );
  getApp( )->reg( ).read( );
  this->read_config( );
  getApp( )->refresh( );
  getApp( )->repaint( );

  return 1;

}

long FXGameLauncher::OnCmd_Main( FXObject *sender, FXSelector sel, void *data )
{
  switch( FXSELID( sel ) ) {
    case FXGameLauncher::MAIN_CONFIG : {
      //read_config( );
      std::cout << "Checking window configuration: mode-" << gl_winmode.text( ) << " " << this->getX( ) << "x" << this->getY( ) << " " << this->getWidth( ) << ", " << this->getHeight( ) << std::endl;
      gl_WinPos.set( this->getX( ), this->getY( ) );
      gl_WinSize.set( this->getWidth( ), this->getHeight( ) );
      break;
    }
  }

  return 1;
}

long FXGameLauncher::OnSig_ExitChild( FXObject *sender, FXSelector sel, void *data )
{
   // Cleaning the Child process
   FXint  __status;
   struct rusage __usage;

   pid_t childPID = wait3( &__status, 0, &__usage );
   std::cout << "Quiting the child process " << childPID << " at " << getpid( ) << std::endl;
   std::cout << "Exit code is: " << __status << std::endl;

   return 1;
}

/*************************************************************************************************/
/* Pomocne funkce pro interni operace tridy                                                      */
/*************************************************************************************************/
void FXGameLauncher::load( )
{
  TiXmlDocument  xdoc;
  TiXmlElement  *xegame;
  FXGameItem    *it;

  std::cout << "Loading the xml-file games list: " << gl_datafile.text( ) << std::endl;

  if( ( gl_datafile.empty( ) != true ) && ( xdoc.LoadFile( gl_datafile.text( ) ) != false ) ) {
    if( ( xegame = xdoc.RootElement( )->FirstChildElement( "Game" ) ) != NULL ) {
      for( xegame; xegame; xegame = xegame->NextSiblingElement( "Game" ) ) {
         if( ( it = new FXGameItem( ) ) != NULL ) {
           it->load( xegame );
           it->checkIcons( getApp( ) );
           gl_pane->insertItem( it );
         }
         else { std::cout << "CHYBA : Nelze vytvorit polozku spoustece" << std::endl; }
      }
    }
  }
  else {
    std::cout << "Chyba : neni zadan soubor, nebo ma chybny format" << std::endl;
    gl_change = true;
  }
  std::cout.flush( );

  gl_pane->showFolder( NULL, true );
  gl_pane->aktiveItem( );
  gl_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::TREE_EXPAND ), NULL );

  FXString status_info = "Ready (";
  status_info += " You have registered " + FXString::value( gl_pane->numItemsAll( ) );
  status_info += " game items in " + FXString::value( gl_pane->numFoldersAll( ) );
  status_info += " folders )";
  gl_statusbar->getStatusLine( )->setNormalText( status_info );
}

void FXGameLauncher::save( )
{
  TiXmlDocument     xdoc;
  TiXmlElement     *xroot;
  TiXmlDeclaration *xdecl;

  xdecl = new TiXmlDeclaration( "1.0", "", "" );
  xdoc.LinkEndChild( xdecl );

  xroot = new TiXmlElement( getApp( )->reg( ).getAppKey( ).text( ) );
  xdoc.LinkEndChild( xroot );

  FXGameItemArray buff;
  gl_pane->getItemList( NULL, &buff, true );
  for( FXint i = 0; i != buff.no( ); i++ ) { buff[ i ]->save( xroot ); }

  std::cout << "Saving the menu xml-file" << std::endl;
  xdoc.SaveFile( gl_datafile.text( ) );

}

void FXGameLauncher::read_config( )
{
  FXString as, hg;
  //FXint pos_x = ( getApp( )->getRootWindow( )->getWidth( ) / 2 ) - ( this->getWidth( ) / 2 );
  //FXint pos_y = ( getApp( )->getRootWindow( )->getHeight( ) / 2 ) - ( this->getHeight( ) / 2 );

  gl_toolkit_pth     = getApp( )->reg( ).readStringEntry( "Modules", "toolkitpath", "/usr/" );
  gl_mlaunch_pth     = getApp( )->reg( ).readStringEntry( "Modules", "launchers", "/opt/Gorgona/modules/Launchers.lua" );
  gl_profile         = getApp( )->reg( ).readStringEntry( "Profile", "Directory", ( FXSystem::getHomeDirectory( ) + "/.config/FXGameLauncher" ).text( ) );
  gl_gamelist        = getApp( )->reg( ).readStringEntry( "Profile", "Gamelist",         "gamelist" );
  gl_browser         = getApp( )->reg( ).readStringEntry( "Profile", "browsercommand",    FXString::null );
  gl_browser_args    = getApp( )->reg( ).readStringEntry( "Profile", "browserargs",       FXString::null );
  gl_doubleclick_key = getApp( )->reg( ).readStringEntry( "Profile", "doubleclickaction", FXString::null );
  as                 = getApp( )->reg( ).readStringEntry( "Profile", "autosave",          "false" );
  hg                 = getApp( )->reg( ).readStringEntry( "Profile", "hidegui",           "false" );
  gl_view            = getApp( )->reg( ).readStringEntry( "Window", "OpenView",          "icons" );  /// icons, list
  gl_winmode         = getApp( )->reg( ).readStringEntry( "Window", "MainMode",  "window" );  /// window, maximize, fulscreen

  gl_autosave        = ( ( as.empty( ) or ( as == "false" ) ) ? false : true );
  gl_hidegui         = ( ( hg.empty( ) or ( hg == "false" ) ) ? false : true );

  gl_datafile    = gl_profile + "/data/" + gl_gamelist + ".xml";//gamelist.xml";
  //gl_datafile  = "/home/gabriel/.parallelrealities/jgamelaunch/gamelist.xml";
  //gl_datafile  = "/home/gabriel/Projects/Fox/FXGameLauncher/BETA.01.00/data/gamelist.xml";

  if( gl_view == "icons" ) { gl_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_ICONS ), NULL ); }
  if( gl_view == "list"  ) { gl_pane->handle( this, FXSEL( SEL_COMMAND, FXListPane::LIST_DETAIL ), NULL ); }

  this->read_Keywords( "/opt/Gorgona/data/keywords.xml");
  std::cout << "{ read_config } OK" << std::endl;
}

void FXGameLauncher::write_config( )
{
  if( FXStat::isDirectory( gl_profile ) == false ) {
    FXDir::create( gl_profile );
    FXDir::create( gl_profile + "/data" );
  }

  getApp( )->reg( ).writeStringEntry( "Modules", "toolkitpath",     gl_toolkit_pth.text( ) );
  getApp( )->reg( ).writeStringEntry( "Modules", "launchers",       gl_mlaunch_pth.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "Directory",       gl_profile.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "Gamelist",        gl_gamelist.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "DoubleClickKey",  gl_doubleclick_key.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "WebBrowser",      gl_browser.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "WebBrowserArgs",  gl_browser_args.text( ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "autosave",       ( ( gl_autosave == true ) ? "true" : "false" ) );
  getApp( )->reg( ).writeStringEntry( "Profile", "hidegui",        ( ( gl_hidegui  == true ) ? "true" : "false" ) );
  getApp( )->reg( ).writeStringEntry( "Window",  "OpenView",        gl_view.text( ) );
  this->handle( this, FXSEL( SEL_CONFIGURE, FXGameLauncher::MAIN_CONFIG ), NULL );
  if( gl_winmode == "window" ) {
    FXint wx, wy;
    wx = this->getX( );
    wy = this->getY( );
    this->translateCoordinatesTo( wx, wy, getRoot( ), 0, 0 );
    std::cout << "Window config: x = " << wx << " y = " << wy << std::endl;
    getApp( )->reg( ).writeIntEntry( "Window", "Position_X", gl_WinPos.x );
    getApp( )->reg( ).writeIntEntry( "Window", "Position_Y", gl_WinPos.y );
    getApp( )->reg( ).writeIntEntry( "Window", "Size_W", gl_WinSize.w );
    getApp( )->reg( ).writeIntEntry( "Window", "Size_H", gl_WinSize.h );

  }
  getApp( )->reg( ).writeStringEntry( "Window", "MainMode", gl_winmode.text( ) );

  if( getApp( )->reg( ).isModified( ) == true ) {
    getApp( )->reg( ).write( );
    std::cout << "{ write_config } OK" << std::endl;
  }
}

void FXGameLauncher::read_Keywords( const FXString &listfile, const FXString &rootname )
{
   FXArray<GO_Keywords> *rlist;

   // Maping root of keyvords list?
   if( !gl_keywordsList.has( rootname ) ) {
     rlist = new FXArray<GO_Keywords>;
     gl_keywordsList.insert( rootname, rlist );
   }
   else { rlist = gl_keywordsList.data( gl_keywordsList.find( rootname ) ); }

   TiXmlDocument kwdoc;
   if( ( listfile.empty( ) != true ) && ( kwdoc.LoadFile( listfile.text( ) ) != false ) ) {
     TiXmlElement *kwelem = kwdoc.RootElement( )->FirstChildElement( "Keyword" );
     for( kwelem; kwelem; kwelem = kwelem->NextSiblingElement( "Keyword" ) ) {
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
     }
   }
    else {
    std::cout << "Chyba : neni zadan keywords soubor, nebo ma chybny format" << std::endl;
    gl_change = true;
  }
  std::cout << "{ read_keywords } OK" << std::endl;
  std::cout.flush( );


}

FXbool FXGameLauncher::exec( const FXArray<const FXchar*> &args, FXbool wait, FXbool ver )
{
  /// Cesta k spoustenemu souboru (args[0]) nesmi zacinat a koncit mezerou (" ")
  /// Cesta ke spoustenemu souboru musi byt absolutni (tedy od korenoveho adresare)
  /// Navratovy kod spousteneho procesu lze zatim ziskat pouze pri wait = true
  /// Hodnota true parametru wait zablokuje celou aplikaci launcheru!

  FXint     resh   = false;
  FXint     pid    = 0;
  FXint     status = 0;
  FXProcess proc;

  if( ver == true ) {
    std::cout << "Run the process:";
    FXint num = args.no( );
    for( FXint i = 0; i != num; i++ ) { std::cout << " " << args[ i ]; }
    std::cout << "\n========================================================" << std::endl;
    std::cout << "\n";

  }

  if( ( resh = proc.start( args[ 0 ], args.data( ) ) ) == true ) {
    pid = proc.id( );
    status = WaitOnGame( &proc );
  }

  if( ver == true ) {
    if( resh == true ) {
      std::cout << "process running of " << pid << std::endl;
      std::cout << "Process exited with " << status << " exit status" << std::endl;
    }
    else { std::cout << "RUN FATAL ERROR: Process is not running!" << std::endl;}
    std::cout << "\n";
    std::cout.flush( );
  }

  return resh;
}

FXbool FXGameLauncher::parse_params( FXArray<const FXchar*> *buffer, const FXString &ar, FXbool dump ) { // Rozparsovani retezce argumentu
  /// Parametry se oddeluji znakem mezery (" ")
  /// Je-li to vyzdovano aplikaci musi byt dodreno i jejich poradi
  /// mezera za, nebo pred strednikem se predava jako soucast parametru
  /// Specialni znaky ( napr uvozovky) je nutno uvadet v zastupne notaci dle XML standardu
  FXint    start, nargs;  // Aktualni sekce, ktera predstavuje argument; Pocet parametru v retezci
  FXString section_str;   // Subsekce retezce

  if( !ar.empty( ) && ( ar != "" ) ) {
    nargs = ar.contains( " " );
    if( nargs > 0 ) {
      nargs++;
      start = 0;
      while( start < nargs ){
        section_str = ar.section( " ", start );
        if( !section_str.empty( ) ) {
          buffer->append( convert_str( section_str ) );
          start++;
        } else { break; }
      }
    }
    else {
      start = nargs = 1;
      buffer->append( convert_str( ar ) );
    }
  }

  if( dump == true ) {
    FXint num = buffer->no( );
    std::cout << "This parameter(s) setting of new process: ";
    for( FXint i = 0; i != num; i++  ) { std::cout << "\n" << i << ". " << buffer->at( i ) << " " ; }
    std::cout << "\n" << std::endl;
    std::cout.flush( );
  }

  return ( ( nargs == start ) ? true : false );
}

void FXGameLauncher::checkWindowState( )
{
  std::cout << "{ checkWindowState }" << std::endl;
  if( gl_winmode == "window" ) {
    std::cout << "Application Gorgona starting in window mode" << std::endl;
    gl_WinPos.x = getApp( )->reg( ).readIntEntry( "Window", "Position_X", 0 );
    gl_WinPos.y = getApp( )->reg( ).readIntEntry( "Window", "Position_Y", 0 );
    gl_WinSize.w = getApp( )->reg( ).readIntEntry( "Window", "Size_W", 800 );
    gl_WinSize.h = getApp( )->reg( ).readIntEntry( "Window", "Size_H", 700 );
    this->position( gl_WinPos.x, gl_WinPos.y, gl_WinSize.w, gl_WinSize.h );
    std::cout << "Window config: x = " << gl_WinPos.x << " y = " << gl_WinPos.y << " w = " << gl_WinSize.h << " h = " << gl_WinSize.h << std::endl;
  }
  if( gl_winmode == "maximize" )   {
    std::cout << "Application Gorgona starting in mximize window mode" << std::endl;
    if( this->maximize( true ) != true ) { std::cout << "Window mode maximize is not possible" << std::endl; }
  }
  if( gl_winmode == "fullscreen" ) {
    std::cout << "Application Gorgona starting in fullscreen mode" << std::endl;
    if( this->fullScreen( true ) != true ) { std::cout << "Window mode fullscreen is not possible" << std::endl;}
  }
}

void FXGameLauncher::get_arguments( StringList *list )
{
  if( list != NULL ) {
    const char *const *__args = getApp( )->getArgv( );
    for( FXint i = 0; i != getApp( )->getArgc( ); i++ ) { list->push( __args[ i ] ); }
  }
}
/*
/// FIXME: SMAZAT
FXbool FXGameLauncher::__exec( )
{
  FXGameProcess *proc;
  FXbool resh = false;

  // Novy proces a jeho spusteni
  if( ( proc = new FXGameProcess( this->get_ActiveItem( ), NULL, 0 ) ) != NULL ) {
    if( ( resh = proc->start( true ) ) == true ) {
      proc->counter( );
      gl_processlist.append( proc );
      gl_change = true;
    }
    else { delete proc; }
  }

  return resh;
}
*/

FXbool FXGameLauncher::run( FXGameItem *it )
{
  FXbool resh = false;
  FXGameItem *item = ( ( it != NULL ) ? it : get_ActiveItem( ) );
  if( item != NULL ) {
    GameProcess proc( item, NULL, 0 );
    resh = proc.start( );
    gl_change = true;
  }
  else { std::cerr << "Neplatny odkaz na hru" << std::endl; }
  return resh;
}

void FXGameLauncher::layout( )
{
  //std::cout << "{ layout }" << std::endl;
  FXGWindow::layout( );
  if( ( gl_created != false ) && (this->isMinimized( ) != true ) ) {
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

FXint FXGameLauncher::WaitOnGame( FXProcess *proc )
{
  FXint       status = 0;
  FXGameItem *it = this->get_ActiveItem( );
  std::cout << "Wait on Game status:" << it->hidel << std::endl;
  if( it->hidel == true ) {
    proc->wait( status );
  }
  return status;
}

void FXGameLauncher::version( )
{
  std::cout << "Copyright " << AutoVersion::DATE << "/" << AutoVersion::MONTH << "/" << AutoVersion::YEAR << "  D.A.Tiger <drakarax@seznam.cz>, GNU GPL 3" << std::endl;
  std::cout << "App version: "<< AutoVersion::MAJOR<< "."<< AutoVersion::MINOR << "." << AutoVersion::REVISION;
  std::cout << " [" << AutoVersion::STATUS << "]" << std::endl;
  std::cout << "lib Fox    : " << FOX_MAJOR << "." << FOX_MINOR << "." << FOX_LEVEL << std::endl;
}

/*** END ******************************************************************************************/

