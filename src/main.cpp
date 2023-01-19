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
/*************************************************************************
* main.cpp                                                               *
*                                                                        *
* Gorgona - spravce her                                                  *
* Copyright (c) 08/02/2015 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<fox-1.7/fx.h>
#include<Gorgona.h>
#include<GorgonaWindow.h>

int main( int argc, char **argv )
{
  Gorgona app( "Gorgona", "FXGameLauncher" );
  app.init( argc, argv );
  new GorgonaWindow( &app );
  app.create( );
  return app.run( );
}
/*** END ****************************************************************/
