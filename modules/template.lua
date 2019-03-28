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
Debian = { }
Debian_mt = { __index = Debian }


--- Implement module ---
function Debian_Init( )
  self = { }
	self.app = "<path/to/ussed/application>"
	self.info = { 
		name      = "Debian",
		date      = "31/07/2016",
		version   = "0.0.1",
		modmanver = "0.1.0",
		author    = "D.A.Tiger; drakarax@seznam.cz",
		licencion = "GNU GPL v3 or latter",
		descript  = "Supprot for Debian / GNU Linux"
	}
	
	return setmetatable( self, Debian_mt )
end

function Debian:launcher( cmd )
	
	
end

function Debian:test( str )
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
Mod_Register( "Debian", Debian_Init( ) )


