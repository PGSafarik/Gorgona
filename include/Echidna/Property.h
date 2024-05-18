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

#ifndef __PROPERTY_H
#define __PROPERTY_H
/*************************************************************************
* Property.h                                                             *
*                                                                        *
* Small class for data value.                                            *
* FXObjectListOf and FXObjectList                                        *
* Copyright (c) 17/05/2024 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<define.h>

namespace ECHIDNA {
  
class Property : public FXObject {
FXDECLARE( Property )  
  FXString m_name;    // Nazev vlastnosti
  FXString m_value;   // Hodnota 
  FXString m_type;    // Typ (string, Number, boolean)    
  FXString m_label;   // Popisek
  
  FXbool  m_change;   // flag for property change
  FXuint  m_hash;     // Hash value of m_value
  
  FXObject   *m_target;   // Target object for notification    
  FXSelector  m_selector; // Notification message
  
public:
  Property( const FXString &name = FXString::null, FXObject *tgt = NULL, FXSelector sel = 0 );
  Property( const FXString &name, const FXString &value, FXObject *tgt = NULL, FXSelector sel = 0, const FXString &tp = FXString::null, const FXString &lb = FXString::null );
  virtual ~Property( );
  
  /* Access methods */ 
  void   set_name( const FXString& name )      { m_name = name; m_change = true; }   
  const  FXString& get_name( ) const           { return m_name; }
  void   set_type( const FXString& type_str )  { m_type = type_str; m_change = true; }
  const  FXString& get_type( ) const           { return m_type; }
  void   set_value( const FXString& value )    { m_value = value; m_change = true; m_hash = m_value.hash( ); }
  const  FXString& get_value( ) const          { return m_value; }
  void   set_label( const FXString& text )     { m_label = text; m_change = true; }
  const  FXString& get_label( ) const          { return m_label; }
  FXuint get_hash( )                           { return m_hash;  }
  
  /* operators */
  virtual void operator =( const FXString &value );
  virtual FXbool operator ==( const Property &prop );
  virtual FXbool operator !=( const Property &prop );
  
  /* operations */
  FXString to_string( const FXString &separator = "=" );
  void from_string( const FXString &str, const FXString &separator = "=" );
  void dump( );
  
};      /* class Property */    

};     /* namespace ECHIDNA */
#endif /* __PROPERTY_H */
