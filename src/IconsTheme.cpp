// IconsTheme.cpp Copyright (c) 24/03/2019;  D.A.Tiger; GNU GPL 3
#include<IconsTheme.h>

/*************************************************************************************************/
IconsTheme::IconsTheme( FXApp *a, const FXString &folder )
          : impl( )
{
  it_app      = a;
  it_folder   = folder;
  it_creating = false;
}

IconsTheme::~IconsTheme( )
{ }

/*************************************************************************************************/
void IconsTheme::insert( const FXString &key, const FXString &dir )
{
  if( key == FXString::null ) {
    std::cout << "Icons theme finding epmty icons Group key!!!" << std::endl;
    return;
  }

  if( impl::find( key ) == -1 ) {
    FXString pth = it_folder + dir;
    FXIconCache *cache = new FXIconCache( it_app, pth );
    //cache->setIconPath( pth );
    impl::insert( key, cache );
    //std::cout << "Icons theme inserting icons Group key: " << key.text( ) << " / " << cache->getIconPath( ).text( ) << std::endl;
  } else { replace( key, dir ); }
}

void IconsTheme::replace( const FXString &key, const FXString &dir )
{
  //std::cout << "Icons theme replaced icons Group key: " << key.text( ) << std::endl;
  FXint old_id = impl::find( key );
  if( old_id != -1 ) { impl::remove( key ); }
  insert( key, dir );
}

FXIconCache* IconsTheme::getCache( FXival pos )
{
  if( pos >= 0 ) { return impl::data( pos ); }
  else { return NULL; }
}

FXIconCache* IconsTheme::getCache( const FXString &key )
{
  return getCache( FXDictionary::find( key ) );
}

void IconsTheme::dump( )
{
  FXint num = impl::no( );
  FXString key, pth;
  std::cout << "Icons theme dump:" << std::endl;
  std::cout << "NO: " << num << " USED: " << impl::used( ) << " FREE: " << impl::free( ) << std::endl;

  if( num > 0 ) {
    for( FXival i = 0; i < num; i++ ) {
      key = impl::key( i );
      if( key.empty( ) ) { continue; }
      FXIconCache *cache = getCache( key );
      if( cache != NULL ) {
        pth = cache->getIconPath( );
        std::cout << "found:" << key.text( ) << "" << pth.text( ) << std::endl;
      } else { std::cout << "found:" << key.text( ) << " IS EMPTY" << std::endl; }

    }

  }
}
/*** END ******************************************************************************************/
