// FXGameItem.cpp Copyright (c) 24/03/2019; D.A.Tiger; GNU GPL 3
#include<FXGameItem.h>


#include<Utils.h>
#include<FSM_Changes.h>
#include<Gorgona.h>

#include<iostream>

FXGameItem::FXGameItem( Gorgona *app )
{
  m_app = app;
  
  m_name       = FXString::null;
  m_type       = FXString::null;
  m_collection = FXString::null;
  m_number     = FXString::null;
  
  this->hidel     = false;
  this->m_change  = false; //(depracated)
  m_state         = FXGameItem::STATE_NEW;

  this->BigIcon   = NULL;
  this->SmallIcon = NULL;

  this->exec = new PERSEUS::Game( app );
}

FXGameItem::FXGameItem( Gorgona *app, const FXString &name, const FXString &type )
{
  m_app = app;

  this->hidel     = false;
  this->m_change  = false; //(depracated)
  m_state         = FXGameItem::STATE_NEW;
 
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
/*** Operators ************************************************************************************/
FXint FXGameItem::operator ( ) ( )
{
  FXString title = read( "Basic:title" );
  FXint pid = exec->run( );

  if( pid <= 0 ) {
    FXString head = "Execution Failed";
    FXString msg = "Error number: ";
    msg += FXString::value( pid ) + "\n"; 
    switch( pid ) {
      case -1 :  msg += "Game \'" + title + "\' is already launched!"; break;
      case -2 :  msg += "Game \'" + title + "\' is not have the launcher set!"; break;
      default :  msg += "The luach of game \'" + title + "\' failed!"; break;
    }
        
    FXMessageBox::warning( (FXApp*) exec->get_app( ), MBOX_OK, head.text( ), msg.text( ) );
    std::cerr << "[GorgonaWindow::Run]: " << head << msg << std::endl; 
    //std::cerr << "=====================================" << std::endl;
    //dump( true );
  }
  else  { m_change = true; }

  return pid;
}

/*** Access ***************************************************************************************/

void FXGameItem::set_id( const FXString &value ) 
{ 
  if( !value.empty( ) && value != m_id ) {
    m_id = value; 
    exec->set_appid( value );
    m_app->notify_changes( FSM_Changes::ID_CHANGE );
  }
}

void FXGameItem::dump( FXbool force )
{
  
  if( force == false ) {
    //std::cout << "Game entry [" << exec->get_launchid( ) << "] = " << this->property[ "Basic:title" ] << std::endl;
    std::cout << "Game entry " << exec->get_appid( ) << " ( " << this->property[ "Basic:title" ] << ", " << exec->get_launchid( ) << ")" << std::endl;   
    std::cout.flush( );
  }
  else {
    std::cout << "Game entry "          << this->property[ "Basic:title" ] << std::endl;
    std::cout << "Change: "             << this->m_change << std::endl;
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
  FXbool repair = false;

  FXString text =  "VALIDATION: \"" + property[ "Basic:title" ]; 
           text += "\" ";
  FXString info = "";
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
  if( m_id.empty( ) ) {
    //m_id = FXString::value( property[ "Basic:title" ].hash( ) );
    set_id( FXString::value( property[ "Basic:title" ].hash( ) ) );
    repair = true;
    //m_change = true;

    info = "FIX 1: The item does not have a GAME ID assigned, the program generated a replacement one.";
    info += "This can then be changed in the item editor. ";    
  }
   
   
  if( repair or !exec->validation( )  ) {
    std::cout << text << " " << m_id << " [REPAIR] \n";
    std::cout << "(" << info << ") \n";
    m_valid = true;
    m_app->notify_changes( FSM_Changes::ID_CHANGE );

  }/*
  else { std::cout << text << "OK. \n"; m_valid = true; } */
  //std::cout << text << " Loaded \n";
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
     this->m_change = chang;
   }
   return resh;
}


void FXGameItem::__T_load( XMLElement *x_record )
{
   /* Item */
   m_type       = x_record->Name( );   
  
   /* Base data for this item */
   m_id         = x_record->Attribute( "id" );
   m_name       = x_record->Attribute( "title" );
   m_collection = x_record->Attribute( "collection" );
   m_number     = x_record->Attribute( "serial" ); 
  
   /* Properties */
   //m_description->load( x_record );
   //m_visual->load( x_record );
   //m_properties->load( x_record );
   
   /* Trigger */
   exec->set_appid( m_id );
   exec->load( x_record );
   
   /* Finally */
   checkIcons( );
   validate( );  
   
   DEBUG_OUT( "FXGameItem::Load( ) " << m_id << "\t" << m_name )
}

void FXGameItem::__T_save( XMLElement *x_record, FXbool force )
{
  if( m_change || force ) {
    /* Basic item data */
    x_record->SetAttribute( "id",         m_id.text( ) );
    x_record->SetAttribute( "title",      m_name.text( ) );
    x_record->SetAttribute( "collection", m_collection.text( ) );
    x_record->SetAttribute( "serial",     m_number.text( ) );
    
    /* Properties */
    //m_description->save( x_record, force );
    //m_visual->save( x_record, force );    
    //m_properties->save( x_record, force );
    
    /* Trigger */
    exec->save( x_record, force );
    
    /* Finally */
    DEBUG_OUT( "FXGameItem::Save( ) " << m_id << "\t" << m_name )  
  }    
}

void FXGameItem::load( XMLElement *x_record )
{
  FXString _name, _value, _sect, _key;
  
  m_id = x_record->Attribute( "id" );
  exec->set_appid( m_id );

  for( XMLElement *elem = x_record->FirstChildElement( ); elem; elem = elem->NextSiblingElement( ) ){
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
  
  checkIcons( );
  
  exec->load( x_record ); 
  validate( );
   
  #ifdef __DEBUG
   dump( );
  #endif
  
  
}

const FXString FXGameItem::read( const FXString &k ) const
{
  FXString s = FXString::null;
  FXival pos = 0;
  if( ( pos = this->property.find( k.text( ) ) ) != -1 ) { s = this->property.data( pos ); }
  return s;
}

FXbool FXGameItem::save( XMLElement *x_record, FXbool force )
{
  XMLElement *e_desc  = NULL;      // Nosny element popisu
  XMLElement *e_tmp   = NULL;      // Pomocny element
  
  if( m_change || force ) {
    DEBUG_OUT( "FXGameItem::Save( ) " << m_id << "\t" << read( "Basic:title" ) ) 
    
    FXString key, value;
    for( FXival i = 0; i < this->property.no( ); i++ ) {
      key   = this->property.key( i );
      value = this->property.data( i );
      if( key.empty( ) || value.empty( ) ) { continue; }

      // Zapis Polozek, ktere je vyhodnejsi osetrit samostatne
      if( key == "Description" && !value.empty( ) ) {
        if( ( e_desc = x_record->FirstChildElement( key.text( ) ) ) == NULL ) {
          e_desc = x_record->InsertNewChildElement( key.text( ) );
        }
        e_desc->SetText( value.text( ) );
      }
      else {
        FXString CHName  = key.section( ":", 0 );
        FXString CHParam = key.section( ":", 1 );
        e_tmp = x_record->FirstChildElement( CHName.text( ) );
        if( e_tmp == NULL ) { e_tmp = x_record->InsertNewChildElement( CHName.text( ) ); }
        e_tmp->SetAttribute( CHParam.text( ), value.text( ) );
      }
    }

    /// FIXME GAMEITEM_001: force!
    exec->save( x_record, true );
    return true;
  }
  
  return false;
}

void FXGameItem::checkIcons( )
{
  FXString file;
  FXint sms = 22; // smal size
  FXint bis = 32; // big size

  if( this->BigIcon != NULL ) { delete this->BigIcon; }
  if( this->SmallIcon != NULL ) { delete this->SmallIcon; }

  if( ( file = this->read( "Icon:all" ) ) != FXString::null ){
    this->SmallIcon = loadExternIcon( m_app, file, sms, sms );
    this->BigIcon   = loadExternIcon( m_app, file, bis, bis );
    return;
  }
  if( ( file = this->read( "Icon:small" ) ) != FXString::null ){
    this->SmallIcon = loadExternIcon( m_app, file, sms, sms );
  }
  if( ( file = this->read( "Icon:big" ) ) != FXString::null ){
    this->BigIcon = loadExternIcon( m_app, file, bis, bis );
  }
}
/*** END ******************************************************************************************/
