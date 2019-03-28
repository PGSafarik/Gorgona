// Utils.cpp Copyright (c) 24/03/2019;  D.A.Tiger; GNU GPL 3
#include<Utils.h>
#include<iostream>

/*************************************************************************************************/
FXint split( const FXString &str, FXArray<FXString> *buffer, const FXString &sep )
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

FXint pack( FXArray<FXString> *buffer, FXString *str, const FXString &sep )
{
  return 0;
}

FXchar* convert_str( const FXString &str )
{
  if( !str.empty( ) ) {
    FXchar *c_str = new FXchar[ str.length( ) + 1 ];
    return strcpy( c_str, str.text( ) );
  }
  return NULL;
}

void clear_string_buffer( FXArray<const FXchar*> *buffer )
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

/*************************************************************************************************/
FXIcon* loadExternIcon( FXApp *a, const FXString &file, FXint width, FXint height )
{
   //std::cout << "[ FXGameitem ] Loading external icon..." << std::endl;
   FXIcon *ic = NULL;
   FXFileStream fstr;
   FXString exp = "jpg";

   if( fstr.open( file ) == true ) {
     //std::cout << "Open the external icon file " << file.text( ) << std::endl;
     exp = FXPath::extension( file );
     if( ( ic = createIconType( a, exp ) ) != NULL ) {
       if( ic->loadPixels( fstr ) == true ) {
         //std::cout << "Load the external file" << std::endl;
         if( ic->getWidth( ) > width || ic->getHeight( ) > height || ic->getWidth( ) < width || ic->getHeight( ) < height ) {
           //std::cout<< "rescaling (" << width << ") image: " << file.text( ) << std::endl;
           ic->scale( width, height );
         }
         ic->create( );
       }
       else { std::cout << "NOT Loading the external file " << file.text( ) << std::endl; }
     }
     fstr.close( );
   }
   return ic;
}

FXIcon* createIconType( FXApp *a, const FXString &type, FXuint opts )
{
   FXIcon *resulth = NULL;

   if( type.empty( ) == true ) { return resulth; }
   //if( type == "" || type = "" ) { resulth = new FX---Icon( a, NULL, 0, opts ); }
   if( type == "BMP" || type == "bmp" ) { resulth = new FXBMPIcon( a, NULL, 0, opts ); }
   if( type == "DDS" || type == "dds" ) { resulth = new FXDDSIcon( a, NULL, 0, opts ); }
   if( type == "EXE" || type == "exe" ) { resulth = new FXEXEIcon( a, NULL, 0, opts ); }
   if( type == "GIF" || type == "gif" ) { resulth = new FXGIFIcon( a, NULL, 0, opts ); }
   if( type == "ICO" || type == "ico" ) { resulth = new FXICOIcon( a, NULL, 0, opts ); }
   if( type == "IFF" || type == "iff" ) { resulth = new FXIFFIcon( a, NULL, 0, opts ); }
   if( type == "JP2" || type == "jp2" ) { resulth = new FXJP2Icon( a, NULL, 0, opts ); }
   if( type == "JPG" || type == "jpg" ) { resulth = new FXJPGIcon( a, NULL, 0, opts ); }
   if( type == "PCX" || type == "pcx" ) { resulth = new FXPCXIcon( a, NULL, 0, opts ); }
   if( type == "PNG" || type == "png" ) { resulth = new FXPNGIcon( a, NULL, 0, opts ); }
   if( type == "PPM" || type == "ppm" ) { resulth = new FXPPMIcon( a, NULL, 0, opts ); }
   if( type == "RAS" || type == "ras" ) { resulth = new FXRASIcon( a, NULL, 0, opts ); }
   if( type == "RGB" || type == "rgb" ) { resulth = new FXRGBIcon( a, NULL, 0, opts ); }
   if( type == "TGA" || type == "tga" ) { resulth = new FXTGAIcon( a, NULL, 0, opts ); }
   if( type == "TIF" || type == "tif" ) { resulth = new FXTIFIcon( a, NULL, 0, opts ); }
   if( type == "WEBP" || type == "webp" ) { resulth = new FXWEBPIcon( a, NULL, FXRGB(192, 192, 192), opts ); }
   if( type == "XBM" || type == "xbm" ) { resulth = new FXXBMIcon( a, NULL, 0, opts ); }
   if( type == "XPM" || type == "xpm" ) { resulth = new FXXPMIcon( a, NULL, 0, opts ); }

   return resulth;
}

/*** END ******************************************************************************************/
