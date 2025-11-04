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
* Boxes.h                                                                *
*                                                                        *
* Male Widgety pro Window Header                                         *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
/*** FindBar **************************************************************************************/
class FindBar : public FXHeaderBox {
FXDECLARE( FindBar )
  FXTextField *m_field;
  FXButton    *m_find_btn;  
  FXObject    *m_target;
  FXSelector   m_message;
   
public:
  FindBar( FXPrimaryWindow *p, IconsTheme *ict, FXObject *tgt, FXSelector sel, FXuint opts );
  virtual ~FindBar( ) { }

  /* Operations */
  virtual void create( );
  void clean( ) { m_field->setText( FXString::null ); } 

  /* Access methods */
  
  /* Messages & handlers */
  enum {
    ID_ACTION = FXHeaderBox::ID_LAST,
    ID_CLEAN,
    ID_LAST
  };
  long onCmd_Action( FXObject *sender, FXSelector sel, void *data );
  

protected:
  FindBar( ) { }
  long Notify( );
};

/*** ToolBar **************************************************************************************/
class ToolBar : public FXHeaderBox {
FXDECLARE( ToolBar )
  FXObject   *m_target;
  FXSelector  m_message;
  IconsTheme *m_ict;

  FXMenuButton *m_button;
  
public:
  ToolBar( FXPrimaryWindow *p, IconsTheme *ict, FXuint opts = 0 );
  virtual ~ToolBar( ) { }

  /* Operations */
  virtual void create( );
  void makeButton( const FXString &Title, const FXString &icon, FXObject *tgt, FXSelector sel );
  void makeSeparator( );

  /* Access methods */
  
  /* Messages & handlers */

protected:
  ToolBar( ) { }  
};

/*** MainBar **************************************************************************************/
class MainBar : public ToolBar {
FXDECLARE( MainBar )
  FXMenuButton *m_button;
  
public:
  MainBar( FXPrimaryWindow *p, IconsTheme *ict, FXMenuPane *menu, FXuint opts = 0 );
  virtual ~MainBar( ) { }

  /* Operations */
  virtual void create( );

  /* Access methods */
  
  /* Messages & handlers */

protected:
  MainBar( ) { }  
};

/*** END ******************************************************************************************/
