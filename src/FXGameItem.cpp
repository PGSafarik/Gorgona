// FXGameItem.cpp Copyright (c) 24/03/2019; D.A.Tiger; GNU GPL 3
#include<FXGameItem.h>
#include<iostream>

FXGameItem::FXGameItem( )
{
  this->hidel     = false;
  this->change    = false;
  this->nop       = 0;
  this->BigIcon   = NULL;
  this->SmallIcon = NULL;
}

FXGameItem::FXGameItem( const FXString &name, const FXString &type )
{
  this->hidel     = false;
  this->change    = false;
  this->nop       = 0;
  this->BigIcon   = NULL;
  this->SmallIcon = NULL;
  clear( name, type );
}

FXGameItem::~FXGameItem( )
{
  this->property.clear( );
  if( this->BigIcon   != NULL ) { delete this->BigIcon; }
  if( this->SmallIcon != NULL ) { delete this->SmallIcon; }
}

void FXGameItem::clear( const FXString &name, const FXString &type )
{
  this->property.clear( );
  this->property.insert( "Basic:title", name.text( ) );
  this->property.insert( "Basic:type", ( type.empty( ) ? "native" : type.text( ) ) );
}

void FXGameItem::dump( FXbool force )
{
  if( force == false ) {
    std::cout << "Game entry [" << this->property.find( "Basic:type" ) << "] = " << this->property.find( "Basic:title" ) << std::endl;
    std::cout.flush( );
  }
  else {
    std::cout << "Game entry "         << this->property.find( "Basic:title" ) << std::endl;
    std::cout << "Change: "            << this->change << std::endl;
    std::cout << "Hide GUI ON: "        << this->hidel << std::endl;
    std::cout << "Nuber of execution: " << this->nop << std::endl;
    /*
    for( FXint i = this->property.first( ); i <= this->property.last( ); i = this->property.next( i  ) ) {
       std::cout << this->property.key( i ) << ": " << this->property.data( i ) << std::endl;
    }
    */
    for( FXival i = 0; i < this->property.no( ); i++ ) {
      FXString k = this->property.key( i );
      if( k.empty( ) ) { continue; }
      else { std::cout << k.text( ) << ": " << this->property.at( k ).text( ) << std::endl; }
    }
    std::cout << std::endl;
  }

}

FXbool FXGameItem::write( const FXString &k, const FXString &v, FXbool chang )
{
   FXbool resh = false;

   if( this->property.find( k.text( ) ) == -1 ) {
     //if( this->property.insert( k.text( ), v.text( ) ) != NULL ) { resh = true; }
     this->property.insert( k.text( ), v.text( ) );
     resh = true;
   }
   else {
     //if( this->property.replace( k.text( ), v.text( ) ) != NULL ) { resh = true; }
     this->property.remove( k.text( ) );
     this->property.insert( k.text( ), v.text( ) );
     resh = true;
   }

   if( resh ) {
     //std::cout << "writing[ " << k.text( ) << "] = " << v.text( ) << std::endl;
     this->change = chang;
   }
   return resh;
}

FXbool FXGameItem::parse( TiXmlElement *myel )
{
  FXString _sect, _key, _value;
  TiXmlElement   *el = NULL;
  TiXmlAttribute *at = NULL;

  // kontrola elementu hry
  if( myel->Value( ) != "Game" ) { return false; }
  el = myel;

  // Cyklus nacitani dat
  while( el != NULL ) {
    _sect = el->Value( );

    // Precteni klicu a hodnot elementu daneho elementu
    if( ( at = el->FirstAttribute( ) ) != NULL ) {
      for( at; at; at = at->Next( ) ) {
        _key   = _sect + ":" + at->Name( );
        _value = at->Value( );

        // Nektere hodnoty je nutno osetrit zvlast
        if( _key == "Run:type"  ) {
          if( _value.empty( ) ) { _value = "native"; }
        }
        if( _key == "Run:backg" ) {
          this->hidel = ( ( _value.empty( ) or ( _value == "false" ) ) ? false : true );
          continue;
        }
        if( _key == "Stats:NumberOfPlays" ) {
          this->nop = ( _value.empty( ) ? 0 : _value.toInt( ) );
          continue;
        }

        // Ostatni se zapisou rovnou do hasmapy
        this->write( _key, _value ); //}
      }
    }

    // Zpracovani elementu - sekci
    if( _sect == "Deskription" ) { this->write( _sect, el->GetText( ), false ); }
    if( _sect == "Game" ) { el = el->FirstChildElement( ); } else { el = el->NextSiblingElement( ); }
  }

  return true;
}

void FXGameItem::load( TiXmlElement *eitem )
{
  FXbool   _set_of_type = ( ( this->property.find( "type" ) != -1 ) ? true : false );
  FXString _name, _value, _sect, _key;
  TiXmlElement   *elem = NULL;
  TiXmlAttribute *attr = NULL;

  if( ( elem = eitem->FirstChildElement( ) ) == NULL ) { return; }
  for( elem; elem; elem = elem->NextSiblingElement( ) ){
    _sect = elem->Value( );
    // Osetreni elementu, se kterymi je potreba nakladat zvlastnim zpusobem :
    if( _sect == "Description" ) {
      this->write( _sect.text( ), elem->GetText( ), false );
      continue;
    }

    // Zpracovani atributu elementu :
    if( ( attr = elem->FirstAttribute( ) ) == NULL ) { continue; }
    for( attr; attr; attr = attr->Next( ) ) {
      _name  = attr->Name( );
      _value = attr->Value( );

      // Zpracovani atributu elementu, ktere vyzaduji zvlastni zachazeni :
      if( _name == "background" ) {
        this->hidel = ( ( _value.empty( ) or ( _value == "false" ) ) ? false : true );
        continue;
      }
      if( _name == "numberOfPlays" ) {
        this->nop = ( _value.empty( ) ? 0 : _value.toInt( ) );
        continue;
      }
      if( _name == "type" ) {
        if( _value.empty( ) == true ) { _value = "native"; }
        _set_of_type = true;
      }
      if( _name == "binary" ) { _name = "exec"; }

      // Vsechny ostatni hodnoty jsou zaklicovany do cache vlastnosti:
      _key = _sect + ":" + _name;
      this->write( _key, _value, false );
    }
  }
  // Defaultni nastaveni typu spoustece
  if( _set_of_type == false ) { this->write( "Basic:type", "native", false ); }
}

const FXString FXGameItem::read( const FXString &k ) const
{
  FXString s = FXString::null;
  FXival pos = 0;
  if( ( pos = this->property.find( k.text( ) ) ) != -1 ) { s = this->property.data( pos ); }
  return s;
  //std::cout << "FXGameItem::read( key ): Key = " << k.text( ) << " value = " << resh.text( ) << std::endl;
  //return resh;
}

void FXGameItem::save( TiXmlElement *pNode, const FXString &ename )
{
  //std::cout << "Writing the Game item : " << read( "Basic:title" ).text( ) << std::endl;
  //std::cout.flush( );

  TiXmlElement *e_desc  = NULL;
  TiXmlElement *e_tmp = NULL;
  TiXmlText    *e_text  = NULL;
  TiXmlElement *e_self  = new TiXmlElement( ename.text( ) );
  pNode->LinkEndChild( e_self );

  FXString key, value;
  //for( FXint i = this->property.first( ); i <= this->property.last( ); i = this->property.next( i ) ){
  for( FXival i = 0; i < this->property.no( ); i++ ) {
    key   = this->property.key( i );
    if( key.empty( ) ) { continue; }
    value = this->property.data( i );

    // Zapis elementu, ktere je vyhodnejsi samostane osetrit
    if( key == "Description" ) {
      e_desc = new TiXmlElement( key.text( ) );
      e_text = new TiXmlText( value.text( ) );
      e_desc->LinkEndChild( e_text );
      e_self->LinkEndChild( e_desc );
//      std::cout << key.text( ) << "->TEXT = " << value.text( ) << std::endl;
    }
    else {
      FXString CHName  = key.section( ":", 0 );
      FXString CHParam = key.section( ":", 1 );

//      std::cout << CHName.text( ) << "->" << CHParam.text( ) << " = " << value.text( ) << std::endl;
      e_tmp = e_self->FirstChildElement( CHName.text( ) );
      if( e_tmp == NULL ) {
        e_tmp = new TiXmlElement( CHName.text( ) );
        e_self->LinkEndChild( e_tmp );
      }
      e_tmp->SetAttribute( CHParam.text( ), value.text( ) );
    }
  }

  e_tmp = e_self->FirstChildElement( "Basic" );
  e_tmp->SetAttribute( "background", ( ( this->hidel  == true )? "true" : "false" ) );
  e_tmp = e_self->FirstChildElement( "Extra" );
  if( e_tmp == NULL ) {
    e_tmp = new TiXmlElement( "Extra" );
    e_self->LinkEndChild( e_tmp );
  }
  e_tmp->SetAttribute( "numberOfPlays", FXString::value( this->nop ).text( ) );
//  std::cout << "End writing";
//  std::cout << "\n\n";
//  std::cout.flush( );
}

void FXGameItem::checkIcons( FXApp *app )
{
  //std::cout << "[ FXGameitem ] Checking external icon..." << std::endl;
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

/*** END ******************************************************************************************/




































