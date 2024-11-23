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
#include<define.h>

#ifndef __FXGAMEITEM_H
#define __FXGAMEITEM_H
/*************************************************************************
* FXGameItem.h                                                           *
*                                                                        *
* Herni polozka                                                          *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/

struct FXGameItem {
  Gorgona        *m_app;

  ECHIDNA::Identifier m_id;         // Item identifier
  FXString            m_name;       // Name of item
  FXString            m_type;       // Item type ( "Game", Tool, Server, DLC ... )
  FXString            m_collection; // Collection (Series) name
  FXString            m_number;     // Serial number of the item in the series
  
  
  FXbool       hidel;	         // Hide in run
  FXbool       m_change;       // Change (depracated)
  FXuint       m_state;        // Indication actual state 
  FXStringDictionary property; // Properties table

  FXIcon *BigIcon;    // Big   item icon instance
  FXIcon *SmallIcon;  // Small item icon instance

  FXint   numist;    // Number of concurrently running instances (if multi-run is enabled) 
  FXbool  one;       // true disabeld multi-run (default)
  FXbool  m_valid;     // flag of validation status
  
  PERSEUS::Game *exec;  // Trigger for this item

  /* Item state flags */
  enum {  
    STATE_NORMAL = 0,  // Default state
    STATE_NEW,         // New item  
    STATE_CHANGE,      // This item is changed
    STATE_REMOVE,      // This item has be removed
    STATE_LAST
  };

  FXGameItem( Gorgona *app );
  FXGameItem( Gorgona *app, const FXString &name, const FXString &type = "native" );
  virtual ~FXGameItem( );

  ///////////////////////////////////////////////
  /* Overloaded operators                      */
  FXint operator ( ) ( );
  
  ///////////////////////////////////////////////
  /* Access methods                            */
  FXString  get_id( ) { return m_id; }
  void      set_id( const FXString &value );

  ///////////////////////////////////////////////
  /* Operations methods                        */
  void dump( FXbool force = false );
  FXbool validate( );
  void   checkIcons( );
  const FXString read( const FXString &k ) const;
  FXbool write( const FXString &k, const FXString &v, FXbool chang = true );
    
  void   load( XMLElement *eitem );
  FXbool save( XMLElement *x_record, FXbool force = false );

  /*Temporarily*/
  void __T_load( XMLElement *x_record );
  void __T_save( XMLElement *x_record, FXbool force = false );

protected :
};

#endif /* __FXGAMEITEM_H */
/*** END ****************************************************************/
