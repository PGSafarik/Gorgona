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
#ifndef __BOXES_H
#define __BOXES_H
/*************************************************************************
* Boxes.h                                                                *
*                                                                        *
* Male Widgety pro Window Header                                         *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<fox-1.7/fx.h>
#include<fxghi.h>

#include<IconsTheme.h>

/*** FindBar **************************************************************************************/
class FindBar : public FXHeaderBox {
FXDECLARE( FindBar )
  FXTextField *m_field;

  FXObject   *m_target;
  FXSelector  m_message;
   
public:
  FindBar( FXWindowHeader *p, IconsTheme *ict, FXObject *tgt, FXSelector sel, FXuint opts );
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
  //long onCmd_Clean(  FXObject *sender, FXSelector sel, void *data );

protected:
  FindBar( ) { }
  long Notify( );
};

#endif /*__BOXES_H */

/*** END ****************************************************************/
