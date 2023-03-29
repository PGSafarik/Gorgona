# STAV PROJEKTU GORGONA A PLÁNY DO BUDOUCNA
    - 28/03/2023 -

*Content:*
  1. [Novinky](#1-Novinky)
  2. [Známé chyby a bugy](#2-Známé-bugy-a-nedostatky)
  3. [Plány do budoucna](#3-Plány-do-budoucna)
    
---
## 1. Novinky    
#### 13/03/2023 Aktualizace 2 - 1.6.0.0 
* Realizace vnitřního modulu PERSEUS a přepracovaní mechanismu spouštění nových procesů.
* První změny ve vnitřní mechanice API pro jazyka Lua  
* Přepracování inicializačních procedur aplikace  
* Přidán modul pro aplikaci Morfeus
* Přidaná možnost spouštět hry přes okno terminálu (zatím pouze xterm)
* Úpravy v zobrazovaní seznamu her
* Hru v seznam je možno nyní spustit dvojklikem na danou položku
* Gorgona registruje počet spuštěni a datum posledního spuštěni dané hry
* V režimu zobrazeni "detaily" se nově zobrazuje též datum posledního spuštěni a žánr hry
* Revize a přepracování uživatelské dokumentace (v českém jazyce). 
* Odebrán status projektu "Pozastaveno"
* Přípravné práce na implementaci nových funkcionalit a drobné úpravy

#### 11/01/2023 Aktualizace 1 - 1.5.0.1 
* Výchozí verze foxlib 1.7.81
* Top-level okna Gorgony jsou nyní vytvářena a spravována FoxGHI knihovnou. Výchozí verze je 0.4.1  
* Nástrojové panely jsou nahrazeny FoxGHI Header Boxy
* Upraven layout top-level oken v návaznosti na změnu správy top-level oken
* Vyhledávání přesunuto ze seznamu položek do Header Baru 
* Přidán modul pro podporu spouštění her pomoci Steam klienta (zatím je nutno manuálně vyhledat Steam app id a zadat do pole přikazu)
* Přidán modul pro podporu spouštění her nainstalovaných pomoci nástroje Legendary (Epic games a GOG.com. I zde je nutno manuálně zadat id hry)
* Jednoduchý nástin validace položek. Zatím kontroluje (a ne zcela správně) validnost podle existence spustitelného souboru u nativních položek.
  Položky které jsou vyhodnoceny jako nevalidní, jsou vypsány do stdout.
* XML tagy reprezentující jednotlivé položky seznamu her jsou nyní seskupeny do xml tagu "Library". Tato změna je momentálně nekompatibilní s předchozími 
  verzemi a původní datový xml soubor nebude správně načten. Je potřeba jej ručně opravit, nebo založit nový. 
* Několik Drobných oprav a změn
    
## 2. Známé bugy a nedostatky
1. 28/03/2019 – Chybná návratová hodnota Lua spouštěčů - Uzavřeno  
   Lua spouštěče nevrací správně návratovou hodnotu po skončení takto spuštěné hry. Pravděpodobně jde o špatně vyzvednutou hodnotu z Lua bufferu.  
     
   Řešeni (11/01/2023): Do finálního přepisu LUA API není podporováno přímé spouštění procesu z LUA modulu

2. 11/01/2023 - Nutnost ručně zadat cestu k modulům v ModManager.lua - Otevřeno  
   Příčina je v návrhu integrace Lua skriptů v Gorgoně. Uvažuje se o dočasném workaroundu do C >LUA API, který bude dodávat cestu z nastavení Gorgony.

## 3. Plány dalšího vývoje 
### Následující verze (1.7.X.X)
1. Přeložit základní technickou dokumentaci do Aj. Stručný uživatelsky manuál v Čj.
2. Nahradit současně používanou, ale nepodporovanou TinyXml 1.x na novější TinyXml 2.x ( a to už docela hoří! )
3. Přepracování položek jednotlivých titulu

### Příští Stable vydání (2.0.0.0)
1. Konfigurační GUI (nyní je možno konfigurovat Gorgonu pouze ručně v souboru)
2. Přepracovaní Gorgona LUA API
3. Implementace Nového systému modulu
4. Přepracovaní a nahrazení stávajících modulu
5. Implementace rozhraní pro synchronizaci herních dat s datovým úložištěm (ext. hdd, cloud, ...)
6. Automatická detekce závislostí CMake/make
7. Přepracovaní zprávy ikon a obrázků

### Vzdálenější budoucnost
1. Detekce, kontrola a validace nainstalovaných her
2. Instalační procedury
3. Podpora internetových herních databázi
4. Podpora pro KGamesLib a Gnome Games
5. Podpora uživatelských herních repositářů
6. Podpora distribučních repositářů (DEB, RPM a ... uvidí se)
7. Webové služby přes Luakit web browser
8. Podpora shellu a parametry pro příkazovou řádku.
9. Implementace služeb a kompatibility s PlayOnLinux
10. Implementace exportu titulu z/do Lutrisu a GameHub
11. Podpora CSS, animace a skinovani
12. Umožnit delegované spouštění her 
13. Podpora obrázkových dat ve formátu SVG.
14. Autooprava nekompatibilnich zmen v xml seznamu her
16. Implementace štítků (seznam hesel, něco jako hashtagy, do tématických okruhů jako např. kategorie, platforma, atd...) 

---

    = Konec =
