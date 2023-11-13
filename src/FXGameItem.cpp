// FXGameItem.cpp Copyright (c) 24/03/2019; D.A.Tiger; GNU GPL 3
#include<FXGameItem.h>


#include<Utils.h>
#include<FSM_Changes.h>
#include<Gorgona.h>

#include<iostream>

FXGameItem::FXGameItem( Gorgona *app )
{
  m_app = app;

  this->hidel     = false;
  this->m_change    = false;
  
  this->BigIcon   = NULL;
  this->SmallIcon = NULL;

  this->exec = new PERSEUS::Game( app );
}

FXGameItem::FXGameItem( Gorgona *app, const FXString &name, const FXString &type )
{
  m_app = app;

  this->hidel     = false;
  this->m_change    = false;
  
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
      default :  msg += "The luach of game \'" + title + "\' failed!"; break;
    }
        
    FXMessageBox::warning( (FXApp*) exec->get_app( ), MBOX_OK, head.text( ), msg.text( ) );
    std::cerr << "[GorgonaWindow::Run]: " << head << msg << std::endl; 
  }
  else  { m_change = true; }

  return pid;
}

/*** Access ***************************************************************************************/

void FXGameItem::set_id( const FXString &value ) 
{ 
  if( !value.empty( ) && value != m_id ) {
    m_id = value; 
    m_app->notify_changes( FSM_Changes::ID_CHANGE );
  }
}

void FXGameItem::dump( FXbool force )
{
  
  if( force == false ) {
    std::cout << "Game entry [" << exec->get_launchid( ) << "] = " << this->property[ "Basic:title" ] << std::endl;
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
    m_id = FXString::value( property[ "Basic:title" ].hash( ) );
    repair = true;
    m_change = true;

    info = "FIX 1: The item does not have a GAME ID assigned, the program generated a replacement one.";
    info += "This can then be changed in the item editor. ";    
  }
   
   
  if( repair or !exec->validation( )  ) {
    std::cout << text << " [REPAIR] \n";
    std::cout << "(" << info << ") \n";
    m_valid = true;
    m_app->notify_changes( FSM_Changes::ID_CHANGE );

  }/*
  else { std::cout << text << "OK. \n"; m_valid = true; } */

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

void FXGameItem::load( XMLElement *eitem )
{
  FXString _name, _value, _sect, _key;
  
  m_id = eitem->Attribute( "id" );

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
  validate( );
  /* 
  #ifdef __DEBUG
   dump( );
  #endif
  */
  
}

const FXString FXGameItem::read( const FXString &k ) const
{
  FXString s = FXString::null;
  FXival pos = 0;
  if( ( pos = this->property.find( k.text( ) ) ) != -1 ) { s = this->property.data( pos ); }
  return s;
}

void FXGameItem::save( XMLElement *pNode, FXbool force )
{
  #ifdef __DEBUG
    std::cout << m_id << "\t" << read( "Basic:title" ) << " \n";
    //std::cout.flush( );
  #endif

  FXString ename = "Game";         // FIXME: Ohejbak na rovnak - fixed it!

  XMLElement *e_desc  = NULL;      // Nosny element popisu
  XMLElement *e_tmp   = NULL;      // Pomocny element
  XMLElement *e_self  =  NULL;     // Element reprezentujici (herni) polozku

  if( ( e_self = FindEntry( pNode ) ) == NULL )  {
    std::cout << "Create new Element \n";
    e_self = pNode->InsertNewChildElement( ename.text( ) );
  }

  e_self->SetAttribute( "id", m_id.text( ) );

  if( m_change || force ) {
    std::cout << "FORCE or CHANGES SAVE ITEM!! \n"; 
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

/*** Comparation methods **************************************************************************/
FXbool FXGameItem::Compare_with( XMLElement *e )
{
  FXString tname = "Game";
  
  FXString e_name = e->Name( );
  FXString e_id    = e->Attribute( "id" );

  //std::cout << "COMPARE with: " << e_id << " ( " << e_name << " ) \n";
  if( e && tname == e_name && m_id == e_id ) {
    return true;
  }

  return false;
}

XMLElement* FXGameItem::FindEntry( XMLElement *parent )
{
  if( parent ) {
    cout << "Find in element " << parent->Name( ) << endl;
    for( XMLElement *act = parent->FirstChildElement( "Game" ); act; act = act->NextSiblingElement( "Game" ) ) {
      if( Compare_with( act ) ) { 
        //cout << "found \n"; 
        return act; 
      }
      //else { cout << "not found \n"; }  
    } 
  } 
            
  return NULL; 
} 

/*** END ******************************************************************************************/
