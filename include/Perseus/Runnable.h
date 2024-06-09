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
#ifndef __RUNNABLE_H
#define __RUNNABLE_H

/*************************************************************************
* Runnable.h                                                             *
*                                                                        *
* Small classes representing an executable structure                     *
* Copyright (c) 05/05/2018 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<define.h>

class Gorgona;

namespace PERSEUS {

  /* Obecna trida definujici spustitelny objekt Gorgony */
  class Runnable: public FXObject  {
  FXDECLARE( Runnable)
    Gorgona  *m_app;         // Ukazatel na instanci aplikace
    
    FXString  m_workdir;     // Pozadovany pracovni adresare
    FXString  m_command;     // Zadany prikaz ke spusteni
    FXString  m_execute;     // Skutecny prikaz ke spusteni ( po zpracovani )
    FXbool    m_terminal;    // Spustit program v (emulatoru) terminalu
    FXbool    m_notify;      // Indikator notifikace opreaci

    FXString m_appid;        // Application ID  
    FXbool   m_change;       // Indikuje zmenu objektu
    FXint    m_pid;          // Process ID
    
    FXObject   *m_target;    // Cilovy objekt notifikaci
    FXSelector  m_message;   // notifikacni zprava
  
    FXString           m_launchid;   // Identifikator spoustece (pro moduly, jinak "native", nebo prazdny )
    FXStringDictionary m_launchprms; // Seznam parametru spoustece

  public :
    Runnable( Gorgona *a, const FXString &appid = FXString::null, FXObject *tgt = NULL, FXSelector sel = 0 );
    Runnable( Gorgona *a, const FXString &appid, const FXString &cmd, const FXString &launcher = "native", FXObject *tgt = NULL, FXSelector sel = 0 );
    virtual ~Runnable( );

    /* Access methods */
    Gorgona* get_app( )                         { return m_app;       } 
    void     set_notify( FXbool value = true )  { m_notify = value;   }
    FXbool   get_notify( )                      { return m_notify;    }
    void     set_workdir( const FXString &dir ) { m_workdir = dir;    }
    FXString get_workdir( )                     { return m_workdir;   } 
    void     set_launchid( const FXString &id ) { m_launchid = id;    }
    FXString get_launchid( )                    { return m_launchid;  }
    void     set_command( const FXString &cmd ) { Command( cmd );     } // FIXME RUNNABLE_001: Run module launcher!
    FXString get_command( )                     { return m_command;   } 
    void     set_change( FXbool value )         { m_change = value;   }
    FXbool   is_changed( )                      { return m_change;    }
    void     set_terminal( FXbool value )       { m_terminal = value; Command( ); }
    FXbool   enabled_termnal( )                 { return m_terminal;  }                        
    FXbool   is_running( )                      { return m_pid > 0;   } 
    void     set_appid( const FXString &value ) { m_appid = value;    }
    FXString get_appid( )                       { return m_appid;     }  

    virtual FXint run( );
    FXbool load( XMLElement *parent ); 
    FXbool save( XMLElement *parent, FXbool force = false );
    virtual FXbool validation( );
    
    /* Debug & testing */
    virtual void dump( ); 

   /* Events messages & Handlers */
   enum {
      PROC_EXIT = 1,
      ID_LAST
   };

   long OnSig_Process( FXObject *tgt, FXSelector sel, void  *data ); 
 
  protected:
    Runnable( ) { }
    
    /* Helpful methods */
    virtual void Command( const FXString &cmd = FXString::null ); 
    virtual void Write( XMLElement *runelement ) { }
    virtual void Read( XMLElement *runelement )  { }

    FXbool IsNative( ) { return ( m_launchid.empty( ) || m_launchid == "native" ); }
    FXString ChangeWorkDir( );
    void CheckTerminal( );
  };

  /* Trida urcena k spousteni a rizeni procesu her */
  class Game : public Runnable {
  FXDECLARE( Game )
    FXint   m_used;        // Pocet spusteni 
    FXlong  m_total;       // Celkova doba spusteni
    FXTime  m_last;        // Posledni spusteni (datum, cas)   
    FXlong  m_time;        // Celkova doba posledniho spusteni 
    FXlong  m_longest;     // Nejdelsi doba behu vubec  

  public:
    Game( Gorgona *a, const FXString &appid = FXString::null, FXObject *tgt = NULL, FXSelector sel = 0 );
    virtual ~Game( );
    
    /* Access methods */
    FXint    count( ) { return m_used; }
    FXString last( );  

    /* Operations methods */
    virtual FXint run( );

    /* Debug & testing */
    virtual void dump( ); 

  protected:
    Game( ) { }
   
    /* Helpful routines */
    void Counter( );
    virtual void Write( XMLElement *runelement );
    virtual void Read( XMLElement *runelement );

  };

}      /* Namespace PERSEUS */
#endif /* __RUNNABLE_H */
/*** END ****************************************************************/
