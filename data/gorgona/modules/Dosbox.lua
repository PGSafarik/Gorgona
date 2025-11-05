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

Dosbox = { }
Dosbox_mt = { __index = Dosbox }


function Dosbox_Init( )
  self = { }
	self.app = "/usr/bin/dosbox"
	self.prefixes = { "dosboxconf" }
	
	self.info = { 
		name      = "Dosbox",
		date      = "27/07/2016",
		version   = "0.1.0",
		modmanver = "0.1.0",
		author    = "D.A.Tiger; drakarax@seznam.cz>",
		licencion = "GNU GPL v3 or latter",
		descript  = "This module is used for running games and utilities writen for\noperating system DOS."
	}
	
	return setmetatable( self, Dosbox_mt )	
end


function Dosbox:launcher( t )
  --print( "=== Dosbox luancher ===" )
  local l_str = ""
  local cmd   = t[ 1 ]
  
  if string.sub( cmd, string.len( cmd ) - ( string.len( self.prefixes[ 1 ] ) - 1 ) ) == self.prefixes[ 1 ] then
    l_str = self.app .. " -conf " .. cmd
  else l_str = self.app .. " " .. cmd end
  
  return l_str	
	
end	

function Dosbox:test( str )
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

Mod_Register( "Dosbox", Dosbox_Init( ) )
