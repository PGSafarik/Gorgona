
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
#ifndef __FSMCHANGES_H
#define __FSMCHANGES_H

/*************************************************************************
* GorgonaWindow.h                                                        *
*                                                                        *
* The Finish State autoMaton, that manages the changes management in the *
* application                                                            *
* Copyright (c) 15/04/2023 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<fox-1.7/fx.h>

class FSM_Changes : public FXObject {
FXDECLARE( FSM_Changes )
  FXuint m_counter = 0;
  FXbool m_cv = false;
  
public :
  FSM_Changes( );
  virtual ~FSM_Changes( );
  
  /* Access methods */
  FXbool operator ( )( ) { return ( m_counter > 0 ); }
  
  /* Messages & handlers */
  enum {
    ID_CHANGE = 1,
    ID_ACCEPT,
    ID_DISCARD,
    ID_IGNORE,
    ID_LAST
  };

  long OnCmd_Change( FXObject *sender, FXSelector sel, void *data );
  long OnCmd_Accept( FXObject *sender, FXSelector sel, void *data ); 
  
protected :
  friend class Gorgona;  
  void SetCondition( FXbool value = true ) { m_cv = value; }
};

#endif /* __FSMCHANGES_H */

/*** END ******************************************************************************************/
