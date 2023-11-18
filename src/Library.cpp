// Library.cpp Copyright (c) 05/08/2019; D.A.Tiger; GNU GPL 3
#include<Library.h>

Library::Library( Gorgona *app, const FXString &elname )
       : m_app( app ), m_elname( elname ) 
{ }

Library::Library( Gorgona *app, const FXString &filename, const FXString &elname )
       : m_app( app ), m_elname( elname )
{
  open( filename );
}

Library::~Library( )
{
  close( true ); 
}

/**************************************************************************************************/
FXbool Library::open( const FXString &filename )
{
  if( !filename.empty( ) ) {
    m_file = filename;
    if( m_opened ) { close( ); }
  }

  if( !isOpen( ) && !filename.empty( ) && ( m_xdoc.LoadFile( filename.text( ) ) == XML_SUCCESS ) ) {
    std::cout << "[DEBUG - Library::open] Loading Library from file: " << filename << std::endl;
    if( ( m_xroot = m_xdoc.RootElement( ) ) != NULL ) {
      if( ( m_xbase  = m_xroot->FirstChildElement( "Library" ) ) != NULL ) {
        //m_elname = elname;
        m_opened = true;
      }
    }     
  }
  else { // FIXME  - samostane metody.
    int err_id = m_xdoc.ErrorID( ); 
    std::cout << "[ERROR Library::open] XML read error - " << m_xdoc.ErrorName() << "(" << err_id << "): " << m_xdoc.ErrorStr( ) << std::endl;
    if( err_id == 3 ) {  /* Pozadovany soubor knihovny nenalezen, nebo (jeste) nevytvorena. Prvni spusteni? */
      m_xroot = m_xdoc.NewElement( "Gorgona" );
      m_xbase = m_xdoc.NewElement( "Library" );
      
      m_xroot->InsertFirstChild( m_xbase );
      m_xdoc.InsertFirstChild( m_xroot ); 

      m_opened = true;
    }
  }    

  return m_opened;
}

FXbool Library::close( FXbool force )
{
  if( !m_opened ) { return m_opened; }
  if( !m_change( ) || force ) {
    if( m_change( ) ) { m_change.handle( m_app, FXSEL( SEL_COMMAND, FSM_Changes::ID_DISCARD ), NULL ); }
    FXint num = no( );
    for( FXint i = 0; i != num; i++ ) { 
      FXGameItem *it = at( i );
      erase( i );
      if( it ) { delete it; }  
    }
    clear( );
    m_xbase = NULL;
    m_xroot = NULL;
    m_xdoc.Clear( );
    m_elname = FXString::null;
    m_file   = FXString::null;
    m_opened = false;
  }     

  return m_opened;
}

FXint Library::load( )
{
  FXint res = -1;
  if( m_opened ) { ( load( m_xbase ) ? res = 1 : res = 0 ); }
  
  return res;
}

FXint Library::save( )
{
  FXint res = -1;
  FXString msg = "[Gorgona::Save_Library] ";
  
  if( m_opened ) {
    XMLElement *root = m_xdoc.RootElement( );
    if( !root ) {
      std::cerr << "[Gorgona::Save_Library] FATAL: Store not have root element!" << std::endl;
      return -1;
    }
    if( this->save( root->FirstChildElement( "Library" ) ) ) {
      msg += "Write XML library ";
      if( m_xdoc.SaveFile( m_file.text( ) ) == XML_SUCCESS ) { 
        msg += "SUCCESS";
        res = 0; 
      }
      else { 
        res = m_xdoc.ErrorID( );   
        msg += "FAILED: "; 
        msg += m_xdoc.ErrorName( ); 
        msg += "("; 
        msg += FXString::value( res ) + "): "; 
        msg += m_xdoc.ErrorStr( );
      }  
    }
    else { msg += "There is nothing to store"; }
  }

  std::cout << msg << std::endl;  
  return res;
}


FXbool Library::load( XMLElement *library_el )
{
  FXbool result = false; 
   
  if( library_el ) {
    for( XMLElement *el = library_el->FirstChildElement( m_elname.text( ) ); el; el = el->NextSiblingElement( m_elname.text( ) ) ) {
      FXGameItem *item = new FXGameItem( m_app );    
      if( item ) {
        item->load( el );
        item->checkIcons( ( FXApp*) m_app );  // ?  FIXME ItemList_001: FXGameItem::CheckIcons muze (ted uz) volat i metoda FXGameItem::Load( )
        push( item );
      }
      else { std::cout << "[WARNING ItemList::Load( )] : Unable to create a trigger item" << std::endl; }  
      result = true;
    }
  } 

  return result; 
}

FXbool Library::save( XMLElement *library_el )
{
  FXbool result = false;
   
  if( library_el && m_change( ) ) {
    std::cout << "Save Library changes: \n";
    std::cout << "===================== \n";

    FXbool force = library_el->NoChildren( );
    if( force ) { cout << "WARNING: Element \"Library\" has no subelements. Using force saving. \n"; }

    FXint num = no( );
    for( FXint i = 0; i != num; i++ ) { at( i )->save( library_el, force ); } // FIXME: nenene!
    
    std::cout << "" << num << std::endl;
    result = true;
  }

  return result;
}

/**************************************************************************************************/
XMLElement* Library::FindElementBy( XMLElement *parent, const FXString &attr, FXString &value )
{
  if( parent ) {
    for( XMLElement *act = parent->FirstChildElement( ); act; act = act->NextSiblingElement( ) ) {
      FXString ret = act->Attribute( attr.text( ) ); 
      if( ret == value ) { return act; }
    } 
  }

  return NULL;   
}
