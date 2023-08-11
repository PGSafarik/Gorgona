// String_Utils.cpp Copyright (c) 08/05/2018;  D.A.Tiger; GNU GPL 3
#include<Echidna/String_utils.h>

using namespace ECHIDNA;

namespace ECHIDNA {
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
