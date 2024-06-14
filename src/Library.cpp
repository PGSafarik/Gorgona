// Library.cpp Copyright (c) 05/08/2019; D.A.Tiger; GNU GPL 3
#include<Library.h>

Library::Library( Gorgona *app, const FXString &elname )
{ 
  m_app    = app;
  m_elname = elname;

  m_opened = false;        
  m_file   = FXString::null;
  m_xroot  = NULL;          
  m_xbase  = NULL;          
}

Library::Library( Gorgona *app, const FXString &filename, const FXString &elname )
{
  m_app    = app;
  m_elname = elname;
  m_opened = false;        
  m_file   = FXString::null;
  m_xroot  = NULL;          
  m_xbase  = NULL;          

  open( filename );
}

Library::~Library( )
{
  close( true ); 
}

/**************************************************************************************************/
FXGameItem* Library::make( const FXString &name, const FXString &id )
{
  FXGameItem *it = NULL;
  
  if( find_title( name ) == -1 ) {
    // Crete new Game item and generate default game identifier (if-not)
    it = new FXGameItem( m_app, name );
    FXString _id = ( id.empty( ) ? FXString::value( name.hash( ) ) : id );
    
    // Make XML record for new game item
    XMLElement *x_root  = m_xdoc.RootElement( );
    XMLElement *x_lib   = x_root->FirstChildElement( "Library" );
    XMLElement *x_record = x_lib->InsertNewChildElement( "Game" );
    x_record->SetAttribute( "id", _id.text( ) ); 
    
    // Internal association game identifier with XML record
    m_xmap.insert( _id, x_record );
    it->set_id( _id );
    
    // Insert new game item on library
    push( it );
  }
  
  return it;
}

FXbool Library::change_id( const FXString &id_old, const FXString &id_new )
{
  FXbool result = false;
  
  if( !id_new.empty( ) ) {
    XMLElement *x_record = m_xmap[ id_old ];
    if( x_record ) {
      m_xmap.remove( id_old );
      x_record->SetAttribute( "id", id_new.text( ) );
      m_xmap.insert( id_new, x_record );
      
      result = true;
    }
  }
  
  return result;
}

FXbool Library::remove( FXGameItem *it )
{
  FXbool res = false;
  FXival pos = -1;
  
  if( it ) { 
    pos = find( it );

    if( pos >= 0 ) {
      FXString name = it->read( "Basic:title" );
      FXString id   = it->get_id( );

      // Odstraneni instance polozky z knihovny
      erase( pos );
      delete it;
    
      // Vymaz zaznamu polozky z XML databaze
      if( !( res = EraseElement( id ) ) ) {
        XMLElement *record = FindElementBy( m_xbase, "id", id );
        if( !record ) { record = FindElementBy( m_xbase, "title", name ); }
        if( record ) {
          m_xdoc.DeleteNode( record );
          res = true;
        }
      }
    }
  }
  
  return res;
}

FXival Library::find_title( const FXString &text ) 
{
  if( !text.empty( ) ) {
    FXival pos = 0;
    FXival num = no( );
    FXGameItem **p = data( );
     
    while( pos < num ) {
      if( p[ pos ]->read( "Basic:title" ) == text ) { return pos; }
      ++pos;     
    }  
  }
     
  return -1;
}
 
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
    
    FXint num = this->no( );
    while( num > 0 ) {
      FXGameItem *it = this->tail( );
      if( it ) {
        erase( num - 1 );
        delete it; 
      }
      num = this->no( ); 
    }
    clear( );
    m_xdoc.Clear( ); // FIXME LIBRAR_001 : Maybe include on Library::clear( )
    
    m_xbase = NULL;
    m_xroot = NULL;
    m_xmap.clear( );
     
    m_elname = FXString::null;
    m_file   = FXString::null;
    m_opened = false;
  }     

  return m_opened;
}

FXint Library::load( )
{
  FXint result = -1;
  if( m_opened ) { 
    for( XMLElement *el = m_xbase->FirstChildElement( m_elname.text( ) ); el; el = el->NextSiblingElement( m_elname.text( ) ) ) {
      FXGameItem *item = InsertElement( el );
      if( item ) { 
        push( item ); 
        result = 0;  
      }
    }
  }
  
  return result;
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

/* DEPRACATED
FXbool Library::load( XMLElement *library_el )
{
  FXbool result = false; 
   
  if( library_el ) {
    for( XMLElement *el = library_el->FirstChildElement( m_elname.text( ) ); el; el = el->NextSiblingElement( m_elname.text( ) ) ) {
      FXGameItem *item = InsertElement( el );
      if( item ) { 
        push( item ); 
        result = true;  
      }
    }
  } 

  return result; 
}
*/
FXbool Library::save( XMLElement *library_el )
{
  DEBUG_OUT( "Library::save( )" )
  FXbool result = false;
   
  if( library_el && m_change( ) ) {
    std::cout << "Save Library changes: \n";
    std::cout << "===================== \n";

    FXbool force = library_el->NoChildren( );
    if( force ) { cout << "WARNING: Element \"Library\" has no subelements. Using force saving. \n"; }

    FXint num = no( );
    for( FXint i = 0; i != num; i++ ) { 
      /*at( i )->save( library_el, force ); */
      FXGameItem *it = at( i );
      it->save( m_xmap[ it->get_id( ) ], force );  // FIXME: nenene!
    } 
    
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

FXGameItem* Library::InsertElement( XMLElement *record )
{
  FXGameItem *it = NULL;
  FXString id; 
  
  if( record ) {
    it = new FXGameItem( m_app );
      
    it->load( record );
    m_xmap.insert( it->get_id( ), record ); 
  }  
  
  return it;
}


FXbool Library::EraseElement( const FXString &id )
{
  FXbool res = false; 
  XMLElement *record = NULL;
  
  if( !id.empty( ) ) {
    if( ( record = m_xmap[ id ] ) != NULL ) {
      m_xmap.remove( id );
      m_xdoc.DeleteNode( record );
      res = true;      
    }
  }
  
  return res;
}
