# PŘEKLAD A INSTALACE
    - 19/03/2023 -

*Content:*
  1. [Získání projektu Gorgona](#1-Získání-projektu-Gorgona)
  2. [Závislosti](#2-Závislosti)
  3. [Kompilace ze zdrojových kódů](#3-Kompilace-ze-zdrojových-kódů)
  4. [Knihovna Fox](#4-Knihovna-Fox)
  5. [Spuštění programu](#5-Spuštění-programu)
  
---
## 1. Získání projektu Gorgona
Projekt Gorgona je momentálně hostován na Githubu, na adrese: https://github.com/PGSafarik/Gorgona . V tuto chvíli je možné získat projekt pouze ve formě zdrojových kodů v aktuální verzi vývoje.  

Je mozne si projekt stahnout přímo z odkazovaných stránek na Githubu, nebo alternativně pomocí gitu:  
<code> 
  $ cd /cilovy/adresar/ke/stazeni/  
  $ git clone https://github.com/PGSafarik/Gorgona
</code>  

## 2. Závislosti
Momentálně nejsou k dispozici ani samostatná vydání nebo instalcni balíky (i kdyz se s nimi do budoucna počítá), takze je nutno Gorgonu na cílovém stroji zkompilovat, sestavit a nainstalovat rucne. K tomuto učelu je v kořenovém adresáři projektu pripraven soubor CMakeList.txt, z něhož utilita cmake vygeneruje buildovací scripty a soubory s jejichž pomocí bude později program Gorgona sestaven a nainstalován. Přesný postup je popsán v bodě 3., nyní následuje seznam softwarových zavislostí:   

* Vyžadováno (Musi byt na cilovem systému přítomny):
    * Foxtoolkit 1.7.81 - GUI Framework                    http://fox-toolkit.org/ftp/fox-1.7.81.tar.gz
    * FoxGHI 0.4.0      - GUI layout oken ve stylu Gnome3  https://github.com/PGSafarik/FoxGHI
    * TinyXml           - DOM XML parser                   https://sourceforge.net/projects/tinyxml
    * Lua 5.1           - Iterpret jazyka Lua              https://www.lua.org
* Doporučené (Mohou být požadovány moduly, nebo rozšiřovat funkci Gorgony):  
    * Steam             - Oficialni klient sluzby Steam
    * Legendary         - Neoficialni klient pro Epic.com a GOG.com
    * Morfeus           - Univerzal GUI wraper (nadstavba) pro herní spouštěče 
    * Wine              - Systémová vrstva umožňující spouštění windows aplikací na Linuxu a MacOs
    * Q4Wine            - GUI nadstavba nad Wine
    * Dosbox            - Emulátor pro aplikace systemu DOS
    * xterm             - Emulátor terminálu
* Kompilační (Dodatečné závislosti pro úspěšnou kompilaci projektu)
    * cmake >= 3.10
    * g++
* Volitelně    
    * git

## 3. Kompilace ze zdrojových kódů
Po stažení projektu z Githubu (a rozbaleni) přejděte do složky s projektem a proveďte případné upravy (viz např. sekce 4). Nakonec v terminálu zadejte následující sekvenci příkazu pro zkompilování a instalci projektu (znak $ reprezentuje výzvu shellu).  
<code> 
  $ mkdir build  
  $ cd build  
  $ cmake .. && make  
  $ sudo make install  
</code>

Po úspěšné kompilaci a sestaveni projektu, bude výsledný program `gorgona` nainstalovan do adresare `/usr/bin/`,  skripty modulu v jazyce Lua a další podpurné soubory do adresáře `/usr/share/gorgona`. Soubory pro integraci s desktopem pak do `/usr/share/pixmaps` a `/usr/share/applications`. 

## 4. Knihovna Fox
Je naprosto nutné, aby knihovny, které jsou vyžadované byly na cílovém systému přítomny v době spuštění programu Gorgona a během kompilace programu musí být k dispozici i vývojové soubory - hlavičkové soubory.  

Toto se týká především frameworku Foxtoolkit a malé doplňkové knihovny FoxGHI. Vzhledem k tomu, že aktuální verze Gorgony je vyvíjena a kompilována proti vývojové verzi Fox, je pravděpodobné, že tato verze Fox nebude dostupná v repositáři distribuce na cílovém počítači. Naprosto jisté to bude v tuto chvili u rozšiřující knihovny FoxGHI. 

Obě knihovny bude tedy pravděpodobně nutno nainstalovat ručně a je tak možné, že bude nutno upravit řádky v souboru CMakeLists.txt, kde jsou definovány cesty ke knihovnám a potřebným hlavičkovým souborům. To lze uděla snadno změnou cesty nastavením proměnných LIB_FOX a LIB_FXGHI. Stavající nastavení je odvozeno od mého vlastní instalace:  
<code>
SET( LIB_FOX   "/cesta/k/vyvojovym/souborum/foxlib" )       # Fox toolkit path  
SET( LIB_FXGHI "/cesta/k/vyvojovym/souborum/FoxGHI" )       # Fox GHI path  
</code>

Po této úpravě se již postupuje překladem podle sekce 3.

## 5. Spuštění programu
Program Gorgona, po uspěšném překladu a instalaci, bude zařazen do menu sekce hry, odkud jej lze pohodlně spouštet. Alternativně jej lze spustit z příkazové řádky terminálu:  
<code>
  $ Gorgona
</code>

---

    = Konec =









