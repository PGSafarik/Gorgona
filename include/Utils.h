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
#include<fx.h>

/////////////////////////////////////////////////
// Global types
//
typedef FXArray<FXString> StringList;

/////////////////////////////////////////////////
// String helpers functions
//
extern FXint   split( const FXString &str, FXArray<FXString> *buffer, const FXString &sep = "/" ); // Vytvori seznam subretezcu dle zadaneho separatoru, vyskytuje-li se v retezci
extern FXint   pack( FXArray<FXString> *buffer, FXString *str, const FXString &sep = "" );         // Spoji retezce z pole do jednoho (opak fce split( ))
extern FXchar* convert_str( const FXString &str );                   // Konverze retezce FXString na FXchar[] vcetne alokace pameti
extern void    clear_string_buffer( FXArray<const FXchar*> *buffer ); // Dealokace retezcu v bufferu, alokovanych pomoci convert_str( )

/////////////////////////////////////////////////
// External Icons
//
extern FXIcon* loadExternIcon( FXApp *a, const FXString &file, FXint width = 16, FXint height = 16 );
extern FXIcon* createIconType( FXApp *a, const FXString &type, FXuint opts = IMAGE_DITHER | IMAGE_SHMI | IMAGE_SHMP /*| IMAGE_ALPHAGUESS*/ );

#endif /* __UTILS_H */
/*** END ****************************************************************/
