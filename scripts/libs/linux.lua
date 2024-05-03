
---[ CONSTANTS ]-------------------------------------------------------------------------------------
io.FILESEPARATOR = "/"

---[ IMPLEMENT ]-------------------------------------------------------------------------------------
local function _eval_shell_cmd( command )  
  assert( type( command ) == "string" )
  assert( command ~= "" )
    
  local stream = io.popen( command )
  if stream then return stream:read( ), stream:close( ) end  
end

local function _eval_shell_test( flag, file_a, file_b )
  assert( type( flag ) == "string" and type( file_a ) == "string" )  
  assert( file_a ~= "" ) 
  
  local test_str = "" 
  if not file_b then test_str = "-" .. flag .. " " .. file_a 
  else 
    assert( type( file_b ) == "string" )  
    assert( file_b ~= "" )
    test_str = file_a .. " -" .. flag .. " " .. file_b 
  end
      
  local res   
  _, _, res = os.execute( "/usr/bin/test " .. test_str )
  if res == 0 then return true else return false end
end

local function _eval_shell_which( exec_name )  
  assert( type( exec_name ) == "string" )
  assert( exec_name ~= "" )
  
  return _eval_shell_cmd( "/usr/bin/which " .. exec_name ) 
end

local function _eval_shell_exist( file ) 
  assert( type( file ) == "string" )
  assert( file ~= "" )
  
  return _eval_shell_test( file, "e" ) 
end    

local function _file_name( path ) return path:gsub( "(.*/)(.*)", "%2" ) end
local function _dir_name( path ) return path:gsub( "(.*/)(.*)", "%1" ) end    
local function _file_extension( filename )      
    local n, e = filename:match( "(.*)%.(.*)" ) 
    n = _file_name( n )
    return n, e
end

---[ STANDARD MODULES UPGRADE ]----------------------------------------------------------------------
os.dirname    = _dir_name           -- Z retezce cesty odrizne nazev ciloveho souboru (/adresare), existuje-li. Tzn., vrati vse DO posledni nalezen '\'.     
os.filename   = _file_name          -- Z retezce cesty extrahuje nazev ciloveho souboru (/adresare), existuje-li. Tzn., vrati vse OD psoledniho nalezeneho '\'.
os.fext       = _file_extension     -- Rozdeli nazev souboru (bez cesty) na jemnou cast a priponu (existuje-li). 
os.which      = _eval_shell_which   -- Binding prikazu which - vrati cestu ke spustitelnemu souboru podle jeho nazvu
os.test       = _eval_shell_test    -- Binding prikazu test  - man test. Pouze na testovani souboru 
os.exist      = _eval_shell_exist   -- True, pokud soubor existuje

io.cmdstream  = _eval_shell_cmd     -- Spusti shellovy prikaz a vrati jeho vystup

--[[ END ]]---------------------------------------------------------------------------------------- 
