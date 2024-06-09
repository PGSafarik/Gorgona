// Property.cpp Copyright (c) 17/05/2024;  D.A.Tiger; GNU GPL 3
#include<Echidna/Property.h>

using namespace ECHIDNA;
namespace ECHIDNA {
  
FXIMPLEMENT( Property, FXObject, NULL, 0 )  
  
Property::Property( const FXString &name, FXObject *tgt, FXSelector sel )
        : m_name( name ), m_target( tgt ), m_selector( sel )
{
  m_type  = "string";
  m_value = FXString::null;
  m_label = FXString::null;    
  
  m_change = false;
  m_hash   = 0;  
}
  
Property::Property( const FXString &name, const FXString &value, FXObject *tgt, FXSelector sel, const FXString &tp, const FXString &lb )
        : m_name( name ), m_value( value ), m_type( tp ), m_label( lb ), m_target( tgt ), m_selector( sel )
{
  m_change = false;  
  ( value.empty( ) ) ? m_hash = 0 : m_hash = value.hash( );
}
  
Property::~Property( )
{ }

/**************************************************************************************************/
void Property::operator =( const FXString &value )
{
  set_value( value ); 
}

FXbool Property::operator ==( const Property &prop )
{
  if( prop.m_type == this->m_type ) {
    return prop.m_hash == this->m_hash;    
  }
  else { return false; }  
}

FXbool Property::operator !=( const Property &prop )
{
  if( prop.m_type == this->m_type ) {
    return prop.m_hash != this->m_hash;  
  }
  else { return true; }  
}

/**************************************************************************************************/
FXString Property::to_string( const FXString &separator )
{
  return ( m_name + separator + m_value );
}

void Property::from_string( const FXString &str, const FXString &separator )
{
  FXint pos = str.contains( separator );
  if( pos > 0 ) {
    m_name  = str.left( pos );
    m_value = str.right( pos + separator.length( ) );
    
    m_name.trim( );
    m_value.trim( );
  }
}

void Property::dump( )
{
  std::cout << "[ PROPERTY ] " << m_name << " = " << m_value << "\n";   
}

/**************************************************************************************************/
Properties::Properties( const FXString &lb, FXObject *tgt, FXSelector sel )
          : FXDictionaryOf<Property>( ), m_label( lb ), m_target( tgt ), m_selector( sel )
{
   m_change = false; 
}
  
Properties::~Properties( )
{ }

Property* Properties::Create( const FXString &name, const FXString &value )
{ 
   Property *n_prop = new Property( name, value, m_target, m_selector );
   return FXDictionaryOf<Property>::insert( name, n_prop ); 
}

Property*& Properties::operator [ ]( const FXString &key ) 
{ 
  if( !has( key ) ) { Create( key ); }
  return at( key );     
}
/*
Property* const & Properties::operator [ ]( const FXString &key ) const 
{ 
  
  
}*/
/**************************************************************************************************/

}; /* namespace ECHIDNA */

/**************************************************************************************************/
