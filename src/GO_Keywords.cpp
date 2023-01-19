// GO_Keywords.cpp Copyright (c) 24/03/2019;  D.A.Tiger; GNU GPL 3
#include<GO_Keywords.h>

GO_Keywords::GO_Keywords( const FXString &w, const FXString &d )
{
   k_word = w;
   k_description = d;
}

GO_Keywords::~GO_Keywords( )
{

}

/*************************************************************************************************/
FXint GO_Keywords::read( TiXmlElement *kwelem )
{
  FXint resh = 0;

  if( kwelem != NULL ) {
    this->k_description = kwelem->Attribute( "decription" );
    this->k_word       = kwelem->Attribute( "keyword" );

    TiXmlElement *telem = kwelem->FirstChildElement( "Tag" );
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

FXbool GO_Keywords::write( TiXmlElement *kwelem )
{

  return true;
}
/*** END ******************************************************************************************/
