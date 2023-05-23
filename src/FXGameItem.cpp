// FXGameItem.cpp Copyright (c) 24/03/2019; D.A.Tiger; GNU GPL 3
#include<FXGameItem.h>
#include<Perseus/Runnable.h>
#include<Utils.h>

#include<iostream>

FXGameItem::FXGameItem( Gorgona *app )
{
  this->hidel     = false;
  this->change    = false;
  
  this->BigIcon   = NULL;
  this->SmallIcon = NULL;

  this->exec = new PERSEUS::Game( app );
}

FXGameItem::FXGameItem( Gorgona *app, const FXString &name, const FXString &type )
{
  this->hidel     = false;
  this->change    = false;
  
  this->BigIcon   = NULL;
  this->SmallIcon = NULL;
  
  property.clear( );
  property.insert( "Basic:title", name.text( ) );

  exec = new PERSEUS::Game( app );
  exec->set_launchid( type );
}

FXGameItem::~FXGameItem( )
{
  this->property.clear( );
  if( this->BigIcon   != NULL ) { delete this->BigIcon; }
  if( this->SmallIcon != NULL ) { delete this->SmallIcon; }
}
/*
void FXGameItem::clear( )
{
  this->property.clear( );
}
*/

void FXGameItem::dump( FXbool force )
{
  
  if( force == false ) {
    std::cout << "Game entry [" << exec->get_launchid( ) << "] = " << this->property[ "Basic:title" ] << std::endl;
    std::cout.flush( );
  }
  else {
    std::cout << "Game entry "          << this->property[ "Basic:title" ] << std::endl;
    std::cout << "Change: "             << this->change << std::endl;
    std::cout << "Hide GUI ON: "        << this->hidel << std::endl;

    for( FXival i = 0; i < this->property.no( ); i++ ) {
      FXString k = this->property.key( i );
      if( k.empty( ) ) { continue; }
      else { std::cout << k.text( ) << ": " << this->property.at( k ).text( ) << std::endl; }
    }
      
    exec->dump( );
  }
  
}

FXbool FXGameItem::validate( )
{
  FXString text =  "VALIDATION: The entry \"" + property[ "Basic:title" ]; 
           text += "\" ";
/*
  if( property[ "Basic:type" ] == "native" ) {
    FXString exec = property[ "Basic:exec" ];
    if( !exec.empty( ) ) {
      if( !FXStat::exists( exec ) ) {
        std::cout << text << "corrupted: The exec command " << exec << " NOT exists!" << std::endl;
        m_valid = false;
      } 
    }
    else {
      std::cout << text << "corrupted: Not have set a exec command!" << std::endl;
      m_valid = false;
    }
  }
*/ 
  m_valid = exec->validation( ); 
  if( m_valid ) { std::cout << text << "OK." << std::endl; }
  return m_valid;
}

FXbool FXGameItem::write( const FXString &k, const FXString &v, FXbool chang )
{
   FXbool resh = false;

   if( this->property.find( k.text( ) ) == -1 ) {
     this->property.insert( k.text( ), v.text( ) );
     resh = true;
   }
   else {
     this->property.remove( k.text( ) );
     this->property.insert( k.text( ), v.text( ) );
     resh = true;
   }

   if( resh ) {
     this->change = chang;
   }
   return resh;
}

void FXGameItem::load( XMLElement *eitem )
{
  FXString _name, _value, _sect, _key;

  for( XMLElement *elem = eitem->FirstChildElement( ); elem; elem = elem->NextSiblingElement( ) ){
    _sect = elem->Value( );
    if( _sect.find( ":" ) >= 0 ) { continue; } // Pokud jde o data jineho modulu, preskocit
 
    // Osetreni elementu, se kterymi je potreba nakladat zvlastnim zpusobem :
    if( _sect == "Description" ) {
      this->write( _sect.text( ), elem->GetText( ), false );
      continue;
    }

    // Zpracovani atributu elementu :
    for( const XMLAttribute *attr = elem->FirstAttribute( ); attr; attr = attr->Next( ) ) {
      _name  = attr->Name( );
      _value = attr->Value( );

       // Vsechny ostatni hodnoty jsou zaklicovany do cache vlastnosti:
      _key = _sect + ":" + _name;
      this->write( _key, _value, false );
    } 
  }

  exec->load( eitem ); 
  
  #ifdef __DEBUG
   dump( );
  #endif

  validate( );
}

const FXString FXGameItem::read( const FXString &k ) const
{
  FXString s = FXString::null;
  FXival pos = 0;
  if( ( pos = this->property.find( k.text( ) ) ) != -1 ) { s = this->property.data( pos ); }
  return s;
}

void FXGameItem::save( XMLElement *pNode, const FXString &ename )
{
  #ifdef __DEBUG
  std::cout << "Writing the Game item : " << read( "Basic:title" ).text( ) << std::endl;
  //std::cout.flush( );
  #endif

  //XMLText    *e_text  = NULL;                                           // Element popisoveho textu
  XMLElement *e_desc  = NULL;                                             // Nosny element popisu
  XMLElement *e_tmp   = NULL;                                             // Pomocny element
  XMLElement *e_self  = pNode->InsertNewChildElement( ename.text( ) );    // Element reprezentujici (herni) polozku

  FXString key, value;
  for( FXival i = 0; i < this->property.no( ); i++ ) {
    key   = this->property.key( i );
    if( key.empty( ) ) { continue; }
    value = this->property.data( i );

    // Zapis elementu, ktere je vyhodnejsi samostane osetrit
    if( key == "Description" ) {
      e_desc = e_self->InsertNewChildElement( key.text( ) );
      /*XMLText *e_text = */e_desc->InsertNewText( value.text( ) );
      // std::cout << key.text( ) << "->TEXT = " << value.text( ) << std::endl;
    }
    else {
      FXString CHName  = key.section( ":", 0 );
      FXString CHParam = key.section( ":", 1 );
      // std::cout << CHName.text( ) << "->" << CHParam.text( ) << " = " << value.text( ) << std::endl;
      e_tmp = e_self->FirstChildElement( CHName.text( ) );
      if( e_tmp == NULL ) { e_tmp = e_self->InsertNewChildElement( CHName.text( ) ); }
      e_tmp->SetAttribute( CHParam.text( ), value.text( ) );
    }
  }

  /// FIXME GAMEITEM_001: force!
  exec->save( e_self, true );
}

void FXGameItem::checkIcons( FXApp *app )
{
  FXString file;
  FXint sms = 22; // smal size
  FXint bis = 32; // big size

  if( this->BigIcon != NULL ) { delete this->BigIcon; }
  if( this->SmallIcon != NULL ) { delete this->SmallIcon; }

  if( ( file = this->read( "Icon:all" ) ) != FXString::null ){
    this->SmallIcon = loadExternIcon( app, file, sms, sms );
    this->BigIcon   = loadExternIcon( app, file, bis, bis );
    return;
  }
  if( ( file = this->read( "Icon:small" ) ) != FXString::null ){
    this->SmallIcon = loadExternIcon( app, file, sms, sms );
  }
  if( ( file = this->read( "Icon:big" ) ) != FXString::null ){
    this->BigIcon = loadExternIcon( app, file, bis, bis );
  }
}

/**************************************************************************************************/
Library::Library( Gorgona *app )
{
  m_app = app;
}

Library::~Library( )
{



}

FXbool Library::load( XMLElement *library_el )
{
  FXbool result = false; 
   
  if( library_el ) {
    for( XMLElement *el = library_el->FirstChildElement( "Game" ); el; el = el->NextSiblingElement( "Game" ) ) {
      FXGameItem *item = new FXGameItem( m_app );    
      if( item ) {
        item->load( el );
        item->checkIcons( ( FXApp*) m_app );  // ?  FIXME ItemList_001: FXGameItem::CheckIcons muze (ted uz) volat i metoda FXGameItem::Load( )
        push( item );
      }
      else { std::cout << "[WARNING ItemList::Load( )] : Unable to create a trigger item" << std::endl; }  
      result = true;
    }
  } /* Knihovna (jeste) nevytvorena. Prvni spusteni? */ 

  return result; 
}

FXbool Library::save( XMLElement *library_el )
{
  FXbool result = false;

  if( library_el && m_change( ) ) {
    FXint num = no( );
    for( FXint i = 0; i != num; i++ ) { at( i )->save( library_el ); }
  }

  return result;
}

/*** END ******************************************************************************************/




































