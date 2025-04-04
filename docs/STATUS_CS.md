# STAV PROJEKTU GORGONA A PLÁNY DO BUDOUCNA
    = 22/03/2025 =

*Content:*
  1. [Novinky](#1-Novinky)
  2. [Známé chyby a bugy](#2-Známé-bugy-a-nedostatky)
  3. [Plány do budoucna](#3-Plány-do-budoucna)
    
---
## 1. Novinky  
#### 23/11/2024 - Aktualizace 1.6.6.3
* DŮLEŽITE: Přechod na novou verzi knihovny FoxGHI (0.4.4.1). Tato verze - mimo jiné - umožňuje klientským programům zjistit verzi libFXGHI přímo, a je zpětně
  nekompatibilní (na urovni zdrojoveho kodu. Již preložené starší verze Gorgony by měli i po upgradu FoxGHI nadále fungovat).

#### 13/05/2024 Oprava - 1.6.6.0
* FIXED - Opravena duplikace XML elementu "description" v knihovně herních titulů.
* FIXED - Opravena chyba se špatnou navratovou hodnotou, vracenou Lua funkci pro přimé spouštění procesu. 
* WORKAROUND - Modul Steamu dočasně umožnuje spustit klienta služby Steam při svém zavedení do paměti. Tím se předejde zablokovaní Steam her, při opětovném spuštění.

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
* Revize a přepracování základní uživatelské dokumentace (v českém jazyce). 
* Odebrán status projektu "Pozastaveno"
* Přípravné práce na implementaci nových funkcionalit a drobné úpravy

#### 11/01/2023 Aktualizace 1 - 1.5.0.1 
* Výchozí verze foxlib 1.7.81
* Top-level okna Gorgony jsou nyní vytvářena a spravována FoxGHI knihovnou. Výchozí verze je 0.4.1  
* Nástrojové panely jsou nahrazeny FoxGHI Header Boxy
* Upraven layout top-level oken v návaznosti na změnu správy top-level oken
* Vyhledávání přesunuto ze seznamu položek do Header Baru 
* Přidán modul pro podporu spouštění her pomoci Steam klienta (zatím je nutno manuálně vyhledat Steam app id a zadat do pole příkazu)
* Přidán modul pro podporu spouštění her nainstalovaných pomoci nástroje Legendary (Epic games a GOG.com. I zde je nutno manuálně zadat id hry)
* Jednoduchý nástin validace položek. Zatím kontroluje (a ne zcela správně) validnost podle existence spustitelného souboru u nativních položek.
  Položky které jsou vyhodnoceny jako nevalidní, jsou vypsány do stdout.
* XML tagy reprezentující jednotlivé položky seznamu her jsou nyní seskupeny do xml tagu "Library". Tato změna je momentálně nekompatibilní s předchozími 
  verzemi a původní datový xml soubor nebude správně načten. Je potřeba jej ručně opravit, nebo založit nový. 
* Dočasná deaktivace přímého spuštění procesu Gorgonou z LUA moodulu. Standardní rutina os.execute() samozřejmě funguje pořád. 
* Několik Drobných oprav a změn
    
## 2. Známé bugy a nedostatky
1. 11/01/2023 - Nutnost ručně zadat cestu k modulům v ModManager.lua - Otevreno
   Příčina je v návrhu integrace Lua skriptů v Gorgoně. Uvažuje se o dočasném workaroundu do C >LUA API, který bude dodávat cestu z nastavení Gorgony.
   Reseni() :   
   
2. 02/04/21023  - Spusteni hry spravovane Steamem zablokuje polozku hry i po ukonceni hry - OTEVŘENO.
   Příčina je v asociaci PID hlavniho procesu Steam klienta s danou položkou v Gorgoně. Problém odpadne umožněním modulu spustit Steam klienta dříve než samotnou hru. Hru je také možno opakovaně spustit přímo ze Steam klienta, nebo klienta nejdřív ukončit s následně hru spustit znovu z Gorgony
   Rešení (): V sekci "Steam" v konfiguračním souboru Gorgony, nastavit položku "Autostart" na hodnotu 1. Modul Steamu pak spustí klienta Steamu pri svem zavedení do paměti (pokud již není spuštěn). Jedna se o dočasný workaround, v nově chystané druhé verzi modulů nebude toto opatření. 

3. 08/04/2023 - Neoznamuje invalidni, nebo neexistujici data v datovem XML souboru - OTEVRENO  
   Pri startu programu a naxitani XML datoveho souboru, Gorgona nekontroluje validnost dat, pouze zda soubor existuje. Pokud data neodpovidaji ocekavani, program se standardne chova, jako by datovy soubor nebyl vytvoren, ci byl prazdny.
    - Program nenabizi uzivateli zadnou moznost kam a do jakeho souboru data v dany moment ulozit (pozn. rucne pres conf.soubor )
    - Pri ukoncovani ve vyse popsane situaci automaticky, bez upozorneni, zapise prazdny datovy soubor, cimz definitivne znici puvodni data v nevalidnim souboru.
   Reseni( ):  
   
4. 08/04/2023 - Nedostatecna funkcionalita vyhledavani polozek v knihovne - Uzavreno
   Reseni(11/08/2023): Prepracovat, pridano do seznamu ukolu  
   
5. 08/04/2023 - Vyhledavaci pole nereaguje na stisk klavesy \[ENTER\] - Uzavreno
   Vyhledavani zapocne pouze stisknutim GUI tlacitka "Find". Podle specifikace ma vsak byt zapocato i stisknutim klavesy ENTER, paklize ma vyhledavaci pole focus.  
   Reseni(11/08/2023): viz bod 5.  

## 3. Plány dalšího vývoje 
### Poznamky k verzovaní projektu
Pro sve projekty používám ukolové plánování. Tzn., že se určí několik cílů, jejichž naplněnění povede k vydání konkrétní hlavní (stabilní, nebo vývojové) verze a čas při tom nehraje žádnou roli. Seznam, cílů, které považuji za stěžejní pro konkrétní budoucí verzi je vypsán níže.   

Verzování je převzato ještě z Anjuta autoversion. Kompletni verze projektu, v dobe psani tohoto textu, je "03.08 1.6.4.3 Devel" a je slozena z datumové části, cisla, a statusu. Pro běžné užití však postačí (a pouziva se) pouze samotne cislo verze (tedy 1.6.4.3). To ma tvar MAJOR.MINOR.BUILD.REVISION. Status popisuje v jakem stavu vyvoje se dana verze zrovna nachazi. 

Duležita jsou prvni dvě pozice čísla verze. Stabilni verze je obvykle ta, ktera ma, mimo MAJOR pozice, vsechny ostatni na hodnote 0. Vyvojova verze pak ma hodnotu ruznou od nuly take na pozici MINOR. Posledni dve pozice slouzi jen k jemnějšímu rozlišeni pro účely vývoje. Ve velmi vzacných případech se může stav vydání změnit pomoci řetězce statusu. 

Vzdy jedna vývojová verze těsně pred vydaním stabilní bude zamražena a vyhrazena na testovaní provedených změn v projektu a ve statusu bude označena jako "Testing". To znamena, že se do ní v té době nebudou přidávat žádné nové funkce a pokud se bude zasahovat do kodu projektu, tak jedine pro opravu nalezených chyb.  



### Následující vývojová verze (1.7.0.0)
1. Přeložit základní technickou dokumentaci do Aj. Stručný uživatelsky manuál v Čj.
2. Nahradit současně používanou, ale nepodporovanou TinyXml 1.x na novější TinyXml 2.x ( a to už docela hoří! )
3. Přepracování struktury položek titulu

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
14. Autooprava nekompatibilních změn v xml seznamu her
16. Implementace štítků (seznam hesel, něco jako hashtagy, do tématických okruhů jako např. kategorie, platforma, atd...) 
17. Klavesové zkratky a kompletni ovladani programu pomoci klavesnice
18. Podpora hernich zarizeni (jmenovite gamepadu a joysticku)
19. Vykonnejsi vyhledavani polozek v knihovne

---

    = Konec =
