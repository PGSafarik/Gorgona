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
#include <define.h>

/*************************************************************************
* Keywords.h                                                             *
*                                                                        *
* %Description of this header file%                                      *
* Copyright (c) 24/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
namespace ECHIDNA {
  // Vlastnosti asociovane s klicovym slovem
  struct GO_KeyEntry {
    FXString   description;	// Popis znacky
    FXString   actionid;		// Akce asociovana s touto znackou
    FXStringList vlist;		    // Seznam moznych hodnot
    FXint      priority;	 	// priorita pri vyhodnocovani vice tagu ( def: 0, min: -10, max 10 )
  };


  class Keywords : public FXDictionaryOf<GO_KeyEntry> {
    FXString m_description;
    FXString m_word;

  public :
    Keywords( const FXString &w = FXString::null, const FXString &d = FXString::null );
    virtual ~Keywords( );

    FXint read( XMLElement *kwelem );
    FXbool write( XMLElement *kwelem );
  };

  typedef FXDictionaryOf<FXArray<Keywords> > GO_KeyRoots;
}
/*** END ****************************************************************/
