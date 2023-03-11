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
Doom = { }
Doom_mt = { __index = Doom }


--- Implement module ---
function Doom_Init( )
  self = { }
	self.app = "/usr/games/prboom"
	self.datadir = "/usr/share/games/doom/"
	
	self.info = { 
		name      = "Doom",
		date      = "27/07/2016",
		version   = "0.1.0",
		modmanver = "0.1.0",
		author    = "D.A.Tiger; drakarax@seznam.cz",
		licencion = "GNU GPL v3 or latter",
		descript  = "Module for running games in FreeDoom client"
	}
	
	return setmetatable( self, Doom_mt )
end

function Doom:launcher( t )
	return self.app .. " " .. self.datadir .. t[ 1 ]
end

function Doom:test( str )
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
Mod_Register( "Doom", Doom_Init( ) )


