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
#include<define.h>
#include<Gorgona.h>
#include<Utils.h>

/*************************************************************************
* Library.h                                                              *
*                                                                        *
* Knihovna polozek                                                       *
* Copyright (c) 05/08/2023 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#ifndef __LIBRARY_H
#define __LIBRARY_H

//////////////////////////////////////////////////
/* Global buffer type                           */  
typedef  FXArray<FXGameItem*> FXGameItemArray;

//////////////////////////////////////////////////
/* Main class                                   */
class Library : public ECHIDNA::ObjectListOf<FXGameItem> {  
  Gorgona     *m_app;
  FSM_Changes  m_change;

  FXbool       m_opened; // Flag indicate open library 
  FXString     m_file;   // Name and path to actual document 
  FXString     m_elname; // The name of items tags  
  XMLDocument  m_xdoc;   // XML item Databaze 
  XMLElement  *m_xroot;  // Root element for this opened document
  XMLElement  *m_xbase;  // Element grouped by all items elemts
     
public :
  Library( Gorgona *app, const FXString &elname = "Game" );
  Library( Gorgona *app, const FXString &filename, const FXString &elname = "Game" );
  virtual ~Library( );

  /* Access methods */
  FXbool isOpen( )                        { return m_opened; } 
  FXbool isChanged( )                     { return m_change( ); }
  void   setChange( FXbool value = true ) { m_change.handle( ( FXObject *) m_app, FXSEL( SEL_COMMAND, FSM_Changes::ID_CHANGE ), NULL ); } 

  /* Operations methods */
  FXbool remove( FXGameItem *it );
  FXbool open( const FXString &filename );
  FXbool close( FXbool force = false);
  
  FXint save( );
  FXint load( ); 
  virtual FXbool load( XMLElement *library_el );
  virtual FXbool save( XMLElement *library_el );

protected:
  XMLElement* FindElementBy( XMLElement *parent, const FXString &attr, FXString &value );
};

#endif /*__LIBRARY_H */
