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
//#include<define.h>

#ifndef __ITEMBASE_H
#define __ITEMBASE_H
/*************************************************************************
* ItemBase.h                                                             *
*                                                                        *
* Small class template replacing the non-conforming classes              *
* FXObjectListOf and FXObjectList                                        *
* Copyright (c) 05/05/2018 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
namespace ECHIDNA {

//class Gorgona;

enum {
  SEL_CHANGE_INCR = SEL_LAST,
  SEL_CHANGE_DECR,
  SEL_LAST
};


class ItemBase : public FXObject {
FXDECLARE( ItemBase )
  // <TYPE param1="data" param="data" />
  Gorgona            *m_app;      // Ukazatel na instanci aplikace 
  FXString            m_type;      // Typ polozky   ( "Game", "tool", "label" )  
  FXStringDictionary  m_property;  // data polozky ve stylu: pozice -> klic = hodnota
  
  /* Control State automation */
  FXuint    m_csa_state;           // Cele kladne cislo, reprezentujici aktualni stav polozky  
  FXuint    m_csa_trans[ 5 ][ 5 ]; // Seznam moznych prechodu automatu
  FXuint    m_csa_ends[ 2 ];       // Seznam koncovych stavu
  FXbool    m_csa_sleep;           // Pozastaveni automatu v urcitem stavu - POZOR velmi zakarne!
  void   SleepStateMat( FXbool s ) { m_csa_sleep = s; } /* Pozastavi stavovy automat - POZOR: VELMI NEBEZPECNE A ZRADNE, opravdu osklivy workarround */
  
  /* notifications */
  FXObject   *m_target;
  FXSelector  m_msg;
  
public :
  ItemBase( Gorgona *a, const FXString &type = "UNKNOWN" );    
  virtual ~ItemBase( );
  
  /*** CSA States flags ***/
  enum {
    CSA_STATE_READY = 0, // Normalni stav ( prvni koncovy stav )
    CSA_STATE_EMPTY,     // Polozka je prazdna (vychozi stav a druhy koncovy stav) 
    CSA_STATE_MODIFY,    // Vnitrni hodnoty polozky byly zmeneny
    CSA_STATE_ACTIVE,    // Polozka je aktivni (napr. bezici proces hry)
    CSA_STATE_REMOVED,   // Polozka bude odstranena
    CSA_STATE_LAST,      // Info o  Item State automation viz. slozka tmp/Diagrams
  };

  /*** Access methods ***/
  Gorgona*   get_app( ) { return m_app; }
  FXuint     get_state( ) { return m_csa_state; }
  FXbool     set_property( const FXString &key, const FXString &value ); 
  FXString   get_property( const FXString &name );
  FXObject*  get_target( ) { return m_target; }
  void       set_object( FXObject *tgt ) { m_target = tgt; };
  FXSelector get_message( ) { return m_msg; }  
  void       set_message( FXSelector msg ) { m_msg = msg; }
  const FXString get_type( ) const { return m_type; }
  
  /* Operations */
  FXint  load( XMLElement *store );
  FXint  save( XMLElement *store );
  FXbool clear( FXbool force = false );
  virtual void dump( FXbool force = false, FXuint distance = 0 );

protected: 
  ItemBase( ) { }
 
  /* internal Control State Automaton methods */
  void InitStateMat( );
  /* Toto je kriticka funkce interniho stavoveho automatu Provadi kontrolu zda je mozno
   * v dane situaci prejit do jineho konkretniho stavu.  
   * notify = 0 - zakaza notifikazi uspesneho prechodu.  */
  FXbool CheckState( FXuint svalue, FXuint notify = SEL_CHANGED, FXbool enable_same = false ); 
  
  /*** internal I/O methods ***/
  virtual FXint _Save( XMLElement  *my_element ) { return 0; }
  virtual FXint _Load( XMLElement  *my_element ) { return 0; }
  virtual FXint _Erase( XMLElement *my_element ) { return 0; }  
  FXbool  SetProperty( const FXString &key, const FXString &value );
  long    SendNotify( FXuint msg_type, void *msg_data = NULL );
};      /* class ItemBase    */

};      /* namespace ECHIDNA */
#endif  /* __ITEMBASE_H      */

/*** END ******************************************************************************************/
