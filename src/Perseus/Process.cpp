// Process.cpp Copyright (c) 05/04/2023;  D.A.Tiger; GNU GPL 3
#include<Perseus/Process.h>

using namespace PERSEUS;

/*************************************************************************************************/
Process::Process( ) 
       : FXProcess( ), m_asid( 0 )   
{ }

Process::~Process( ) 
{ }

/**************************************************************************************************/
FXbool Process::run( const EntriesList &cmd )
{
  m_runtime = FXDate::localDate( ).getTime( );

  FXbool res = this->start( cmd[ 0 ], cmd.data( ) );   
  return res;
}

/*** END ******************************************************************************************/
