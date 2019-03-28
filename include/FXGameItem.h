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
#ifndef __FXGAMEITEM_H
#define __FXGAMEITEM_H
/*************************************************************************
* FXGameItem.h                                                           *
*                                                                        *
* Herni polozka                                                          *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/

#include<fx.h>
#include<tinyxml.h>

#include<Utils.h>


struct FXGameItem {
  FXint        nop;		 // Dosavadni pocet spusteni
  FXbool       hidel;	 // Skryt spoustec pri spusteni
  FXbool       change;   // Indikace zmeny polozky
  FXStringDictionary property; // Tabulka vlastnosti polozty

  FXIcon *BigIcon;
  FXIcon *SmallIcon;

  FXint   numist;    // Pocet spusteni ( bezicich instanci hry )
  FXbool  one;       // Spoustet jen jednu instanci

  FXGameItem( );
  FXGameItem( const FXString &name, const FXString &type = "native" );
  virtual ~FXGameItem( );

  ///////////////////////////////////////////////
  ///
  ///
  void dump( FXbool force = false );
  void clear( const FXString &name, const FXString &type = FXString::null );

  void   checkIcons( FXApp *app );
  const FXString read( const FXString &k ) const;
  FXbool write( const FXString &k, const FXString &v, FXbool chang = true );

  void load( TiXmlElement *eitem );
  void save( TiXmlElement *pNode, const FXString &ename = "Game" );

  FXbool parse( TiXmlElement *myel ); // Nacteni hodnot zpostece hry z xml elementu


  FXString operator ( ) ( const FXString &key, const FXString &value = FXString::null, FXbool change = true  )
  {
     FXString resh = FXString::null;

     if( !key.empty( ) ) {
       if( !value.empty( ) && this->write( key, value, change ) ) { resh = this->read( key ); }
       else { resh = this->read( key ); }
     }


     return resh;
  }

protected :


};

typedef  FXArray<FXGameItem*> FXGameItemArray;

#endif /* __FXGAMEITEM_H */
/*** END ****************************************************************/
