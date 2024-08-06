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
#include<fox-1.7/fx.h>
#include<string>
#include<iostream>

#ifndef __STRING_UTILS_H
#define __STRING_UTILS_H

/*************************************************************************
* String_utils.h                                                         *
*                                                                        *
* A library of simple additional functions for working with chains       *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
namespace ECHIDNA {
  /////////////////////////////////////////////////
  /* String classes                              */ 
  class Identifier : public FXString {
  public :
    Identifier( ) : FXString( ) { }
    Identifier(const FXString& s) : FXString( s ) { }
    Identifier(const FXchar* s) : FXString( s ) { }
    Identifier(const FXnchar* s) : FXString( s ) { }
    Identifier(const FXwchar* s) : FXString( s ) { }
    Identifier(const FXchar* s,FXint n) : FXString( s, n ) { }
    Identifier(const FXnchar* s,FXint n) : FXString( s, n ) { }
    Identifier(const FXwchar* s,FXint n)  : FXString( s, n ) { }
    Identifier(FXchar c,FXint n) : FXString( c, n ) { }

    void set_key( const FXString &src = FXString::null );
    FXString get_key( );
    void set_value( const FXString &val = FXString::null );
    FXString get_value( );
    FXint verify( );
    void make( const FXString &src, const FXString &pattern = FXString::null );
  };

  /////////////////////////////////////////////////
  /* Global types                                */ 
  typedef FXArray<FXString>      StringList;
  typedef FXArray<const FXchar*> CharsList;
  typedef FXArray<Identifier>    IdentList;
  
  /////////////////////////////////////////////////
  /* String helpers functions                    */
  extern FXint    split( const FXString &str, StringList *buffer, const FXString &sep = "/" ); // Vytvori seznam subretezcu dle zadaneho separatoru, vyskytuje-li se v retezci
  extern FXString pack( const StringList &buffer, const FXString &sep = "" );                  // Spoji retezce z pole do jednoho (opak fce split( ))
  extern FXchar*  convert_str( const FXString &str );                                          // Konverze retezce FXString na FXchar[] vcetne alokace pameti
  extern void     clear_string_buffer( CharsList *buffer );                                    // Dealokace retezcu v bufferu, alokovanych pomoci convert_str( )

};     /* ECHIDNA */
#endif /*__STRING_UTILS_H*/
