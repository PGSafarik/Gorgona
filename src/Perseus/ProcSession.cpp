// Process.cpp Copyright (c) 21/01/2025;  D.A.Tiger; GNU GPL 3
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
FXbool ProcSession::start( )
{
  if( ( m_sid = setsid( ) ) < 0 ) { 
    std::cerr << "[ProcSession WARNING]: Failed of setsid( ) call. Is this process already the session leader? ("  << m_sid << ")" << std::endl;
  }
  
  if( m_id.empty( ) ) { m_id = ( m_sid >= 1 ? FXString::value( m_sid ) : "1" ); }
  //FXSystem::setEnvironment( m_name, m_id );

  m_jobs.resize( ID_NORMAL_MAX + 1, 0 );
  set_job( JOB_SYSTEM );
  DEBUG_OUT( "[ProcSession::start] starting process session " << m_name << " (" << m_sid << ")" )

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

  return -1;
}

FXint ProcSession::check( )
{
  FXint pid_value;
  FXString checker = "/home/gabriel/Projects/Fox/sources/Gorgona/tmp/utils/grpchecker.sh";
  
  boost::asio::io_service ios3;
  std::future<std::string> fdata;
  BP::child filesystems( checker.text( ), BP::std_in.close( ), BP::std_out > fdata, BP::std_err > BP::null, ios3 );
  ios3.run( );

  m_maintable.clear( );
  std::istringstream _out;
  _out.str( fdata.get( ) );
  for( std::string line; std::getline( _out, line); ) { m_maintable.push_back( std::stoi( line ) ); }
  
  std::cout << "Gorgana Session processes: " << std::endl;
  std::cout << "===========================" << std::endl;
  for( int i = 0; i != m_maintable.size( ); i++ ) { std::cout << m_maintable[ i ] << std::endl; }   
  std::cout << std::endl;
    
  return (FXint) m_maintable.size( );
}

/*** END ******************************************************************************************/
} /* PERSEUS */
