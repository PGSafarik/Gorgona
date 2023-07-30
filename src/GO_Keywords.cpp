// GO_Keywords.cpp Copyright (c) 24/03/2019;  D.A.Tiger; GNU GPL 3
#include<GO_Keywords.h>

GO_Keywords::GO_Keywords( const FXString &w, const FXString &d )
{
   m_word = w;
   m_description = d;
}

GO_Keywords::~GO_Keywords( )
{

}

/*************************************************************************************************/
FXint GO_Keywords::read( XMLElement *kwelem )
{
  FXint resh = 0;

  if( kwelem != NULL ) {
    this->m_description = kwelem->Attribute( "decription" );
    this->m_word       = kwelem->Attribute( "keyword" );

    XMLElement *telem = kwelem->FirstChildElement( "Tag" );
    while( telem != NULL ) {
      GO_KeyEntry *e = new GO_KeyEntry;
      e->description = telem->Attribute( "description" );
      e->actionid    = telem->Attribute( "action" );
      split( telem->Attribute( "value" ), &e->vlist, ";" );

      this->insert( telem->Attribute( "key" ), e );
      resh++;
      
      telem = telem->FirstChildElement( "tag" ); 
    }
  }

  return resh;
}

FXbool GO_Keywords::write( XMLElement *kwelem )
{

  return true;
}
/*** END ******************************************************************************************/
