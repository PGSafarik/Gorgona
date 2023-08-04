
/*************************************************************************
* This program is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU General Public License as published by   *
* the Free Software Foundation, either version 3 of the License, or      *
* (at your option) any later version.                                    *
*                                                                        *
* This program is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU General Public License      *
* along with this program.  If not, see <https://www.gnu.org/licenses/>. *
*************************************************************************/
#include<define.h>

#ifndef __OBJECTLISTOF_H
#define __OBJECTLISTOF_H

/*************************************************************************
* OjectListOf.h                                                          *
*                                                                        *
* Small class template replacing the non-conforming classes              *
* FXObjectListOf and FXObjectList                                        *
* Copyright (c) 05/05/2018 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/

namespace ECHIDNA {

  template<class OBJECT> class ObjectListOf: public FXArray<OBJECT*> {
   
  public:
  
    /* Opertions methods */ 
    FXival find( const OBJECT *object, FXival start = 0 ) 
    {
       FXival pos = FXMAX( 0, pos );
       FXival num = FXArray<OBJECT*>::no( );
       OBJECT *p  = FXArray<OBJECT*>::data( );
     
       while( pos < num ) {
         if( p[ pos ] == object ) { return pos; }
         ++pos;     
       }
     
       return -1;
    }
 
    FXival rfind( const OBJECT *object, FXival start = 2147483647 )
    {
      FXival pos = FXMIN( start, FXArray<OBJECT*>::no( ) - 1 ); 
      OBJECT *p  = FXArray<OBJECT*>::data( );

      while( 0 <= pos ) {
        if( p[ pos ] == object ) { return pos; }
        --pos; 
      }

      return -1;
    }

    void save( FXStream &store )
    {
      FXint num = FXArray<OBJECT*>::no( );
      OBJECT *p = FXArray<OBJECT*>::data( );

      store << num;
      for( FXival i = 0; i != num; i++ ) { store << p[ i ]; }       
    }

    void load( FXStream &store )
    {
      FXint num;
      store >> num;
  
      if( FXArray<OBJECT*>::no( num ) ) {
        OBJECT *p = FXArray<OBJECT*>::data( ); 
        for( FXival i = 0; i != num; i++ ) { store >> p[ i ]; } 
      } 
    } 

  };

  //typedef ObjectListOf<FXObject> ObjectList;
  class ObjectList : public ObjectListOf<FXObject> { }; 

}      /* ECHIDNA */
#endif /* __OBJECTLISTOF_H */
