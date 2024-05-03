_G.string._TRIMING = true         -- Enable internal using string.trim 

local function _trim( str )
  if not str then return end  
  return string.match( str, "^%s*(.-)%s*$" )    
end    

local function _left( str, length, pos )
  if str and ( length and length > 0 ) then 
    if not pos or pos <=  0 then pos = 1 end     
    return str:sub( pos, length + pos - 1 ) 
  end  
end

local function _right( str, length, pos )
  if str and ( length and length > 0 ) then
    if not pos or pos <= 0 then pos = 1 end     
    return str:sub( -( length + pos - 1 ), -pos )
  end  
end    

local function _split( str, sep, t )
  sep = sep or ","
  local it = string.gmatch( str, "[^" .. sep .. "]+" )
  if t then 
    for item in it do 
     if string._TRIMING then item = _trim( item ) end 
     table.insert( t, item ) 
    end

    return t
  end
  
  return it  
end
    
local function _join( t, sep )  
  if not t then return nil end
  
  sep = sep or " "  
  local resh = "" 
  
  for i = 1, #t do 
    local entry = t[ i ]
    if entry then
      if i == 1 then resh = t[ i ] else resh = resh .. sep .. t[ i ] end   
    end  
  end    
  
  return resh
end    

local function _compute_hash( str )
  p          = 31 
  m          = 1e9 + 9
  hash_value = 0
  p_pow      = 1

  for c in string.gmatch( str, "." ) do
     hash_value = ( hash_value + ( c:byte( ) - string.byte( "a" ) + 1 ) * p_pow ) % m
     p_pow = ( p_pow * p ) % m
  end

  return hash_value
end

string.split  = _split        -- funkce rozdeli retezec na pole subretezcu podle vyskytu zadaneho separatoru. Vraci iterator, nebo pole polozek
string.join   = _join         -- Opak funce split. Prvky jednorozmerneho pole spoji tak, jak jdou za sebou v jeden retezec, oddelene od sebe separatorem
string.trim   = _trim         -- Odstrani vsechny mezery na zacatku i na konci retezce 
string.hash   = _compute_hash -- Vypocita hashovaci cislo vstupniho retezce 
string.right  = _right        -- Funkce vrati cast retezce od konce o delce length
string.left   = _left         -- Funkce vrati kopii casti retezce o delce length
