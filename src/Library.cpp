// Library.cpp Copyright (c) 05/08/2019; D.A.Tiger; GNU GPL 3
#include<Library.h>

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
    std::cout << "Save Library changes: \n";
    std::cout << "===================== \n";

    FXint num = no( );
    for( FXint i = 0; i != num; i++ ) { at( i )->save( library_el, true ); } // FIXME: nenene!
    
    std::cout << "" << num << std::endl;
  }

  return result;
}

