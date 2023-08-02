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
#ifndef __UTILS_H
#define __UTILS_H

/*************************************************************************
* Utils.h                                                                *
*                                                                        *
* Knihovna jednoduchych funkci pro ruzne uziti v ramci projektu          *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<define.h>

/////////////////////////////////////////////////
// Global types
//
typedef FXArray<FXString> StringList;
typedef FXArray<const FXchar*> EntriesList;

/////////////////////////////////////////////////
// String helpers functions
//
extern FXint   split( const FXString &str, FXArray<FXString> *buffer, const FXString &sep = "/" ); // Vytvori seznam subretezcu dle zadaneho separatoru, vyskytuje-li se v retezci
extern FXint   pack( FXArray<FXString> *buffer, FXString *str, const FXString &sep = "" );         // Spoji retezce z pole do jednoho (opak fce split( ))
extern FXchar* convert_str( const FXString &str );                   // Konverze retezce FXString na FXchar[] vcetne alokace pameti
extern void    clear_string_buffer( FXArray<const FXchar*> *buffer ); // Dealokace retezcu v bufferu, alokovanych pomoci convert_str( )

//////////////////////////////////////////////////
/* Aditional string (conversion) operators      */
extern FX::FXString& operator <<( FX::FXString &dest, const std::string &source );
extern std::string&  operator <<( std::string &dest, const FX::FXString &source );
extern std::ostream& operator <<( std::ostream &store, const FX::FXString &str );
extern std::istream& operator >>( std::istream &store, FX::FXString &str );
//extern FX::FXString& operator =( const std::string &str );      

/////////////////////////////////////////////////
/* External Icons                              */
extern FXIcon* loadExternIcon( FXApp *a, const FXString &file, FXint width = 16, FXint height = 16 );
extern FXIcon* createIconType( FXApp *a, const FXString &type, FXuint opts = IMAGE_DITHER | IMAGE_SHMI | IMAGE_SHMP /*| IMAGE_ALPHAGUESS*/ );

//////////////////////////////////////////////////
/* More helpers function                        */
extern void Welcome( FXApp *app );

//////////////////////////////////////////////////
/* Terminal profile struct                      */
struct TermInfo {
  FXString name;      // Nazev, identifikator terminalu
  FXString exec;      // Spustitelny soubor emulatoru
  FXString p_run;     // Parametr predavajici prikaz ke spusteni v terminalu
  FXString p_noclose; // Parametr zabranujici ukonceni emulatoru po provedeni prikazu
  FXString p_workdir; // Parametr, umoznujici (u nekterych) emulatoru terminalu zmenit pracovni adresar (Gorgona jej nevyuziva)
};

//////////////////////////////////////////////////
/* The Signal notify object                     */
struct GSlot {
  FXbool    clear_me; // Priznak, ze ma byt slot odklizen z pameti (neni-li uz potrebny) 
  FXuint    msg_id;   // ID zpravy, kterou signal odesle konkretnimu cily
  FXObject *target;   // cilovy objekt slotu

  GSlot( FXObject *target, FXuint message, FXbool cls = true )
  {
    this->msg_id   = message;
    this->target   = target;
    this->clear_me = cls;
  }
  virtual ~GSlot( ) { }
  long send( FXObject *sender, FXuint msg_type, void *data )
  {
    long res = 1; 
    std::cout << "[GSlot] Send signal \n"; 
    if( target ) { res = target->tryHandle( sender, FXSEL( msg_type, msg_id ), data ); }
    return res;
  }
}; 

class GSignal : public FXObject {
FXDECLARE( GSignal )
  FXArray<GSlot* > m_slots;      // Seznam pripojenych slotu 
  FXObject        *m_emitor;     // Objekt, ktery bude predan jako odesilatel
  FXuint           m_type;       // Typ zpravy, kterou bude objektum odesilana    

public :
  GSignal( FXObject *owner = NULL, FXuint msg_type = SEL_SIGNAL );
  virtual ~GSignal( );

  /* Access */
  FXuint get_type( )                { return m_type; }
  void   set_type( FXuint type_id ) { m_type = type_id; }
 
  /* operations */
  FXbool connect( FXObject *target, FXuint message = 0 );
  FXbool connect( GSlot *slot );
  FXbool disconnect( FXObject *target );
  FXbool disconnect( GSlot *slot );

  int emit( void *data  = NULL );
protected :

};

#endif /* __UTILS_H */
/*** END ****************************************************************/
