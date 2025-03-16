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
  /* (cs)
   * Za normalnich okolnosti by stacil POUZE StringList.split a bylo by vymalovano. Jenze inovace v obsahu XDG_DATA_DIRS vymyslena
   * a zavedena naozajstnymi KOKOTY si vynutila tento zasrany workarround, ktery odstrani vsechny ZKURVENE duplicitni cesty ukocene 
   * pathseparatorem. A protoze lidska blbost ma tendeci se nekontrolovatelne mnozit, je nutno tuto hruzu aplikovat ted na vsechny 
   * ENV jejichz obsahem je nejaky seznam cest ve filesystemu. Ilustruje to fakt, ze uz i na Linuxu se ridi drive ciste korporatni 
   * zasadou: KRETENI VPRED! Minimalne poslednich deset let urcite...
   * PS. Na distribuci Debianu / GNU Linux to zamrzi obzvlast bolestive.
   * 
   * (en)
   * Under normal circumstances ONLY StringList.split would be enough and it would be painted. An innovation in the content of 
   * XDG_DATA_DIRS invented and implemented by real PICKS forced this shitty workaround, which removes all FUCKING duplicate paths 
   * terminated by pathseparator. And because human stupidity tends to multiply uncontrollably, it is necessary to apply this burden 
   * to all ENVs whose content is some list of paths in the filesystem. This is illustrated by the fact that even on Linux drives are 
   * governed by a purely corporate principle: IDIOTS FORWARD! At least the last ten years for sure...
   * P.S. This is particularly painful for the Debian / GNU Linux distribution.
   */    
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