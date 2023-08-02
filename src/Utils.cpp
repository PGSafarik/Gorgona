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

/**************************************************************************************************/
FX::FXString& operator <<( FX::FXString &dest, const std::string &source ) {
  if( !source.empty( ) ) {
    int len = source.size( );
    dest.length( len );
    source.copy( dest.text( ), len ); 
  }   
  else { dest = FXString::null; }

  return dest;
}

std::string& operator <<( std::string &dest, const FX::FXString &source ) {
  if( !source.empty( ) ) { dest.assign( source.text( ) ); } 
  else { dest = ""; }
  
  return dest;
}

std::ostream& operator <<( std::ostream &store, const FX::FXString &str  ) {
  return store << str.text( );
}

std::istream& operator >>( std::istream &store, FX::FXString &str  ) {
  std::string s;

  store >> s;
  str << s;

  return store;
}
/*
FX::FXString& operator =( const std::string &str ) {


}
*/
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

/**************************************************************************************************/
void Welcome( FXApp *app )
{
  std::cout << "=== " << app->getAppName( ) << " ========================================" << std::endl;
  std::cout << "Copyright " << AutoVersion::DATE << "/" << AutoVersion::MONTH << "/" << AutoVersion::YEAR << "  D.A.Tiger <drakarax@seznam.cz>, GNU GPL 3" << std::endl;
  std::cout << "Version    : " << AutoVersion::MAJOR<< "."<< AutoVersion::MINOR << "." << AutoVersion::REVISION;
  std::cout << " [" << AutoVersion::STATUS << "]" << std::endl;
  std::cout << "lib Fox    : " << FOX_MAJOR << "." << FOX_MINOR << "." << FOX_LEVEL << std::endl;
  std::cout << "lib FoxGHI : " << 0 << "." << 4 << "." << 1 << std::endl;
  std::cout << "=== Message =========================================" << std::endl;
}

/*** Signal *********************************************************************************/
FXIMPLEMENT( GSignal, FXObject, NULL, 0 )

GSignal::GSignal( FXObject *owner, FXuint msg_type )
{
  m_emitor = ( owner ? owner : this );
  m_type   = msg_type;
}

GSignal::~GSignal( )
{

}

FXbool GSignal::connect( FXObject *target, FXuint message )
{
  if( target ) {
    GSlot *slot = new GSlot( target, message, true );
    return this->connect( slot );
  }

  return false;
}

FXbool GSignal::connect( GSlot *slot )
{
   FXbool res = false;

   if( slot && slot->target ) { m_slots.push( slot ); res = true; }
   return res;
}

FXbool GSignal::disconnect( GSlot *slot )
{
  return disconnect( slot->target );  
}

FXbool GSignal::disconnect( FXObject *target )
{
  FXbool res = false;
  
  if( target ) {
    FXival num = m_slots.no( );
    FXival pos = -1;
 
    for( FXival i = 0; i != num; i++ ) {
      if( target == m_slots[ i ]->target ) {
        pos = i;
        break;
      } 
    }

    if( pos >= 0 ) {
      GSlot *slot = m_slots[ pos ];
      m_slots.erase( pos );
      if( slot->clear_me ) {
        slot->target = NULL;
        slot->msg_id = 0;

        delete slot;
      }
      res = true; 
    }
  }

  return res; 
}

int GSignal::emit( void *data )
{
  FXint res = 0;
  FXival num = m_slots.no( );

  for( FXival i = 0; i != num; i++ ) {
    GSlot *slot = m_slots[ i ];
    if( slot && slot->send( m_emitor, m_type, data ) == 0 ) { res++; }
  }
  
  std::cout << "[GSignal] Emit " << res << " signals, for " << num << " registered slots  \n"; 
  return res;
}

/*** END ******************************************************************************************/
