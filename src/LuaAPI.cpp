// FXLauncherLua.cpp Copyright (c) 24/03/2019;  D.A.Tiger; GNU GPL 3
#include<LuaAPI.h>
#include<GorgonaWindow.h>
#include<FXGameItem.h>
#include<Utils.h>

Gorgona *_inst;   // Instance obsluzneho objektu

// Auxiliary functions
FXRegistry& get_registry( ) { return _inst->reg( ); }

// Gorgoana API for LUA scripts
int static l_launcher_input( lua_State *L );     // Input box
int static l_launcher_message( lua_State *L );   // Message box
//int static l_launcher_exec( lua_State *L );      // exec ( Run program )
//int static l_launcher_selitem( lua_State *L );   // Select game item

// Manipulation with Gorgona cfg register
int static l_registry_write( lua_State *L );     // Read registry value
int static l_registry_read( lua_State *L );      // Write registry value
int static l_registry_test( lua_State *L );      // Test on existing Head or Key
int static l_registry_delete( lua_State *L );     // Remove header or Key


/*************************************************************************************************/
FXbool l_open( Gorgona *app )
{ 
  _inst = app;

  FXbool     result = false;
  lua_State *state = _inst->getLua( );

  if( state ) {  
    luaL_openlibs( state );

    /// GUI /////////////////////////////////////
    lua_register( state, "MessageBox", l_launcher_message );
    lua_register( state, "InputBox",   l_launcher_input );
    /// List manipulating ///////////////////////
    /// lua_register( state, "GetSelectItem",    l_launcher_selitem );
    /// Tree Manipulating ///////////////////////
    /// Others //////////////////////////////////
    //lua_register( state, "__exec",          l_launcher_exec );
    lua_register( state, "Register_write",  l_registry_write );
    lua_register( state, "Register_read",   l_registry_read );
    lua_register( state, "Register_test",   l_registry_test );
    lua_register( state, "Register_delete", l_registry_delete );

    //lua_register( _state, "RegLaunch",  l_launcher_RegLaunch );

    // Creating table for button type MessageBox values
    lua_newtable( state );
    l_TableWrite_num( state, "ok",                         1 ); /*MBOX_OK*/
    l_TableWrite_num( state, "ok_cancel",                  2 ); /*MBOX_OK_CANCEL*/
    l_TableWrite_num( state, "yes_no",                     3 ); /*MBOX_YES_NO*/
    l_TableWrite_num( state, "yes_no_cancel",              4 ); /*MBOX_YES_NO_CANCEL*/
    l_TableWrite_num( state, "quit_cancel",                5 ); /*MBOX_QUIT_CANCEL */
    l_TableWrite_num( state, "quit_save_cancel",           6 ); /*MBOX_QUIT_SAVE_CANCEL*/
    l_TableWrite_num( state, "skip_skipall_cancel",        7 ); /*MBOX_SKIP_SKIPALL_CANCEL*/
    l_TableWrite_num( state, "save_cancel_dontsave",       8 ); /*MBOX_SAVE_CANCEL_DONTSAVE*/
    l_TableWrite_num( state, "yes_yesall_no_noall_cancel", 9 ); /*MBOX_YES_YESALL_NO_NOALL_CANCEL*/
    lua_setglobal( state, "mbox_buttons" );

    lua_newtable( state );
    l_TableWrite_num( state, "information", 1 );
    l_TableWrite_num( state, "warnning",    2 );
    l_TableWrite_num( state, "error",       3 );
    l_TableWrite_num( state, "question",    4 );
    lua_setglobal( state, "mbox_type" );

    result = true;
  }

  return result;
}

void l_close( )
{
  _inst = NULL;
}

/**************************************************************************************************/
int l_launcher_input( lua_State *L )
{
   FXString caption = "Zadejte, prosim, pozadovanou hodnotu: ";

   if( lua_gettop( L ) == 1 ) {
     switch( lua_type( L, 1 ) ) {
       case LUA_TNUMBER : {
         caption = FXString::value( (double) lua_tonumber( L, 1 ) );
         break;
       }
       case LUA_TSTRING : {
         caption = lua_tostring( L, 1 );
         break;
       }
     }
   }

   FXString data = inputBox( caption );

   if( data.empty( ) ) { lua_pushnil( L ); }
   else { lua_pushstring( L, data.text( ) ); }
   return 1;
}

int l_launcher_message( lua_State *L )
{
  FXString msg, title, actived;
  FXint num, resh, mtype;
  FXuint buttons;

  if( ( num = lua_gettop( L )  ) < 1 ) {
    l_Error( "MessageBox: Malo argumentu - nutno zadat alespon text zpravy!" );
    return 0;
  }

  msg = lua_tostring( L, 1 );  //}
  if( num >= 2 ) { title = lua_tostring( L, 2 ); } else { title = FXString::null; }
  if( num >= 3 ) {
    switch( ( FXint ) lua_tonumber( L, 3 ) ) {
      case 1 : buttons = MBOX_OK;                         break;
      case 2 : buttons = MBOX_OK_CANCEL;                  break;
      case 3 : buttons = MBOX_YES_NO;                     break;
      case 4 : buttons = MBOX_YES_NO_CANCEL;              break;
      case 5 : buttons = MBOX_QUIT_CANCEL;                break;
      case 6 : buttons = MBOX_QUIT_SAVE_CANCEL;           break;
      case 7 : buttons = MBOX_SKIP_SKIPALL_CANCEL;        break;
      case 8 : buttons = MBOX_SAVE_CANCEL_DONTSAVE;       break;
      case 9 : buttons = MBOX_YES_YESALL_NO_NOALL_CANCEL; break;
    }
  } else { buttons = MBOX_OK; }
  if( num == 4 ) { mtype = ( int ) lua_tonumber( L, 4 ); } else { mtype = 1; }

  switch( mtype ) {
    case 1 : resh = FXMessageBox::information( _inst, buttons, title.text( ), msg.text( ) ); break;
    case 2 : resh = FXMessageBox::warning( _inst, buttons, title.text( ), msg.text( ) );     break;
    case 3 : resh = FXMessageBox::error( _inst, buttons, title.text( ), msg.text( ) );       break;
    case 4 : resh = FXMessageBox::question( _inst, buttons, title.text( ), msg.text( ) );    break;
  }

  switch( resh ) {
    case 1 :  actived = "yes";      break;
    case 2 :  actived = "no";       break;
    case 3 :  actived = "ok";       break;
    case 4 :  actived = "cancel";   break;
    case 5 :  actived = "quit";     break;
    case 6 :  actived = "save";     break;
    case 7 :  actived = "skip";     break;
    case 8 :  actived = "skip_all"; break;
    case 9 :  actived = "yes_all";  break;
    case 10 : actived = "no_all";   break;
  }

  lua_pushstring( L, actived.text( ) );
  return 1;
}
/*
int l_launcher_exec( lua_State *L )
{
  FXint prm_num;
  FXArray<const FXchar*> args;

  if( ( prm_num = lua_gettop( L ) ) > 0 ) {
    for( FXint i = 1; i <= prm_num; i++ ) { args.insert(  i - 1,  lua_tostring( L, i ) ); }
    args.insert( prm_num, NULL );

    if( !_inst->exec( args ) ) {
      FXString err_str  = "lua::exec( ) : Process ";
      err_str          += args[ 0 ];
      err_str          += " running failed! \n";
      output( err_str );
    }
  }

  return 0;
}
*/

/*
int l_launcher_selitem( lua_State *L )
{
  luaL_checktype( L, 1, LUA_TTABLE );

  FXGameItem *it = _inst->get_ActiveItem( );
  if( it != NULL ) {
    l_TableWrite_str( L, "title", it->read( "Basic:title" ) );
    l_TableWrite_str( L, "genre", it->read( "Basic:genre" ) );
    l_TableWrite_str( L, "exec",  it->read( "Basic:exec" ) );
    l_TableWrite_str( L, "type",  it->read( "Basic:type" ) );
  }

  return 0;
}
*/

int l_registry_write( lua_State *L )
{
   FXString sect, key, def;
   FXbool   val = false;

   // Check types of function parameter
//   luaL_checktype( L, 1, LUA_TSTRING );  // Param 1 - Registry section
//   luaL_checktype( L, 2, LUA_TSTRING );  // Param 2 - Value key
//   luaL_checktype( L, 3, LUA_TSTRING );  // Param 3 - Value

   // Get function parameters value
   FXint pnum = lua_gettop( L );
   if( pnum >= 2 && pnum <= 3 ) {
     sect = lua_tostring( L, 1 );                                     // Param 1 - Registry section
     key = lua_tostring( L, 2 );                                      // Param 2 - value key
     def = ( ( pnum < 3 ) ? FXString::null : lua_tostring( L, 3 ) );  // param 3 - set value
   }
   else {
     std::cout << "[Gorgona:LUA_API::Register_write] Nespravny pocet argumentu! Zadano " << pnum << "; avsak vyzadovany min 2, max 3: " << std::endl;
     std::cout << "Register_write( section, key, value)" << std::endl;
   }

   // Data operation
   val = get_registry( ).writeStringEntry( sect.text( ), key.text( ), def.text( ) );

   // Return resulth value
   lua_pushboolean( L, val );

   return 1;
}

int l_registry_read( lua_State *L )
{
   FXString sect, key, def, val = FXString::null;

   FXint pnum = lua_gettop( L );
   if( pnum >= 2 && pnum <= 3 ) {
     sect = lua_tostring( L, 1 );                                     // Param 1 - Registry section
     key = lua_tostring( L, 2 );                                      // Param 2 - value key
     def = ( ( pnum < 3 ) ? FXString::null : lua_tostring( L, 3 ) );  // param 3 - default value
     // std::cout << "[Gorgona:LUA_API::Registr_read] params Num: " << pnum << std::endl;
     // std::cout << "[Gorgona:LUA_API::Registr_read] params Val: " << sect.text( ) << ", " << key.text( ) << ", " << def.text( ) << std::endl;
   }
   else {
     std::cout << "[Gorgona:LUA_API::Registr_read] Nespravny pocet argumentu! Zadano " << pnum << "; avsak vyzadovano min 2, max 3: " << std::endl;
     std::cout << "Register_read( section, key, value)" << std::endl;
   }

   // Data operation
   if( get_registry( ).existingSection( sect.text( ) ) == true )  {
     val = get_registry( ).readStringEntry( sect.text( ), key.text( ), def.text( ) );
   }

   // Return resulth value
   //std::cout << "[Gorgona:LUA_API::Registr_read] return : " << val.text( ) << std::endl;
   lua_pushstring( L, val.text( ) );

   return 1;
}

int l_registry_test( lua_State *L )
{
   FXString sect, key;
   FXbool es, ek;

  FXint pnum = lua_gettop( L );
   if( pnum > 0 && pnum <= 2 ) {
     sect = lua_tostring( L, 1 );                                     // Param 1 - Registry section
     key = ( ( pnum < 2 ) ? FXString::null : lua_tostring( L, 2 ) );  // param 2 - Value key
     //std::cout << "[Gorgona:LUA_API::Registr_read] params Num: " << pnum << std::endl;
     //std::cout << "[Gorgona:LUA_API::Registr_read] params Val: " << sect.text( ) << ", " << key.text( ) << std::endl;

   }
   else {
     std::cout << "[Gorgona:LUA_API::Registr_check] Nespravny pocet argumentu! Zadano " << pnum << "; avsak vyzadovano min 1, max 2: " << std::endl;
     std::cout << "Register_check( section, key )" << std::endl;
   }

   // Data operation
   es = ek = false;
   if( ( es = get_registry( ).existingSection( sect.text( ) ) ) == true )  {
     if( !key.empty( ) ) {
       if( get_registry( ).at( sect.text( ) ).find( key.text( ) ) != -1 ) { ek = true; }
     }
   }

   // Return resulth value
   //std::cout << "[Gorgona:LUA_API::Registr_read] return : " << es << ", " << ek << std::endl;
   lua_pushboolean( L, es ); // Ret 1 - Flag of registry section exist
   lua_pushboolean( L, ek ); // Ret 2 - Flag of value key exist

   return 2;
}

int l_registry_delete( lua_State *L )
{
   FXString sect, key;
   FXbool es, ek;

   FXint pnum = lua_gettop( L );
   if( pnum > 0 && pnum <= 2 ) {
     sect = lua_tostring( L, 1 );                                      // Param 1 - Registry section
     key = ( ( pnum < 2 ) ? FXString::null : lua_tostring( L, 2 ) );   // param 2 - Value key
   }
   else {
     std::cout << "[Gorgona:LUA_API::Registr_remove] Nespravny pocet argumentu! Zadano " << pnum << "; avsak vyzadovano min 1, max 2: " << std::endl;
     std::cout << "Register_check( section, key )" << std::endl;
   }

   // Data operation
   es = ek = false;
   if( !sect.empty( ) && get_registry( ).existingSection( sect ) ) {
     if( !key.empty( ) ) {
       get_registry( ).at( sect ).remove( key );
       ek = true;
     }
     else {
      get_registry( ).deleteSection( sect );
      es = true;
     }
   }

   // Return resulth value
   lua_pushboolean( L, es );
   lua_pushboolean( L, ek );

   return 2;
}

/*************************************************************************************************/
FXbool l_Script( const FXString &script )
{
  std::cout << "Loading the initial script: " << script.text( ) << std::endl;
  FXbool loaded = false;
  if( _inst->isLuaInit( ) == true ) {
    FXint resh = luaL_dofile( _inst->getLua( ), script.text( ) );
    if( resh == 0 ) { loaded = true; }
    else { l_ErrorMessage( resh ); }
  }
  return loaded;
}


void l_Error( const FXString &str )
{
  lua_pushstring( _inst->getLua( ), str.text( ) );
  lua_error( _inst->getLua( ) );
}

void l_ErrorMessage( FXint num, const FXString &msg )
{
  FXString err_text;
  FXString err_head = "Script runtime error ( ";
  err_head += FXString::value( num ) + " )";

  if( msg.empty( ) ) {
    err_text = lua_tostring( _inst->getLua( ), -1 );
    lua_pop( _inst->getLua( ), 1 );
  }
  else { err_text = msg; }

  output( err_head + "\n" + err_text );
  FXMessageBox::error( _inst, MBOX_OK, err_head.text( ), err_text.text( ) );
}

void l_TableWrite_str( lua_State *L, const FXString &index, const FXString &value )
{
  lua_pushstring( L, value.text( ) );
  lua_setfield( L, -2, index.text( ) );
}

void l_TableWrite_num( lua_State *L, const FXString &index, FXint value )
{
  lua_pushnumber( L, value );
  lua_setfield( L, -2, index.text( ) );
}

int l_ReadLaunchers( FXArray<FXString> *keylist )
{
  FXint num = 0;
  //std::cout << "Registered user lauchers : " << std::endl;
  FXint t = 1;
  lua_getglobal( _inst->getLua( ), "ltable" );
  if( lua_istable( _inst->getLua( ), t ) == true ) {
    /// FIXME LUA_01 : Nasledujici zakomentovany kod, je vyuzitelny pro cteni tabulek. Na pole je nepouzitelny
    /*
    lua_pushnil( _inst->getLua( ) );
    while( lua_next( _inst->getLua( ), t ) != 0 ) {
      FXString key = lua_tostring( _inst->getLua( ), -2 );
      keylist->push( key );
      num++;
      lua_pop( _inst->getLua( ) , 1 );
    }
   */
    num = luaL_getn( _inst->getLua( ), t );
    for( FXint i = 1; i <= num; i++ ) {
      lua_rawgeti( _inst->getLua( ), t, i );
      FXString key = lua_tostring( _inst->getLua( ), -1 );
      //std::cout << i << " : " << key.text( ) << std::endl;
      keylist->push( key );
    }
  }
  return num;
}


/*************************************************************************************************/
FXString inputBox( const FXString &label, const FXString &title )
{
   FXString resh  = FXString::null;
   FXInputDialog::getString( resh, _inst, title, label, NULL );
   return resh;
}

void output( const FXString &value, FXbool nonl )
{
  FXString endl_str = ( ( ( nonl == false ) && ( value.at( value.length( ) ) != '\n' ) ) ? "\n" : FXString::null );
  std::cout << value.text( ) << endl_str.text( );
  std::cout.flush( );
}

/*************************************************************************************************/
FXString luams_launch( const FXString &module_id, const FXArray<FXString> &prms )
{
  /* Call the module service: module_id.launcher( prms[ ] ): cmd_str */
  FXString cmd_str = FXString::null;
  lua_State *l = _inst->getLua( );
  

  lua_getglobal(  l, "launcher"  );
  //lua_pushstring( l, convert_str( p_id ) );
  //lua_pushstring( l, convert_str( p_cmd ) );

  if( lua_pcall( l, 2, 1, 0 ) != 0 ) { /// FIXME : FUNKCE PREJIMA DVA ARGUMENTY A VRACI JEDNU HODNOTU!
    std::cout << "[ERROR Module callback "<< module_id << ".launcher( prms ) ]: " << lua_tostring( l, -1 ) << std::endl;
  }
  else {
    cmd_str = lua_tostring( l, -1 );
    lua_pop( l, -1 );
  }
  
 return cmd_str;  
}

FXint luams_validate( const FXString &module_id, const FXString &flag_str, const FXString &entry_name, FXint *result )
{
  /* call: module_id.validate( flag_str, entry_name ): status, result */ 
  FXint status = 0;
  lua_State *l = _inst->getLua( );

  
  lua_getglobal( l, "validate" );
  lua_pushstring( l, convert_str( module_id ) ); 
  lua_pushstring( l, convert_str( flag_str ) ); 
  lua_pushstring( l, convert_str( entry_name ) );

  if( lua_pcall( l, 2, 2, 0 ) != 0 ) {
   std::cout << "[ERROR Module callback "<< module_id << ".validate( " << flag_str << ", " << entry_name << ") ]: " << lua_tostring( l, -1 ) << std::endl;
   status = -1;
  }
  else {
    status = lua_tointeger( l, -1 );
    lua_pop( l, -1 );
    if( status != 0 ) { 
      *result = lua_tointeger( l, -2 );
      lua_pop( l, -2 );
    } 
  }

  return status;
}

/*** END ******************************************************************************************/
