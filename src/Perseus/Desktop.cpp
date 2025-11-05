// String_Utils.cpp Copyright (c) 2025;  D.A.Tiger; GNU GPL 3
#include<Perseus/Desktop.h>

using namespace PERSEUS;
namespace PERSEUS {
FXString CheckEnv( const FXString &env, const FXString &def_val )
{
  // Funkce vrati prednastavenou hodnotu  v env (XDG_*). Pokud neni definovana vraci hodnotu zadanou parametrem def. 
  FXString value = FXString::null;
  
  if( !env.empty( ) ) {
    FXString s = FXSystem::getEnvironment( env );
    value = ( s.empty( ) ? def_val : s );
  }
  return value; 
}

FXuint CheckEnvList( const FXString &xdg_env, const FXString &def_val, FXStringList &list  )
{
  // Podobne jako CheckEnv, s tim rozdilem, ze hodnotu rozseka na jednotlive slozky podle zavedenho separatorem ":"
  FXuint   res = 0;
  FXString value = CheckEnv( xdg_env, def_val );

  if( !value.empty( ) ) {
    if( value.left( 1 ) == ENVPATHSEPSTRING ) { value = def_val + value; }
    res = list.split( value, ENVPATHSEPSTRING);

   /* Workarround for eliminate a duplication entries */
    list.sort( );
    FXuint num = list.no( );
    FXString tested; 
    for( FXuint i = 0; i != num; i++ ) {
      FXString item = list[ i ];
      if( item.right( 1 ) == PATHSEPSTRING ) { item.erase( item.length( ) - 1 ); }
      if( item != tested ) { tested = item; }
      else {
        list.erase( i );
        i--;
        num--; 
        res--;
      }
    }
  }  
  
  return res;  
}


/*** User ****************************************************************************************/
FXString HomeConfigDirectory( ) { return FXSystem::getHomeDirectory( ) + PATHSEPSTRING + CheckEnv( "XDG_CONFIG_HOME", ".config"      ); }
FXString HomeDataDirectory( )   { return FXSystem::getHomeDirectory( ) + PATHSEPSTRING + CheckEnv( "XDG_DATA_HOME",   ".local/share" ); }

/*** System **************************************************************************************/
FXuint SystemConfigDirectories( FXStringList &list ) { return CheckEnvList( "XDG_CONFIG_DIRS", "/etc:/etc/xdg", list ); }
FXuint SystemDataDirectories( FXStringList &list )   { return CheckEnvList( "XDG_DATA_DIRS", "/usr/share:/usr/local/share", list ); }

/*** Mimes ***************************************************************************************/
int getMimeList( FXStringList &store, const FXString &file )
{
  FXint resh = 0;
  FXString data;
  
  FXbool sorting = ( store.no( ) > 0 );
  FXFile fd( file );     
  
  if( fd.isOpen( ) ) {
    data.length( fd.size( ) );
    fd.readBlock( data.text( ), data.length( ) );
    fd.close( );  
    
    if( !data.empty( ) ) {
      resh = store.split( data, "\n" );
      if( sorting ) { store.sort( ); } 
    }
  }
  else { resh = -1; }
  
  
  return resh;  
}

} /* namespace PERSEUS */
/*** END ******************************************************************************************/