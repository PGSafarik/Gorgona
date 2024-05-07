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

--- Declare module ---
local module = { }
local module_mt = { __index = module }

-- Helper functions
local function SteamUrl( action, appid ) 
    return "steam://" .. action .. "/" .. appid
end

--- Implement module ---
function module_Init( )
  self = { }
  --self.exec = "/usr/bin/steam"
  --self.steamURL = "steam://rungameid/"
  
  self.info = { 
    name      = "Steam",
	  date      = "13/01/2023",
	  version   = "0.0.1",
	  modmanver = "0.1.0",
	  author    = "D.A.Tiger; drakarax@seznam.cz",
	  licencion = "GNU GPL v3 or latter",
	  descript  = "Support for the Steam game launcher"
  }
	
  local _name = string.lower( self.info.name )
  self.exec = os.which( _name )
  
  if Register_read( "Steam", "Autostart", "0" ) == "1" then
    local _pid  = os.process( _name )
    if _pid == 0 then _pid = Execute( self.exec ) end
  end
  
	return setmetatable( self, module_mt )
end

function module:launcher( t )  
  return self.exec .. " " .. SteamUrl( "rungameid", t[ 1 ] )
end

function module:test( str )
	print( self.info.name .. " - Gorgona module" )
	print( "==============================================" )
	print( self.info.descript )
	print( "Version: " .. self.info.version )
	print( "Copyright ".. self.info.date .. " " .. self.info.author.. "." )
	print( "Licencion: " .. self.info.licencion )
	print( "Required Gorgona module manager version: " .. self.info.modmanver )
	if str ~= nil then print( str ) end
	print( "" )
	return 0	
end

--- Inicialized module ---
Mod_Register( "Steam", module_Init( ) )


