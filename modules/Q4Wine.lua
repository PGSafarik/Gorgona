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
Q4Wine = { }
Q4Wine_mt = { __index = Q4Wine }


--- Implement module ---
function Q4Wine_Init( )
  self = { }
	self.app = "/usr/bin/q4wine-cli"
	self.info = { 
		name      = "Q4Wine",
		date      = "27/07/2016",
		version   = "0.1.0",
		modmanver = "0.1.0",
		author    = "D.A.Tiger; drakarax@seznam.cz",
		licencion = "GNU GPL v3, or letter",
		descript  = "This module is used for running games and utilities writen for \noperation system Windows. This module using project Q4Wine, created \nby Alexey S. Malakov."
	}	
	return setmetatable( self, Q4Wine_mt )
end

function Q4Wine:launcher( cmd )
local data = split( cmd, ":" )
  local prefix = data[ 1 ] or "default"
  local index  = data[ 2 ]
  if index == nil or index == "" then index = prefix end
  
  print( "prefix = " .. prefix )
  print( "index = " .. index )
  local cmd = self.app .. " -p " .. prefix .. " -i " .. index .. "" 	
	print( "prikaz ke spusteni = " .. cmd )
	
	return cmd
end

function Q4Wine:test( str )
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
Mod_Register( "Q4Wine", Q4Wine_Init( ) )

