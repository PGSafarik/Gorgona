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
#include "Echidna/ObjectList.h"

/*************************************************************************
* Signal.h                                                               *
*                                                                        *
* Signal-slot object communication.                                      *
* FXObjectListOf and FXObjectList                                        *
* Copyright (c) 17/05/2024 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#define SIGNAL_OK     0    // Operace notifikatoru probehla uspesne
#define SIGNAL_ERR   (-1)  // Obecna chyba signalu/slotu
#define SIGNAL_OWNER (-2)  // Tuto operaci muze provest jen vlasnik signalu
#define SIGNAL_EXIST (-3)  // Pokus o registraci jiz existujiciho signalu/slotu
#define SIGNAL_NOT   (-4)  // Pozadovany signal nebyl nalezen (neni zaregistrovan)


namespace ECHIDNA {
  class Slot;
  
  class __SIGNAL : public FXObject {
  FXDECLARE( __SIGNAL )
    ObjectListOf<Slot> *m_slots;   // Seznam pripojenych slotu
    void               *m_owner;   // Adresa objektu vlastnici signal
    FXbool              m_silence; // Rezim "silencium" - Pokud == TRUE, objekt nebude na pozadavek emitovat signal
     
  public:
  __SIGNAL( void *owner );
  ~__SIGNAL( );
  
  /* Access methods */
  int connect( Slot *slot );        // Zapojeni signalu do slotu
  int disconnect( Slot *slot );     // Odpojeni signalu ze slotu
  int disconnect( );                // Odpojeni ode vsech slotu
  
  bool silence( ) { return m_silence; } // Indikace rezimu "silencium"     
  
  /* Operations */
  int emit( void *emitor, bool change_silence = false ); // Emitovani signalu, pripadne jeho "umlceni"

  /* Messages & handlers */
  enum {
    SLOT_CONNECT = 1,   // Pripojit do slotu
    SLOT_DISCCONECT,    // Odpojit ze slotu
    SIGNAL_EMIT,        // Emitovat signal
    SIGNAL_SILENCIUM,   // Prepnout signal do rezimu "silencium"
    ID_LAST
  };
  
  long on_CmdSlot( FXObject *sender, FXSelector *sel, void *data );
  long on_CmdSignal( FXObject *sender, FXSelector *sel, void *data );  
 
  protected:
    __SIGNAL( ) { }
    
    //void  CheckSlots( );             // Check and removing empty slots
    //FXint HasSlot( FXObject *obj );  // if return value -1, object not connect        
  
  };    /* class __SIGNAL */ 
} /* namespace ECHIDNA */

