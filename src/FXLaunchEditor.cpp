// FXLauncherEditor.cpp Copyright (c) %date%;  D.A.Tiger; GNU GPL 3
#include<FXLaunchEditor.h>
#include<GorgonaWindow.h>
#include<LuaAPI.h>
#include<FXGameItem.h>
#include<Utils.h>
#include<IconsTheme.h>


#include<iostream>
#include<cstring>

/*************************************************************************************************/
FXDEFMAP( FXLaunchEditor ) LAUNCHEDITORMAP[ ] = {
  FXMAPFUNC( SEL_COMMAND, FXLaunchEditor::ID_ACCEPT, FXLaunchEditor::onCmdAccept ),
  FXMAPFUNC( SEL_COMMAND, FXLaunchEditor::ID_STORNO, FXSecondaryWindow::onCmdCancel ),
  FXMAPFUNC( SEL_COMMAND, FXLaunchEditor::SECTION_SELECT, FXLaunchEditor::onCmdSection ),
  FXMAPFUNC( SEL_COMMAND, FXLaunchEditor::OPEN_FILE, FXLaunchEditor::onCmd_Dialog ),
  FXMAPFUNC( SEL_COMMAND, FXLaunchEditor::OPEN_DIR,  FXLaunchEditor::onCmd_Dialog ),
  FXMAPFUNC( SEL_COMMAND, FXLaunchEditor::ICON_BIG,  FXLaunchEditor::onCmd_Icon ),
  FXMAPFUNC( SEL_COMMAND, FXLaunchEditor::ICON_SMALL,  FXLaunchEditor::onCmd_Icon )
};
FXIMPLEMENT( FXLaunchEditor, FXSecondaryWindow, LAUNCHEDITORMAP, ARRAYNUMBER( LAUNCHEDITORMAP ) )

/*************************************************************************************************/
FXLaunchEditor::FXLaunchEditor( FXWindow *p, IconsTheme *icons, FXGameItem *it )
                : FXSecondaryWindow( p, "Edit Game", WINDOW_STATIC, 0, 0, 660, 320  )
{
  le_item     = it;
  values_text = new FXDictionary;
  label_size  = 150;
  icth        = icons;
  le_share    = "";

  FXIcon *ic_window = icth->getIcon( "Actions/run-build-configure.png" );
  FXIcon *ic_basic  = icth->getIcon( "Actions/run-build-install.png" );
  FXIcon *ic_descr  = icth->getIcon( "Actions/documentation.png" );
  FXIcon *ic_advan  = icth->getIcon( "Actions/games-highscores.png" );
  FXIcon *ic_accept = icth->getIcon( "Actions_big/dialog-ok-apply.png" );
  FXIcon *ic_storno = icth->getIcon( "Actions_big/dialog-cancel.png" );
  FXIcon *ic_bicon = ( ( it->BigIcon != NULL ) ? it->BigIcon : icth->getIcon( "Actions_big/roll.png" ) );
  FXIcon *ic_micon = ( ( it->SmallIcon != NULL ) ? it->SmallIcon : icth->getIcon( "Actions/roll.png" ) );

  //this->setIcon( ic_window );
  FXVerticalFrame   *content      = new FXVerticalFrame( this, FRAME_NONE | LAYOUT_FILL, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0 );
  FXHorizontalFrame *content_sect = new FXHorizontalFrame( content, FRAME_GROOVE | LAYOUT_FILL );
  le_sections = new FXList( content_sect, this, FXLaunchEditor::SECTION_SELECT, FRAME_LINE | LIST_NORMAL | LAYOUT_FILL_Y | LAYOUT_FIX_WIDTH, 0, 0, 100 );
  new FXVerticalSeparator( content_sect, SEPARATOR_GROOVE | LAYOUT_FILL_Y );
  le_switcher = new FXSwitcher( content_sect, FRAME_NONE | LAYOUT_FILL, 0, 0, 0, 0,  0, 0, 0, 0 );
  make_sect( "Zakladni",  "Zakladni nastaveni spoustece", ic_basic );
  make_sect( "Popis",     "Informace",                    ic_descr );
  make_sect( "Statistiky","Statistiky a informace",       ic_advan );

  makevalue_text( "Zakladni", "Titul" );
  makevalue_text( "Zakladni", "Zanr" );
  FXArray<FXString> tlist;
  tlist.push( "native" );
  l_ReadLaunchers( &tlist );
  makevalue_combo( "Zakladni", "Spoustec", FXString::null, &tlist );
  makevalue_file( "Zakladni", "Prikaz"  );
  makevalue_dir( "Zakladni", "Pracovni adresar"  );
  le_backg = new FXCheckButton( get_sect( "Zakladni" ), "Cekat na hru", NULL, 0 );
  FXHorizontalFrame *btnfr = new FXHorizontalFrame( get_sect( "Zakladni" ), FRAME_SUNKEN | LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH | PACK_UNIFORM_HEIGHT, 0, 0, 0, 0,  0, 0, 0, 0,  2, 0 );
  btnfr->setBackColor( getApp( )->getShadowColor( ) );
  le_Micon = new FXButton( btnfr, "Mala ikona",  ic_micon, this, FXLaunchEditor::ICON_SMALL, FRAME_RAISED|ICON_ABOVE_TEXT|LAYOUT_FILL_Y );
  le_Bicon = new FXButton( btnfr, "Velka ikona", ic_bicon, this, FXLaunchEditor::ICON_BIG,   FRAME_RAISED|ICON_ABOVE_TEXT|LAYOUT_FILL_Y );

  makevalue_text( "Popis", "Autor" );
  makevalue_text( "Popis", "Domovska stranka" );
  makevalue_text( "Popis", "Rok vydani" );
  new FXLabel( get_sect( "Popis" ), "Popis a pozadavky hry:", NULL, FRAME_NONE | LAYOUT_FILL_X );
  FXVerticalFrame *textfr = new FXVerticalFrame( get_sect( "Popis" ), FRAME_LINE | LAYOUT_FILL, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0 );
  le_text = new FXText( textfr, NULL, 0, TEXT_WORDWRAP | LAYOUT_FILL );

  makeinfo_Value( "Statistiky", "Pocet spusteni",               FXString::null );
  makeinfo_Value( "Statistiky", "Naposled",                     FXString::null );
  makeinfo_Value( "Statistiky", "Celkova doba", FXString::null );

  /// vytvoreni tlacitkove listy dialogu
  new FXButton( getHeader( ), "\t\t OK",     ic_accept, this, FXLaunchEditor::ID_ACCEPT, BUTTON_TOOLBAR ); 
  new FXButton( getHeader( ), "\t\t Storno", ic_storno, this, FXLaunchEditor::ID_STORNO, BUTTON_TOOLBAR ); 

  this->load( );
}

FXLaunchEditor::~FXLaunchEditor( )
{


}

/*************************************************************************************************/
void FXLaunchEditor::create( )
{
  FXSecondaryWindow::create( ); 
}


/*************************************************************************************************/
long FXLaunchEditor::onCmdAccept( FXObject *sender, FXSelector sel, void *data )
{
  this->save( );
  return FXSecondaryWindow::onCmdAccept( sender, sel, data );
}

long FXLaunchEditor::onCmdSection( FXObject *sender, FXSelector sel, void *data )
{
  switch( FXSELID( sel ) ) {
    case FXLaunchEditor::SECTION_SELECT : {
      FXint id = le_sections->getCurrentItem( );
      if( id >= 0 ) { le_switcher->setCurrent( id ); }
      break;
    }
  }

  return 1;
}

long FXLaunchEditor::onCmd_Dialog( FXObject *sender, FXSelector sel, void *data )
{
   long resulth = 0;

   FXString identifier;
   FXId     *object = static_cast<FXId*>( sender );
   if( object != NULL ) {
     identifier = ( ( FXchar*) object->getUserData( ) );
     if( identifier.text( ) == NULL ) { std::cout << "FXLaunchEditor::onCmd_Dialog : identify FAILED" << std::endl; }
     //std::cout << "FXLaunchEditor::onCmd_Dialog : Sender identifier = " << identifier.text( ) << std::endl;
   }


   switch( FXSELID( sel ) ) {
     case FXLaunchEditor::OPEN_FILE : {
       FXFileDialog dlg( this, "Vybrat soubor" );
       if( ( dlg.execute( PLACEMENT_SCREEN ) == true ) && ( identifier != "" ) ) {
         setvalue_text( identifier, dlg.getFilename( ) );
         resulth = 1;
       }
       break;
     }
     case FXLaunchEditor::OPEN_DIR : {
       FXDirDialog dlg( this, "Vybrat adresar" );
       if( ( dlg.execute( PLACEMENT_SCREEN ) == true ) && ( identifier != "" ) ) {
         setvalue_text( identifier, dlg.getDirectory( ) );
         resulth = 1;
       }
       break;
     }
   }

   return resulth;
}

long FXLaunchEditor::onCmd_Icon( FXObject *sender, FXSelector sel, void *data )
{
  FXFileDialog dlg( this, "Vybrat soubor" );
  if( dlg.execute( PLACEMENT_SCREEN ) == true  ) {
    switch( FXSELID( sel ) ) {
      case FXLaunchEditor::ICON_BIG : {
        le_item->write( "Icon:big", dlg.getFilename( ) );
        //std::cout << "Zapisuji velkou ikonu : " << iconfile.text( ) << std::endl;
        break;
      }
      case FXLaunchEditor::ICON_SMALL : {
        le_item->write( "Icon:small", dlg.getFilename( ) );
        //std::cout << "Zapisuji malou ikonu : " << iconfile.text( ) << std::endl;
        break;
      }
    }
    le_item->checkIcons( getApp( ) );
    le_Bicon->setIcon( le_item->BigIcon );
    le_Micon->setIcon( le_item->SmallIcon );
  }

  return 1;
}


/*************************************************************************************************/
void FXLaunchEditor::make_sect( const FXString &name, const FXString &descr, FXIcon *ic )
{
  FXint sect_id;

  le_sections->appendItem( name, ic );
  sect_id = le_sections->findItem( name );
  if( sect_id >= 0 ) {
    le_switcher->setCurrent( sect_id );
    FXVerticalFrame *f = new FXVerticalFrame( le_switcher, FRAME_NONE | LAYOUT_FILL, 0, 0, 0, 0,  2, 2, 2, 2,  2, 2 );
    FXLabel *l = new FXLabel( f, descr, ic, FRAME_LINE |  JUSTIFY_LEFT | ICON_BEFORE_TEXT | LAYOUT_FILL_X );
    new FXHorizontalSeparator( f, FRAME_NONE | LAYOUT_FILL_X );
    l->setBackColor( getApp( )->getTipbackColor( ) );
    l->setTextColor( getApp( )->getTipforeColor( ) );

    vf_array.insert( sect_id, f );
  }
}

FXVerticalFrame* FXLaunchEditor::get_sect( const FXString &name )
{
  FXVerticalFrame *resh = NULL;
  FXint sect_id = le_sections->findItem( name );
  if( sect_id >= 0 ) { resh = vf_array[ sect_id ]; }
  return resh;

}

void FXLaunchEditor::makevalue_text( const FXString &sect, const FXString &label, const FXString &value )
{
  FXVerticalFrame *sect_fr = get_sect( sect );
  FXTextField      *w_value;

  if( sect_fr ) {
    FXHorizontalFrame *f = new FXHorizontalFrame( sect_fr, FRAME_NONE | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0 );
    new FXLabel( f, label, NULL, JUSTIFY_LEFT | ICON_BEFORE_TEXT | LAYOUT_FIX_WIDTH, 0, 0, label_size );
    w_value = new FXTextField( f, 50, NULL, 0, TEXTFIELD_NORMAL | LAYOUT_FILL_X );

    if( !value.empty( ) ) { w_value->setText( value ); }

    values_text->insert( label.text( ), static_cast<void*>( w_value ) );
  }
}

void FXLaunchEditor::makeinfo_Value( const FXString &sect, const FXString &label, const FXString &value )
{
  FXVerticalFrame *f_sect = get_sect( sect );

  if( f_sect ) {
    FXHorizontalFrame *f = new FXHorizontalFrame( f_sect, FRAME_NONE | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0 );
    new FXLabel( f, label + ":", NULL, JUSTIFY_LEFT | ICON_BEFORE_TEXT | LAYOUT_FIX_WIDTH, 0, 0, label_size );
    new FXLabel( f, value, NULL, JUSTIFY_LEFT | ICON_BEFORE_TEXT | LAYOUT_FIX_WIDTH, 0, 0, label_size );
  }  
}

void FXLaunchEditor::makevalue_combo( const FXString &sect, const FXString &label, const FXString &value, FXArray<FXString> *t )
{
  FXVerticalFrame *sect_fr = get_sect( sect );
  FXComboBox      *w_value;

  if( sect_fr ) {
    FXHorizontalFrame *f = new FXHorizontalFrame( sect_fr, FRAME_NONE | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0 );
    new FXLabel( f, label, NULL, JUSTIFY_LEFT | ICON_BEFORE_TEXT | LAYOUT_FIX_WIDTH, 0, 0, label_size );
    FXHorizontalFrame *cf = new FXHorizontalFrame( f, FRAME_LINE | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0 );
    w_value = new FXComboBox( cf, 5, NULL, 0, COMBOBOX_NORMAL | LAYOUT_FILL_X );
    if( t != NULL ) {
      for( FXint i = 0; i != t->no( ); i++ ) {
        //FXString key = t->at( i );
        //std::cout << key.text( ) << std::endl;
        w_value->appendItem( t->at( i ) );
      }
      w_value->setNumVisible( 5 );
    }

    if( !value.empty( ) ) { w_value->setText( value ); }

    values_text->insert( label.text( ), static_cast<void*>( w_value ) );
  }
}

void FXLaunchEditor::makevalue_file( const FXString &sect, const FXString &label, const FXString &value )
{
  FXVerticalFrame *sect_fr = get_sect( sect );
  FXTextField     *w_value;
  FXButton        *w_button;
  FXchar          *w_id = convert_str( label );

  if( sect_fr ) {
    FXHorizontalFrame *f = new FXHorizontalFrame( sect_fr, FRAME_NONE | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0 );
    new FXLabel( f, label, NULL, JUSTIFY_LEFT | ICON_BEFORE_TEXT | LAYOUT_FIX_WIDTH, 0, 0, label_size );
    w_value = new FXTextField( f, 50, NULL, 0, TEXTFIELD_NORMAL | LAYOUT_FILL_X );

    w_button = new FXButton( f, "", icth->getIcon( "Actions/document-open-data.png" ), this, FXLaunchEditor::OPEN_FILE, BUTTON_NORMAL );
    w_button->setUserData( (void*) w_id  );

    if( !value.empty( ) ) { w_value->setText( value ); }

    values_text->insert( label.text( ), static_cast<void*>( w_value ) );
  }
}

void FXLaunchEditor::makevalue_dir( const FXString &sect, const FXString &label, const FXString &value )
{
  FXVerticalFrame *sect_fr = get_sect( sect );
  FXTextField     *w_value;
  FXButton        *w_button;
  FXchar          *w_id = convert_str( label );

  if( sect_fr ) {
    FXHorizontalFrame *f = new FXHorizontalFrame( sect_fr, FRAME_NONE | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0 );
    new FXLabel( f, label, NULL, JUSTIFY_LEFT | ICON_BEFORE_TEXT | LAYOUT_FIX_WIDTH, 0, 0, label_size );
    w_value = new FXTextField( f, 50, NULL, 0, TEXTFIELD_NORMAL | LAYOUT_FILL_X );

    w_button = new FXButton( f, "", icth->getIcon( "Actions/document-open-folder.png" ), this, FXLaunchEditor::OPEN_DIR, BUTTON_NORMAL );
    w_button->setUserData( (void*) w_id  );

    if( !value.empty( ) ) { w_value->setText( value ); }

    values_text->insert( label.text( ), static_cast<void*>( w_value ) );
  }
}

void FXLaunchEditor::setvalue_text( const FXString &name, const FXString &value )
{
  FXTextField *w_value = NULL;
  FXival pos =  values_text->find( name.text( ) );

  if( pos == -1 ) {
    std::cerr << "INTERNAL FATAL FAIL: Widget on name " << name.text( ) << " its not found!" << std::endl;
  }
  else {
    w_value = static_cast<FXTextField*>( values_text->data( pos ) );
    w_value->setText( value );
  }
}

FXString FXLaunchEditor::getvalue_text( const FXString &name )
{
  FXTextField *w_value = NULL;
  FXString resh = FXString::null;
  FXival pos = values_text->find( name.text( ) );

  if( pos == -1 ) {
    std::cerr << "INTERNAL FATAL FAIL: Widget on name " << name.text( ) << " its not found!" << std::endl;
  }
  else {
    w_value = static_cast<FXTextField*>( values_text->data( pos ) );
    resh    = w_value->getText( );
  }

  return resh;
}

void FXLaunchEditor::setvalue_combo( const FXString &name, const FXString &value )
{
  FXComboBox *w_value = NULL;
  FXival pos = values_text->find( name.text( ) );

  if( pos == -1 ) {
    std::cerr << "INTERNAL FATAL FAIL: Widget on name " << name.text( ) << " its not found!" << std::endl;
  }
  else {
    w_value = static_cast<FXComboBox*>( values_text->data( pos ) );
    w_value->setText( value );
  }
}

FXString FXLaunchEditor::getvalue_combo( const FXString &name )
{
  FXComboBox *w_value = NULL;
  FXString resh       = FXString::null;
  FXival   pos        = values_text->find( name.text( ) );

  if( pos == -1 ) {
    std::cerr << "INTERNAL FATAL FAIL: Widget on name " << name.text( ) << " its not found!" << std::endl;
  }
  else{
    w_value = static_cast<FXComboBox*>( values_text->data( pos ) );
    resh    = w_value->getText( );
  }

  return resh;
}

void FXLaunchEditor::load( )
{
  if( le_item ) {
    setvalue_text( "Titul", le_item->read( "Basic:title" ) );
    setvalue_text( "Zanr", le_item->read( "Basic:genre") );
    setvalue_text( "Rok vydani", le_item->read( "Advanced:year" ) );
    setvalue_text( "Autor", le_item->read( "Advanced:author" ) );
    setvalue_text( "Domovska stranka", le_item->read( "Advanced:homepage" ) );

    setvalue_combo( "Spoustec", le_item->exec->get_launchid( ) );
    setvalue_text( "Prikaz", le_item->exec->get_command( ) );
    setvalue_text( "Pracovni adresar", le_item->exec->get_workdir( ) );

    le_text->setText( le_item->read( "Description" ) );
    le_backg->setCheck( le_item->hidel );
  }
}

void FXLaunchEditor::save( )
{
  if( le_item ) {
    le_item->write( "Basic:title", getvalue_text( "Titul" ) );
    le_item->write( "Basic:genre", getvalue_text( "Zanr" ) );
    le_item->write( "Advanced:year", getvalue_text( "Rok vydani" ) );
    le_item->write( "Advanced:author", getvalue_text( "Autor" ) );
    le_item->write( "Advanced:homepage", getvalue_text( "Domovska stranka" ) );
    le_item->write( "Description", le_text->getText( ) );

    le_item->exec->set_launchid( getvalue_combo( "Spoustec" ) );
    le_item->exec->set_command( getvalue_text( "Prikaz" ) );
    le_item->exec->set_workdir( getvalue_text( "Pracovni adresar" ) );

    le_item->hidel = le_backg->getCheck( );
  }

}

/*** END ******************************************************************************************/

