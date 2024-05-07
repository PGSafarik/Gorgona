// TermProfile.cpp; Gorgona/Perseus; Copyright (c) 27/04/2024;  D.A.Tiger; GNU GPL 3
#include<Perseus/TermProfile.h>
using namespace PERSEUS;

/**************************************************************************************************/
TermProfile::TermProfile( const FXString ident )
{
   name      = ident;
   exec      = FXString::null; 
   p_run     = FXString::null;
   p_noclose = FXString::null;
   p_workdir = FXString::null;
}

TermProfile::~TermProfile( ) 
{ }

FXbool TermProfile::load( const FXSettings &conf, const FXString ident )
{
    
    
 return false;   
}
  
FXbool TermProfile::save( FXSettings &conf, const FXString ident )
{
    
    
  return false;    
}

/*** END ******************************************************************************************/
