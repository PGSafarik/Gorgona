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
#include<define.h>
#include<Perseus/Runnable.h>

struct FXGameItem {
  FXString     id;		         // Identifikator polozky
  FXbool       hidel;	         // Skryt spoustec pri spusteni
  FXbool       change;         // Indikace zmeny polozky
  FXStringDictionary property; // Tabulka vlastnosti polozty

  FXIcon *BigIcon;
  FXIcon *SmallIcon;

  FXint   numist;    // Pocet spusteni ( bezicich instanci hry )
  FXbool  one;       // Spoustet jen jednu instanci

  FXbool  m_valid;     // 
  
  PERSEUS::Game *exec;  // Spoustec  

  FXGameItem( Gorgona *app );
  FXGameItem( Gorgona *app, const FXString &name, const FXString &type = "native" );
  virtual ~FXGameItem( );

  ///////////////////////////////////////////////
  ///
  ///
  void dump( FXbool force = false );
  //void clear( );
  FXbool validate( );

  void   checkIcons( FXApp *app );
  const FXString read( const FXString &k ) const;
  FXbool write( const FXString &k, const FXString &v, FXbool chang = true );

  void load( XMLElement *eitem );
  void save( XMLElement *pNode, const FXString &ename = "Game" );

  FXint operator ( ) ( )
  { 
    FXString title = read( "Basic:title" );
    FXint pid = exec->run( );

    if( pid <= 0 ) {
      FXString head = "Execution Failed";
      FXString msg = "Error number: ";
      msg += FXString::value( pid ) + "\n"; 
      switch( pid ) {
        case -1 :  msg += "Game \'" + title + "\' is already launched!"; break; 
        default :  msg += "The luach of game \'" + title + "\' failed!"; break;
      }
        
      FXMessageBox::warning( (FXApp*) exec->get_app( ), MBOX_OK, head.text( ), msg.text( ) );
      std::cerr << "[GorgonaWindow::Run]: " << head << msg << std::endl; 
    }
    else  { change = true; }

    return pid;
  }

protected :

};

typedef  FXArray<FXGameItem*> FXGameItemArray;

class Library : public FXArray<FXGameItem*> {
  Gorgona     *m_app;
  FSM_Changes  m_change;

  //FXbool   m_change = false;

public :
  Library( Gorgona *app );
  virtual ~Library( );

  /* Access methods */
  FXbool isChanged( )                     { return m_change( ); }
  void   setChange( FXbool value = true ) { m_change.handle( ( FXObject *) m_app, FXSEL( SEL_COMMAND, FSM_Changes::ID_CHANGE ), NULL ); } 

  /* Operations methods */
  virtual FXbool load( XMLElement *library_el );
  virtual FXbool save( XMLElement *library_el );
};

#endif /* __FXGAMEITEM_H */
/*** END ****************************************************************/
