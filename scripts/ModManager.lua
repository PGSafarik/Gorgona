--[[
 This program is free software: you can redistribute it and/or modify   
 it under the terms of the GNU General Public License as published by   
 the Free Software Foundation, either version 3 of the License, or      
 (at your option) any later version.                                    
                                                                        
 This program is distributed in the hope that it will be useful,        
 but WITHOUT ANY WARRANTY; without even the implied warranty of         
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          
 GNU General Public License for more details.                           
                                                                        
 You should have received a copy of the GNU General Public License      
 along with this program.  If not, see <https://www.gnu.org/licenses/>. 
--]]
scripts_dir     = "/home/gabriel/Projects/Fox/sources/Gorgona/scripts"
package.path = package.path .. ";" .. scripts_dir .. "/libs/?.lua"
 
require "extstring"
require "linux"


--- Rutiny rizeni modulu ---
function Mod_Initialize( ) 
	if Modules == nil then Modules = { } end 
	if ltable == nil then ltable = { } end 
	
	-- FIXME MOD_01 : Get this path from Gorgona !!!
	--Mod_Load( "/usr/share/Gorgona/modules/" )
    Mod_Load( )
    
	print( "List of Launchers id: " )
  for i = 1, #ltable do print( ltable[ i ] ) end 
end

function Mod_Register( _name, _mod )
	local nfo = "[ Gorgona:Luamodules::Mod_register INFO ] Loading this module \t\t\t [ Failed ]"
  
  if _name == nil or _name == "" then
    print( "[ Gorgona:Lua_modules::Mod_Register ERROR ] Musi byt zadan nazev inicializovaneho modulu" )
    return -1
  end
  
  if Modules[ _name ] ~= nil  then
    print( "[ Gorgona:Luamodules::Mod_register WARNING ] Modul " .. _name .. " jiz existuje." )
    print( "Tato operace provede zameni! Muze dojit k nespravnemu chovani Gorgony v dusledku zameny dat a " )
		print( "funkci modulu! " ) 
  end
  
	if _mod ~= nil then 
    Modules[ _name ] = _mod
		if _mod[ "launcher" ] ~= nil then table.insert( ltable, #ltable + 1, _name ) end
		nfo =  "[ Gorgona:Luamodules::Mod_register INFO ] Loading this module \t\t\t [ OK ]" 
	end	

	return test( _name, nfo )
end

function Mod_Load( )
    local modules_dir = scripts_dir .. "/modules"
	local list = io.popen( "ls -1 " .. modules_dir )
	for file in list:lines(  ) do
      if file ~= "Launchers.lua" and file ~= "ModManager.lua" and file ~= "template.lua" and file ~= "libs" then
	    file = modules_dir .. "/" .. file
	    print( file )
	    dofile( file )
	  end	
	end	
end	

--- Callbacky ---
function test( _modname, _str )
	-- Testovaci funkce pro ladeni modulu
		local mod = Modules[ _modname ]
  if mod == nil then 
    print( "[ Gorgona:Lua_modules::launcher ERROR ] Modul " .. _modname .. " nenalezen" )
    return "-1"
  end
	if mod[ "test" ] == nil then
		print( "[ Gorgona:Lua_modules::launcher ERROR ] Modul " .. _modname .. " nema funkci test( )" )
		return "-2"
	end	
  
  --return mod:test( _str )
  return 0
end	

function launcher( _client, _command )
	-- rozlozeni prikazu hry a ziskani kompletniho spousteciho prikazu
	if _client == nil or _client == "" or _client == "native" then return "-1" end
	local mod = Modules[ _client ]
  if mod == nil then 
    print( "[ Gorgona.Perseus.Lua_modules::launcher ERROR -3 ] Client " .. _client .. " nenalezen" )
		MessageBox( "Modul " .. _client .. " nenalezen\n( Gorgona.Perseus.Lua_modules::launcher ERROR -3 )",  "Hru/aplikaci nelze spustit!", mbox_buttons.ok, mbox_type.error )
    return "-3"
  end
      
  return mod:launcher( _command )
end


Mod_Initialize( )


--[[
print( "=== DEBUG ===========" )
test( "Dosbox", launcher( "Dosbox", "test.dosboxconf" ) )
test( "Q4Wine", launcher( "Q4Wine", "GameTop:thinclient" ) )
test( "WebClient", launcher( "WebClient", "www.gog.com" ) )
test( "Console", launcher( "Console", "test" ) )
test( "Doom", launcher( "Doom", "doom2.wad" ) )
--]]

print( "[ Gorgona:Lua_modules INFO ] Module manager Loaded" )
print( "" )

--- END ---


