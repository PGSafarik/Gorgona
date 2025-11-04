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

/*************************************************************************
* IconsTheme.h                                                           *
*                                                                        *
* Trida obsluhujici nacitani ikon                                        *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
typedef  FXDictionaryOf<FXIconCache> impl;

class IconsTheme : public impl
{
  FXbool    it_creating;
  FXString  it_size;    // Velikost ikon
  FXString  it_folder;  // Defaultni slozka
  FXApp    *it_app;     // Ukazatel na instanci aplikace

public:
  IconsTheme( FXApp *a, const FXString &folder );
  virtual ~IconsTheme( );

  virtual void create( ) { it_creating = true; }
  void insert( const FXString &key, const FXString &dir );
  void replace( const FXString &key, const FXString &dir );
  FXIconCache* getCache( FXival pos );
  FXIconCache* getCache( const FXString &key );
  void dump( );

  inline FXIcon* getIcon( const FXString &type_key, const FXString &icon_key )
  {
    FXIcon      *icon  = NULL;
    FXIconCache *cache = NULL;
    //FXString     dbg_text = "";
    FXString     pth;

    if( ( cache = getCache( type_key ) ) == NULL ) {
	  std::cout << "ERROR:  Cache for "	<< type_key.text( ) << " NOT EXISTING!!!" << std::endl;
	  return NULL;
    }
    pth = cache->getIconPath( );
    //std::cout << pth.text( ) << " (" << type_key.text( ) << ", " << icon_key.text( ) << ")" << std::endl;
    icon = cache->find( icon_key.text( ) );

    if( !icon ) {
      //dbg_text = "Icon search false! ";
      pth += "/" + icon_key;

	  if( FXStat::exists( pth ) ) {
        icon = cache->insert( icon_key.text( ) );
	    //if( !icon ) { dbg_text += "Load icon corupted"; }
        if( it_creating == true ) { icon->create( ); }
	  }
	  else {
		std::cout << "ERROR:  path " << pth.text( ) << " NOT EXISTING!!!" << std::endl;
		return NULL;
	  }
 	}

    //std::cout << dbg_text.text( ) << std::endl;
    return icon;
  }

  inline FXIcon* getIcon( const FXString &ident )
  {
     return getIcon( ident.section( "/", 0, 1 ), ident.section( "/", 1, 1 ) );
  }

  FXIcon *operator []( const FXString &ident )
  {
     return getIcon( ident.section( "/", 0, 1 ), ident.section( "/", 1, 1 ) );
  }

protected:
  IconsTheme( ) { }
};
/*** END ****************************************************************/
