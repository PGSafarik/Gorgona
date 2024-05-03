// ItemBase.cpp Copyright (c) %date%;  D.A.Tiger; GNU GPL 3
#include<Gorgona.h>
#include<Echidna/ItemBase.h>

using namespace ECHIDNA;
namespace ECHIDNA {

FXIMPLEMENT( ItemBase, FXObject, NULL, 0 )

ItemBase::ItemBase( Gorgona *a, const FXString &type ) 
{
  m_app    = a;
  m_type   = type;

  InitStateMat( );
}

ItemBase::~ItemBase( )
{ }

/**************************************************************************************************/
FXbool ItemBase::set_property( const FXString &key, const FXString &value )
{
  if( !key.empty( ) && !value.empty( ) ) { 
    if( CheckState( ItemBase::CSA_STATE_MODIFY, SEL_CHANGE_INCR ) ) { return SetProperty( key, value ); }    
  }

  return false;
}

FXString ItemBase::get_property( const FXString &key )
{
  FXString v = FXString::null;
  
  if( key.empty( ) ) { v = m_type; }
  else {
    FXint pos = m_property.find( key );
    if( pos != -1 ) { v = m_property.data( pos ); } 
  }
  
  return v;
}

/**************************************************************************************************/
FXint ItemBase::load( XMLElement *store )
{
  FXint res = -1;
  
  if( store && CheckState( ItemBase::CSA_STATE_READY, SEL_CHANGE_DECR ) ) {
    XMLElement *record = store->FirstChildElement( m_type.text( ) );
    
    if( record ) {
      const XMLAttribute *attr = record->FirstAttribute( );
      while( attr != NULL ) {
        SetProperty( attr->Name( ), attr->Value( ) ); 
        attr = attr->Next( );
      }
      res = _Load( record );
    }
  }
  
  return res;
}

FXint ItemBase::save( XMLElement *store ) 
{
  FXint       res = -1;
  FXuint      state = get_state( );
  XMLElement *record = store->FirstChildElement( m_type.text( ) );
  
  switch( state ) {
    case ItemBase::CSA_STATE_MODIFY : {
      if( CheckState( ItemBase::CSA_STATE_READY, SEL_CHANGE_DECR ) ) {
        if( !record ) { record = store->InsertNewChildElement( m_type.text( ) ); }
        FXint p_num = m_property.no( );
        for( FXint i = 0; i != p_num; i++ ) {
          if( !m_property.empty( i ) ) {
            FXString p_key = m_property.key( i );
            FXString p_val = m_property.data( i );
            std::cout << m_type << "[" << p_key << "] = " << p_val << std::endl;
            if( !p_val.empty( ) ) { record->SetAttribute( p_key.text( ), p_val.text( ) ); }
          }
        }
        
        res = _Save( record );
      }  
      break; 
    }
    case ItemBase::CSA_STATE_REMOVED : {
      if( CheckState( ItemBase::CSA_STATE_EMPTY, SEL_CHANGED ) ) {
        clear( );
        if( record ) {
          _Erase( record );
          store->DeleteChild( record );
        }
      }  
      break; 
    }
    default: { res = 0; }
  }  

  return res;
}

FXbool ItemBase::clear( FXbool force )
{
  FXbool res = false;
  FXuint next_state = ( force ? ItemBase::CSA_STATE_REMOVED : ItemBase::CSA_STATE_MODIFY );

  // Kvuli kompilatoru: warning: enumerated mismatch in conditional expression: 'FX::<unnamed enum>' vs 'ECHIDNA::<unnamed enum>' [-Wenum-compare]
	FXuint msg =  SEL_CHANGED; 
  if( force ) { msg = SEL_CHANGE_INCR; }      
  
  if( CheckState( next_state, msg ) ) {
    m_property.clear( );
    res = true;
  }
  
  return res;
}

void ItemBase::dump( FXbool force, FXuint distance )
{
  FXString d_str1 = "";
  FXString d_str2 = "\t";
  if( distance > 0 ) {
    FXString n = "";
    for( FXuint i = 0; i != distance; i++ ) { n += "\t"; }
    d_str1 += n;
    d_str2 += n;
  }   
    
  std::cout << d_str1 << "[DEBUG - ItemBase::dump( ) ] " << m_type << " state: " << m_csa_state << std::endl;
  FXint p_num = m_property.no( );
  for( FXint i = 0; i != p_num; i++ ) {
    if( !m_property.empty( i ) ) {
      FXString p_key = m_property.key( i );
      FXString p_val = m_property.data( i );
      std::cout << d_str2 << m_type << "[" << p_key << "] = " << p_val << std::endl;
    }
  } 
}

/**************************************************************************************************/
void ItemBase::InitStateMat( )
{  
  for( FXuint i = 0; i != 5; i++ ) {
    for( FXuint j = 0; j != 5; j++ ) {
       m_csa_trans[ i ][ j ] = 0;
    }
  }  
  
  m_csa_trans[ ItemBase::CSA_STATE_READY   ][ ItemBase::CSA_STATE_MODIFY  ] = 1;
  m_csa_trans[ ItemBase::CSA_STATE_READY   ][ ItemBase::CSA_STATE_ACTIVE  ] = 1;
  m_csa_trans[ ItemBase::CSA_STATE_READY   ][ ItemBase::CSA_STATE_REMOVED ] = 1;
  m_csa_trans[ ItemBase::CSA_STATE_EMPTY   ][ ItemBase::CSA_STATE_READY   ] = 1;
  m_csa_trans[ ItemBase::CSA_STATE_EMPTY   ][ ItemBase::CSA_STATE_MODIFY  ] = 1;
  m_csa_trans[ ItemBase::CSA_STATE_MODIFY  ][ ItemBase::CSA_STATE_READY   ] = 1;
  m_csa_trans[ ItemBase::CSA_STATE_MODIFY  ][ ItemBase::CSA_STATE_MODIFY  ] = 1;
  m_csa_trans[ ItemBase::CSA_STATE_ACTIVE  ][ ItemBase::CSA_STATE_MODIFY  ] = 1;
  m_csa_trans[ ItemBase::CSA_STATE_REMOVED ][ ItemBase::CSA_STATE_READY   ] = 1;
  m_csa_trans[ ItemBase::CSA_STATE_REMOVED ][ ItemBase::CSA_STATE_EMPTY   ] = 1;
  
  m_csa_ends[ 0 ] = ItemBase::CSA_STATE_READY;
  m_csa_ends[ 1 ] = ItemBase::CSA_STATE_EMPTY;
  
  m_csa_state = ItemBase::CSA_STATE_EMPTY;
  m_csa_sleep = false;
}

FXbool ItemBase::CheckState( FXuint next_state, FXuint notify, FXbool force )
{
  if( m_csa_sleep ) { return true; }
  if( m_csa_trans[ m_csa_state ][ next_state ] == 1 || force ) 
  { 
    FXbool differ = ( m_csa_state != next_state );
    if( differ ) { m_csa_state = next_state; }
   
    if( notify > 0 && differ ) { SendNotify( notify ); }
    
#ifdef __DEBUG
    std::cout << "[DEBUG - ItemBase::CheckState( ) ] " << m_type << " control automation " << ( differ ? "changed on " : " stay in " )  << " : " << m_csa_state << std::endl;
    FXint p_num = m_property.no( );
    for( FXint i = 0; i != p_num; i++ ) {
      if( !m_property.empty( i ) ) {
        FXString p_key = m_property.key( i );
        FXString p_val = m_property.data( i );
        std::cout << "\t" << m_type << "[" << p_key << "] = " << p_val << std::endl;
			}
    }
#endif

    return true; 
  }
 
  return false;      
}

/**************************************************************************************************/
FXbool ItemBase::SetProperty( const FXString &key, const FXString &value )
{
  if( !key.empty( ) ) { return m_property.insert( key, value ); }

  return false;
}

long ItemBase::SendNotify( FXuint msg_type, void *msg_data )
{
  if( m_target ) { return m_target->handle( this, FXSEL( msg_type, m_msg ), msg_data ); }
  return 0;  
}

/**************************************************************************************************/
}; /* namespaece ECHIDNA */

/*** END ******************************************************************************************/
