// Process.cpp Copyright (c) 05/04/2023;  D.A.Tiger; GNU GPL 3
#include<Perseus/Process.h>

using namespace PERSEUS;

/*************************************************************************************************/
Process::Process( ) 
       : FXProcess( ), m_asid( 0 )   
{ 
  InitControlMat( );
}

Process::~Process( ) 
{ }

/**************************************************************************************************/
void Process::set_asid( FXuint value ) 
{ 
   if( CheckStateMat( Process::ST_INIT ) ) { m_asid = value; }
} 

FXbool Process::run( const CharsList &cmd )
{
  FXbool res = false;
  
  if( CheckStateMat( Process::ST_RUNNING ) ) {
    res = this->start( cmd[ 0 ], cmd.data( ) );   
    if( res ) { 
      m_runtime = FXDate::localDate( ).getTime( );
    }
    else { CheckStateMat( Process::ST_ERROR ); }
  }

  return res;
}

void Process::exited( FXint code ) 
{
  if( CheckStateMat( Process::ST_STOPPED ) ) {
    m_retcode = code;
  } 
}

void Process::InitControlMat( ) { 
  for( FXuint i = 0; i != 4; i++ ) {
    for( FXuint j = 0; j != 4; j++ ) {
      m_control_trans[ i ][ j ] = 0;
		}
  }
 
  m_control_trans[ Process::ST_INIT ][ Process::ST_INIT ]       = 1;
  m_control_trans[ Process::ST_INIT ][ Process::ST_RUNNING ]    = 1;
  m_control_trans[ Process::ST_INIT ][ Process::ST_ERROR ]      = 1;
  m_control_trans[ Process::ST_RUNNING ][ Process::ST_ERROR ]   = 1;
  m_control_trans[ Process::ST_RUNNING ][ Process::ST_STOPPED ] = 1;
  m_control_trans[ Process::ST_ERROR ][ Process::ST_STOPPED ]   = 1;

  m_control_ends = Process::ST_STOPPED;

   m_control_state = Process::ST_INIT;
}

FXbool Process::CheckStateMat( FXuint next_state, FXbool fast ) 
{ 
  FXbool result = false;
 
  if( m_control_trans[ m_control_state ][ next_state ] == 1 || fast ) 
  { 
    m_control_state = next_state;    
    result = true; 
  }
 
  std::cout << "Process " << id( ) << " state: " << m_control_state << std::endl;  
  return result;
}

/*** END ******************************************************************************************/
