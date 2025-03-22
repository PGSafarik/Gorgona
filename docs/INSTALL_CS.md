# PŘEKLAD A INSTALACE
    - 22/03/2025 -

*Content:*
  1. [Získání projektu Gorgona](#1-Získání-projektu-Gorgona)
  2. [Závislosti](#2-Závislosti)
  3. [Kompilace ze zdrojových kódů](#3-Kompilace-ze-zdrojových-kódů)
  4. [Knihovna Fox](#4-Knihovna-Fox)
  5. [Spuštění programu](#5-Spuštění-programu)
  
---
## 1. Získání projektu Gorgona
Projekt Gorgona je momentálně hostován na Githubu, na adrese: https://github.com/PGSafarik/Gorgona . V tuto chvíli je možné získat projekt pouze ve formě zdrojových kódu v aktuální verzi vývoje.  

Je možné si projekt stáhnout přímo z odkazovaných stránek na Githubu, nebo alternativně pomocí Gitu:  
<code> 
  $ cd /cilovy/adresar/ke/stazeni/  
  $ git clone https://github.com/PGSafarik/Gorgona
</code>  

## 2. Závislosti
Momentálně nejsou k dispozici ani samostatná vydání nebo instalační balíky (i když se s nimi do budoucna počítá), takže je nutno Gorgonu na cílovém stroji zkompilovat, sestavit a nainstalovat ručně. K tomuto účelu je v kořenovém adresáři projektu připraven soubor CMakeList.txt, z něhož utilita cmake vygeneruje buildovací skripty a soubory s jejichž pomocí bude později program Gorgona sestaven a nainstalován. Přesný postup je popsán v bodě 3., nyní následuje seznam softwarových závislosti:   

* Vyžadováno (Musí byt na cílovém systému přítomny):
    * Fox toolkit 1.7.81            - GUI Framework                    http://fox-toolkit.org/ftp/fox-1.7.81.tar.gz
    * FoxGHI <= 24.11 0.4.4.1 ALPHA - GUI layout oken ve stylu Gnome3  https://github.com/PGSafarik/FoxGHI
    * TinyXml                       - DOM XML parser                   https://sourceforge.net/projects/tinyxml
    * Lua 5.1                       - Interpret jazyka Lua             https://www.lua.org

* Doporučené (Mohou být požadovány moduly, nebo rozšiřovat funkci Gorgony):  
    * Steam             - Oficiální klient služby Steam
    * Legendary         - Neoficiální klient pro Epic.com a GOG.com
    * Morfeus           - Univerzální GUI wraper (nadstavba) pro herní spouštěče 
    * Wine              - Systémová vrstva umožňující spouštění Windows aplikací na Linuxu a MacOS
    * Q4Wine            - GUI nadstavba nad Wine
    * Dosbox            - Emulátor pro aplikace systému DOS
    * xterm             - Emulátor terminálu

* Kompilační (Dodatečné závislosti pro úspěšnou kompilaci projektu)
    * cmake >= 3.10
    * g++

* Volitelně    
    * git

## 3. Kompilace ze zdrojových kódů
Po stažení projektu z Githubu (a rozbaleni) přejděte do složky s projektem a proveďte případné úpravy (viz např. sekce 4). Nakonec v terminálu zadejte následující sekvenci příkazu pro zkompilování a instalaci projektu (znak $ reprezentuje výzvu shellu).  
<code> 
  $ mkdir build  
  $ cd build  
  $ cmake .. && make  
  $ sudo make install  
</code>

Po úspěšné kompilaci a sestaveni projektu, bude výsledný program `gorgona` nainstalován do adresáře `/usr/bin/`,  skripty modulu v jazyce Lua a další podpůrné soubory do adresáře `/usr/share/gorgona`. Soubory pro integraci s desktopem pak do `/usr/share/pixmaps` a `/usr/share/applications`. 

## 4. Knihovna Fox
Je naprosto nutné, aby knihovny, které jsou vyžadované byly na cílovém systému přítomny v době spuštění programu Gorgona a během kompilace programu musí být k dispozici i vývojové soubory - hlavičkové soubory.  

Toto se týká především frameworku Fox toolkit a malé doplňkové knihovny FoxGHI. Vzhledem k tomu, že aktuální verze Gorgony je vyvíjena a kompilována proti vývojové verzi Fox, je pravděpodobné, že tato verze Fox nebude dostupná v repositáři distribuce na cílovém počítači. Naprosto jisté to bude v tuto chvíli u rozšiřující knihovny FoxGHI. 

Obě knihovny bude tedy pravděpodobně nutno nainstalovat ručně a je tak možné, že bude nutno upravit řádky v souboru CMakeLists.txt, kde jsou definovány cesty ke knihovnám a potřebným hlavičkovým souborům. To lze udělat snadno - změnou cesty nastavením proměnných LIB_FOX a LIB_FXGHI. Stávající nastavení je odvozeno od mého vlastní instalace:
<code>
SET( LIB_FOX   "/cesta/k/vyvojovym/souborum/foxlib" )       # Fox toolkit path  
SET( LIB_FXGHI "/cesta/k/vyvojovym/souborum/FoxGHI" )       # Fox GHI path  
</code>

Po této úpravě se již postupuje překladem podle sekce 3.

## 5. Spuštění programu
Program Gorgona, po úspěšném překladu a instalaci, bude zařazen do menu sekce hry, odkud jej lze pohodlně spouštět. Alternativně jej lze spustit z příkazové řádky terminálu:  
<code>
  $ Gorgona
</code>

---

    = Konec =









