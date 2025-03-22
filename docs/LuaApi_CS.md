# Gorgona Lua API 
    = 22/03/2025 Version 1 =  

## Reference 
### System
* **Execute( cmd, arg1, arg2, ... )**  
  Prime spusteni noveho procesu, obdoda lua fukce `os.execute( )`. Avsak na rozdil od ni, funkce `Execute( )` pozada Gorgonu, aby tuto operaci vykonala a nove spusteny proces je tak primym potomkem Gorgony, ktera si jej hned po uspesnem spusteni zaregistruje a je pod jeji primou zpravou a dohledem. Gorgona vytvari sve potomky primo - bez volani systemove funkce `system( )`, ani k tomuto ucelu sama nespousti zadny novy proces shelu, ne ktery by tuto akci delegovala. Funkce je zamyslena hlevne pro potreby modulu, jako je spousteni pomocnych procesu, nebo hernich platforem. Neni urcena pro vyuziti v 'launcher' sluzbe modulu (viz).  
  
  Funkce `Execute( )` vraci pri uspechu **PID** nove spusteneho procesu, v pripade neuspechu 0.  
  
### Gorgona settings registry    
  Jedna se o kolekci funkci umoznujicich pristup do registru nastaveni Gorgony a manipulaci se jeho zaznamy. Je potreba vzit na vedomi, ze uzivatel muze v nastaveni zakazat modulu menit nastaveni Gorgony, nebo veskera nastaveni, mimo sekce s nazvem modulu.  
  
* **Register_write( section, key, value )**  
  Tato fukce vyhleda v sekci (_section_) pozadovany klic (_key_) a zapise do nej zadanou hodnotu (_value_). Pokud klic, nebo sekce neexistuji budou vytvoreny. Navratovou hodnotou je v pripade neuspechu _false_, jinak _true_.  

* **Register_read( section, key, value )**  
  Tato fukce vyhleda v sekci (_section_) pozadovany klic (_key_) a vrati hodnotu ktera je pod timto klicem ulozena (_value_). Pokud klic, nebo sekce neexistuji nebo neni zadana zadna hodnota, muze byt vracena vychozi hodnota (_default_). Navratovou hodnotou je v pripade neuspechu _false_, jinak _true_.  

* **Register_test( section\[, key\]  )**  
  Pristup do registru nastaveni Gorgony. Tato fukce vyhleda sekci (_section_) a pripadne podrizeny klic (_key_) a pro kazdou z techto hodnot vrati _true_, nebo _false_, podle to zda existuji ci ne.  
  
* **Register_delete( section, key )**  
  Odstrani vybrany klic, pripadne celou sekci. Pro kazdou uvedenou hodnotu vrati _true_, v pripade uspechu jinak _false_.  

###  Lua extension - string

* **string.split( str, separator, array )**  
  Funkce rozdeli retezec (_str_) na pole subretezcu podle vyskytu zadaneho separatoru. Pokud je predano pole (_array_) budou subretezce vlozeny do nej, jinak funkce vrati iterator.  

* **string.join( array, separator )**  
  Opak funce string.split( ). Prvky jednorozmerneho pole (_array_) spoji tak, jak jdou za sebou v jeden retezec, oddelene od sebe separatorem.  
  
* **string.trim( str )**  
  Odstrani vsechny mezery na zacatku i na konci retezce (_str_).__
  
* **string.right( str, length, position )**  
  Funkce vrati cast retezce (_str_) od konce o delce (_length_). Posledni parametr (_position_) posune zacatek subretezce.__
  
* **string.left( str, length, position )**  
  Funkce vrati kopii casti retezce (_str_) o delce (_length_). Posledni parametr (_position_) posune zacatek subretezce.__

* **string.compute_hash( str )**  
  Vypocita hashovaci cislo vstupniho retezce (_str_)
  
### Lua extension - os

* **os.dirname( path )**  
  Z retezce cesty (_path_) odrizne nazev ciloveho souboru (/adresare), existuje-li. Tzn., vrati vse DO posledni nalezeneho znaku '\'.
  
* **os.filename( path )**  
  Z retezce cesty (_path_) extrahuje nazev ciloveho souboru (/adresare), existuje-li. Tzn., vrati vse OD psoledniho nalezeneho znaku '\'.

* **os.fext( filename )**  
  Rozdeli nazev souboru (_filename_), bez cesty, na jmenou cast a priponu (existuje-li). 
  
* **os.which( name )**  
  Binding prikazu which - vrati cestu ke spustitelnemu souboru podle jeho nazvu (_name_)

* **os.test( flag, file_a, file_b )**  
  Binding prikazu test. Pouzitelny pouze na testovani souboru.  
  
* **os.exist( file )**  
  Vrati true, pokud zadany soubor (_file_) existuje.  
  
* **os.cmdstream( command )**  
  Spusti shellovy prikaz (_command_) a vrati jeho vystup.__
