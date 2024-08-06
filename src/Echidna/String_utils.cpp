// String_Utils.cpp Copyright (c) 08/05/2018;  D.A.Tiger; GNU GPL 3
#include<Echidna/String_utils.h>

using namespace ECHIDNA;

namespace ECHIDNA {
/*** Identifier class *****************************************************************************/
  //SRC_KEY:ID_VALUE
  void Identifier::set_key( const FXString &src )
  {
    FXString delim = ":";
    FXint pos = find( delim );

    if( src.empty( ) ) { // Klic je prazdny - pokud existuje predesli, bude smazan 
      if( pos >= 0 ) { erase( 0, pos + 1); }
    }
    else {
      if( empty( ) ) { assign( src + delim ); } 
      else {
        if( pos < 0 ) { prepend( src + delim ); }  // Neni znam zadny drive zadany klic zdroje
        else if( pos == 0 ) { prepend( src ); }    // Na zacatku je oddelovac 
        else if( pos == length( ) - 1 ) { assign( src + delim ); }
        else { 
          erase( 0, pos );
          prepend( src + delim ); 
        }  
      }
    }  
  }
  
  FXString Identifier::get_key( )
  {
    FXString delim = ":";
    FXint pos      = find( delim );
    
    return ( ( pos > 0 ) ? left( pos ) : FXString::null );
  }
  
  void Identifier::set_value( const FXString &val )
  {
    FXString delim = ":";
     
    if( empty( ) ) { assign( val ); }
    else {
      FXint pos = find( delim );
      if( pos < 0 || pos == 0 ) { assign( val ); }
      else  {
        erase( pos, length( ) - ( pos ) );
        append( delim + val );     
      }
    } 
  }
  
  FXString Identifier::get_value( )
  {
    if( !empty( ) ) { 
      FXString delim = ":";
      FXint pos = find( delim );
      
      return ( ( pos >= 0 ) ? mid( pos + 1, length( ) -1 ) : *this );   
    }
    else { return FXString::null; } 
  }
  
  FXint Identifier::verify( )
  {
    // 1 - not set, 2 - no has value, 3 - bad format
    FXint    resh  = 0;
    FXString delim = ":";
    
    if( empty( ) ) { resh = 1; }
    else {
      FXint pos = find( delim );
      if( pos == length( ) - 1 ) { resh = 2; }
      //else if( length( ) == ( pos + 1 ) ) { resh = 3; }
    } 
    
    return resh;
  }
  
  void Identifier::make( const FXString &src, const FXString &pattern )
  {
    assign( value( src.hash( ) ) ); 
  }
  
/*** String buffering *****************************************************************************/
  FXint split( const FXString &str, StringList *buffer, const FXString &sep )
  {
    FXint count = 0;
    FXint nstr, pos;
    FXString sub_str;

    if( !str.empty( ) && ( buffer != NULL ) ) {
      nstr = str.contains( sep );
      if( nstr > 0 ) {
        nstr++;
        pos = 0;
        while( pos < nstr ) {
          sub_str = str.section( sep, pos );
          if( !sub_str.empty( ) ) {
            buffer->append( sub_str );
            count++;
          }
          pos++;
        }
      }
    }
    return count;
  }

  FXString pack( const StringList &buffer, const FXString &sep )
  {
    FXString str = FXString::null;
    FXint    num = buffer.no( );

    for( FXint i = 0; i != num; i++ ) {
       if( i == 0 ) { str = buffer[ 0 ]; }
       else { str += buffer[ i ]; }
       
       if( i + 1 < num ) { str += sep; }
    }     
    
    return str;
  }

  FXchar* convert_str( const FXString &str )
  {
    if( !str.empty( ) ) {
      FXchar *c_str = new FXchar[ str.length( ) + 1 ];
      return strcpy( c_str, str.text( ) );
    }
    return NULL;
  }

  void clear_string_buffer( CharsList *buffer )
  {
    FXint num;
    if( ( num = buffer->no( ) ) > 0 ) {
      for( FXint i = 0; i != num; i++ ) {
         const FXchar *data = buffer->at( i );
         if( data != NULL ) { delete[] data; }
      }
      buffer->clear( );
    }
  }

} /* ECHIDNA */
/*** END ******************************************************************************************/
