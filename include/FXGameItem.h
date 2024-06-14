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

  FXString     m_id;		       // Identifikator polozky
  FXbool       hidel;	         // Skryt spoustec pri spusteni
  FXbool       m_change;       // Indikace zmeny polozky (depracated)
  FXuint       m_state;        // Indikace aktualniho stavu polozky 
  FXStringDictionary property; // Tabulka vlastnosti polozty

  FXIcon *BigIcon;
  FXIcon *SmallIcon;

  FXint   numist;    // Pocet spusteni ( bezicich instanci hry )
  FXbool  one;       // Spoustet jen jednu instanci

  FXbool  m_valid;     // 
  
  PERSEUS::Game *exec;  // Spoustec 

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

  void load( XMLElement *eitem );
  void save( XMLElement *pNode, FXbool force = false );

protected :
  XMLElement* FindMyXMLElement( XMLElement *parent, const FXString &fname = "Game" );
};

#endif /* __FXGAMEITEM_H */
/*** END ****************************************************************/
