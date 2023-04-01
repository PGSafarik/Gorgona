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
#ifndef __FXLAUNCHEDITOR_H
#define __FXLAUNCHEDITOR_H

/*************************************************************************
* FXLauncherEditor.h                                                     *
*                                                                        *
* Dialog herni polozky (pridani, editace)                                *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<define.h>

class FXGameLauncher;
class FXGameItem;
class IconsTheme;

enum { // Ciselny seznam sekci
  SECTION_BASIC = 0,    //
  SECTION_DESCRIPTION,  //
  SECTION_ADVANCED,     //
  SECTION_EXTRA,        //
  SECTION_LAST
};

enum {
  VALUE_BASARGUMENTS = 0,   //
  VALUE_BASTITLE,           //
  VALUE_BASTIPE,            //
  VALUE_BASEXEC,            //
  VALUE_BASGENRE,           //
  VALUE_BASBACKGROUND,      //
  VALUE_DESCRIPTION,        //
  VALUE_ADVYEAR,            //
  VALUE_ADVAUTHOR,          //
  VALUE_ADVHOME,            //
  VALUE_LAST
};

class FXLaunchEditor : public FXSecondaryWindow /*FXDialogBox*/ {
FXDECLARE( FXLaunchEditor )
  // Widgety dialogu
  FXSwitcher    *le_switcher;
  FXList        *le_sections;
  FXText        *le_text;
  FXCheckButton *le_backg;
  FXCheckButton *m_term;
  FXButton      *le_Bicon;
  FXButton      *le_Micon;

  IconsTheme    *icth;

  FXArray<FXVerticalFrame*> vf_array;
  FXDictionary *values_text;
  FXString le_share;

  // Data
  FXGameItem *le_item;      // Polozka zpoustece
  FXint       label_size;   // Delka popisku polozek
  FXbool      le_change;    // indikator zmeny polozky

public :
  FXLaunchEditor( FXWindow *p, IconsTheme *icons, FXGameItem *it = NULL );
  virtual ~FXLaunchEditor( );

  /* Operations methods */
  virtual void create( );


  /* Access methods */

  /* Event messages & handlers */
  enum {
    ID_ACCEPT = FXDialogBox::ID_LAST,
    ID_STORNO,
    SECTION_SELECT,
    OPEN_FILE,
    OPEN_DIR,
    ICON_BIG,
    ICON_SMALL,

    ID_LAST
  };
  long onCmdAccept( FXObject *sender, FXSelector sel, void *data );
  long onCmdSection( FXObject *sender, FXSelector sel, void *data );
  long onCmd_Dialog( FXObject *sender, FXSelector sel, void *data );
  long onCmd_Icon( FXObject *sender, FXSelector sel, void *data );

protected :
  FXLaunchEditor( ) { }

  /* Helpful methods for sections managment */
  void make_sect( const FXString &name, const FXString &descr, FXIcon *ic = NULL );
  FXVerticalFrame* get_sect( const FXString &name );

  /* Helpful methods for generic widgets */
  void     makevalue_text( const FXString &sect, const FXString &label, const FXString &value = FXString::null );
  void     makevalue_combo( const FXString &sect, const FXString &label, const FXString &value = FXString::null, FXArray<FXString> *t = NULL );
  void     makevalue_file( const FXString &sect, const FXString &label, const FXString &value = FXString::null );
  void     makevalue_dir( const FXString &sect, const FXString &label, const FXString &value = FXString::null );
  void     makeinfo_Value( const FXString &sect, const FXString &label, const FXString &value );

  void     setvalue_text( const FXString &name, const FXString &value = FXString::null );
  FXString getvalue_text( const FXString &name );
  void     setvalue_combo( const FXString &name, const FXString &value = FXString::null );
  FXString getvalue_combo( const FXString &name );

  /* Item data rutines */
  void load( );
  void save( );
};

#endif /* __FXLAUNCHEDITOR_H */
/*** END ****************************************************************/
