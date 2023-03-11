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
#ifndef __FXLAUNCHERLUA_H
#define __FXLAUNCHERLUA_H

/*************************************************************************
* FXLauncherLua.h                                                        *
*                                                                        *
* Data a funkce pro moduly spoustecu (Lua)                               *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<lua5.1/lualib.h>
#include<lua5.1/lauxlib.h>
#include<fox-1.7/fx.h>
#include<iostream>
#include<cstring>

//class GorgonaWindow;
class Gorgona;

// Zaklad skriptovaciho jazyka Lua
FXbool     l_open( Gorgona *app );
void       l_close( );

// Pomocne rutiny
FXbool l_Script( const FXString &script );
void   l_Error( const FXString &str );
void   l_ErrorMessage( FXint num, const FXString &msg = FXString::null );
void   l_TableWrite_str( lua_State *L, const FXString &key, const FXString &value ); // t[key] = value
void   l_TableWrite_num( lua_State *L, const FXString &key, FXint value );
FXint  luax_pushStringArray( const FXArray<FXString> &array );
int    l_ReadLaunchers( FXArray<FXString> *keylist );

// Modules services 
FXString luams_launch( const FXString &module_id, const FXArray<FXString> &prms );                                        // module_id.launcher( prms[] ): staus 
FXint    luams_validate( const FXString &module_id, const FXString &flag_str, const FXString &entry_name, FXint *result ); // module_id.validate( flag_str, entry_name ): status, result 

// Dialogy
FXString inputBox( const FXString &label, const FXString &title = "Vlozte, prosim, pozadovane udaje:" );
void     output( const FXString &value, FXbool nonl = false );


/* Zprava kolekce
void makeNewItem( ) { }       // Vytvoreni a zarazeni nove polozky
void findItem( )    { }       // Vrati identifikatory polozek, ktere se shoduji se zadanym(i) parametrem(/Y)
void setItemProperty( ) { }   //
void getItemProperty( ) { }   //
*/
#endif /* __FXLAUNCHERLUA_H */
/*** END ****************************************************************/
