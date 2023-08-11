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
  if( !isOpen( ) && !filename.empty( ) && ( m_xdoc.LoadFile( filename.text( ) ) == XML_SUCCESS ) ) {
    std::cout << "[DEBUG - Library::open] Loading Library from file: " << filename << std::endl;
    if( ( m_xroot = m_xdoc.RootElement( ) ) != NULL ) {
      if( ( m_xbase  = m_xroot->FirstChildElement( "Library" ) ) != NULL ) {
        m_file   = filename;
        //m_elname = elname;
        m_opened = true;
      }
    }     
  }
  else { // FIXME  - samostane metody.
    std::cout << "[ERROR Library::open] XML read error - " << m_xdoc.ErrorName() << "(" << m_xdoc.ErrorID( ) << "): " << m_xdoc.ErrorStr( ) << std::endl;
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

  if( m_opened ) { 
   XMLElement *root = m_xdoc.RootElement( );
   ( save( root->FirstChildElement( "Library" ) )? res = 1 : res = 0 ); 
  }
  
  if( ( res > 0 ) && m_xdoc.SaveFile( m_file.text( ) ) != XML_SUCCESS ) { 
    std::cout << "[Gorgona::Save_Library] XML writting error - " << m_xdoc.ErrorName() << "(" << m_xdoc.ErrorID( ) << "): " << m_xdoc.ErrorStr( ) << std::endl;
  }

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
  } /* Knihovna (jeste) nevytvorena. Prvni spusteni? */ 

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
