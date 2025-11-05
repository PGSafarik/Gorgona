// Process.cpp Copyright (c) 21/01/2025;  D.A.Tiger; GNU GPL 3
#include<Perseus/Process.h>
#include<Perseus/ProcSession.h>

using namespace PERSEUS;
namespace PERSEUS {
  /**************************************************************************************************/
  ProcSession::ProcSession( const FXString &name, const FXString &id ) : m_name( name ), m_id( id )
  {

  }

  ProcSession::~ProcSession( )
  {

  }

  /**************************************************************************************************/
  FXbool ProcSession::start(  const FXString &check_utility )
  {
    m_grpcheck = check_utility;
    DEBUG_OUT( "Gorgona session checker utility: " << m_grpcheck << std::endl );

    if( ( m_sid = getsid( getpid( ) ) ) < 0 ) {
      int error_no = errno;
      std::cerr << "[ProcSession WARNING]: Failed of setsid( ) call: " << strerror( error_no ) << " ("  << error_no <<  ")" << std::endl;
    }

    if( m_id.empty( ) ) { m_id = ( m_sid >= 1 ? FXString::value( m_sid ) : "1" ); }

    m_jobs.resize( ID_NORMAL_MAX + 1, 0 );
    set_job( JOB_SYSTEM );
    DEBUG_OUT( "[ProcSession::start] starting process session " << m_name << " (" << m_id << ")" )

    return true;
  }

  FXbool ProcSession::terminate( FXbool fast )
  {
    FXint check, fail;

    check = fail = 0;
    for( auto it = this->begin( ); it != this->end( ); it++ ) {
      PERSEUS::Process *proc = it->second;
      if( !proc->kill( ) ) {
        std::cerr << "[ProcSession ERROR]: Failed of terminate process " << proc->id( ) << std::endl;
        fail++;
      }
      check++;
    }

    if( fail > 0 ) {
      std::cerr << "[ProcSession WARNING]: The process session " << this->id( ) << " was not terminated correctly: " << std::endl;
      std::cerr << fail << " processes are still running!" << std::endl;
    }

    m_sid = -1;
    return true;
  }

  FXbool ProcSession::join( FXint pid, FXint group_id )
  {
    if( !has( pid ) ) {
      for( FXint n : m_maintable ) {
        if( n == pid ) { return false; }
      }
      m_maintable.push_back( pid );
      return true;
    }

    return false;
  }

  FXint ProcSession::set_job( FXuint type )
  {
    FXint result = -1;

    if( type != JOB_SYSTEM ) {
      FXint max = ( type == JOB_RESERVED ? ID_RESERVED_MAX : ID_NORMAL_MAX );
      FXint min = ( type == JOB_RESERVED ? 1 : ID_RESERVED_MAX + 1 );
      FXbool found = false;

      max++;
      DEBUG_OUT( "[ProcSession::set_job] MIN = " << min << " MAX = " << max )
      for( FXint i = min; i != max; i++ ) {
        if( m_jobs[ i ] == 0 ) { result = i; }
      }
    }
    else { result = 0; }

    if( result >= 0 ) {
      FXSystem::setEnvironment( m_name, FXString::value( result ) );
      if( result > 0 ) { m_jobs[ result ] = 1; }
      m_actjob = result;
    }

    return result;
  }

  FXint ProcSession::check( )
  {
    FXint pid_value;

    FILE *fd = popen( m_grpcheck.text( ), "r" );
    if( fd == NULL ) {
      return -1;
    }

    char buffer[ 128 ];  // 1024
    m_maintable.clear( );
    while( fgets( buffer, 128, fd ) != NULL ) {
      m_maintable.push_back( std::stoi( buffer ) );
    }

    pclose( fd );

#ifdef __DEBUG
    std::cout << "Gorgana Session processes (" << m_maintable.size( ) << ")" <<  std::endl;
    std::cout << "=================================" << std::endl;
    for( int i = 0; i != m_maintable.size( ); i++ ) { std::cout << m_maintable[ i ] << std::endl; }
    std::cout << std::endl;
#endif

    return (FXint) m_maintable.size( );
  }

  /*** END ******************************************************************************************/
} /* PERSEUS */
