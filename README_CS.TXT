Gorgona – Správce herních kolekcí a spouštěč her pro OS Linux 
================================================================================
14.01.2023 

Obsah:
  1. Základní informace
  2. Překlad a instalace
  3. Známé chyby a bugy
  4. Stav projektu a plány do budoucna
  5. Autor a licence
  6. Internet
  
1. ZÁKLADNÍ INFORMACE:
=======================
PROJEKT ZASTAVEN – viz. Kapitola 4. 

Projekt Gorgona je určený jako správce a spouštěč her pod OS Linux. Je vytvořen v jazyce C++ s použitím GUI frameworku Fox. Jeho primárním úkolem bylo 
náhradit za Java aplikaci JGameLauncher od společnosti Parallel Realities. Hlavní motivací byla nutnost psát pro mnoho her shellove spouštěče, nebo 
nemožnost třídit hry podle určitých kritérií (např. podle žánru).  

Gorgona v teto chvíli víceméně své původní cíle splnila a v některých ohledech svůj původní vzor předběhla. Obsahuje interpretr jazyka Lua a několik 
vložených funkcí, které umožňují psát univerzální spouštěče her. V tuto chvíli umožňuje pouze ruční vložení hry do seznamu, a zadáni potřebných údajů o 
hře. Gorgona potom zařadí hru do odpovídající sekce, odkud ji lze poté také spouštět. Seznam her je možno zobrazit ve dvou variantách (ikony a podrobný 
výpis). Pro tento projekt a jeho části, jsou použity názvy z řecké mytologie :
  Gorgona – Správce her
  Morfeus – Univerzální GUI pro spouštění her (tzv. launcher).  

Screenshot: 
docs/images/screenshot.png

2. PŘEKLAD A INSTALACE
========================
Závislosti:
  Foxtoolkit 1.7.81 - GUI Framework http://fox-toolkit.org/ftp/fox-1.7.81.tar.gz
  FoxGHI 0.4.0      - https://github.com/PGSafarik/FoxGHI
  TinyXml           - https://sourceforge.net/projects/tinyxml
  Lua 5.1           - https://www.lua.org

Dodatečné závislosti pro kompilaci  
  cmake >= 3.10
  g++ 

Po stažení projektu z Githubu (a případném rozbaleni) přejděte do složky s projektem a zadejte následující 
sekvenci příkazu:
  mkdir build
  cd build 
  cmake .. && make
  sudo make install

Po úspěšné kompilaci a sestaveni projektu bude výsledný program, včetně spouštěcích modulu v jazyce Lua, nainstalovány do adresáře /opt/gorgona

pozn.:
Knihovna foxlib MUSÍ být v době kompilace nainstalována na vašem systému. Verze 1.7 je vývojová verze, je proto možné, ze ji nebudete mít v systému a 
nebude ani dostupná v repositáři, proto ji budete muset doinstalovat ručně. Více informací jak to udělat najdete na stránkách foxtoolkitu. Následující 
řádky se týkají případných úprav CMakeLists.

Mate-li knihovnu nainstalovanou na jiném, než standardním umístění, je nutno tuto skutečnost zanést v souboru CMakeLists.txt v adresáři projektu. 
Vyhledejte a změňte řádky nastavující cestu ke knihovně:
    INCLUDE_DIRECTORIES( "/Cesta/vasi/instalaci/foxlib/include" )   #Foxlib headers
    LINK_DIRECTORIES( "/Cesta/vasi/instalaci/foxlib/lib" )          #Foxlib library

Tyto řádky jsou ve výchozím stavu zakomentovány, po úpravě tedy ještě odstraňte počáteční znaky '#', a poté soubor uložte. Dále normálně postupujte podle 
instrukci pro sestavení a instalaci projektu výše.     

3. ZNÁME CHYBY A BUGY
======================
1. 28/03/2019 – Chybná návratová hodnota Lua spouštěčů - Otevřeno
  Lua spouštěče nevrací správně návratovou hodnotu po skončení takto spuštěné hry. Pravděpodobně jde o špatně vyzvednutou hodnotu z lua bufferu.
  
2. 11/01/2023 - Nutnost rucne zadat cestu k modulum v ModManager.lua - Otevreno  
   Příčina je v návrhu integrace Lua skriptů v Gorgoně. Pro zatím se uvažuje o workaroundu do C->LUA API.

4. STAV PROJEKTU A PLÁNY DO BUDOUCNA
======================================
PROJEKT ZASTAVEN!

V letech 2019/2020 se objevilo na Linuxové scéně více podobných programů, jako Gorgona. Především to byl Lutris, dále např. GameHub, které byly už v té 
době po stránce nabízených služeb a funkcí mnohem dál než Gorgona a mají (IMHO) větší vývojářskou a komunitní základnu. Vzhledem k tomuto faktu a k tomu, 
že jsem si sám oblíbil projekt GameHub, jsem se rozhodl práce na Gorgoně zastavit a v jejím vývoji (alespoň pro zatím) nepokračovat. Veškeré úpravy v 
projektu mají v tuto chvíli pouze udržovací charakter. 

Repositář projektu na Githubu však zůstane zachován pro případné zájemce, či pro budoucí generace, aby věděli, že takový projekt kdy existoval... 

Projekt Morfeus, který se vyvíjel souběžně s Gorgonou, byl od ní oddělen, jako nezávislá utilita a je nadále vyvíjen samostatně. 

11/01/2023 Aktualizace 
  -- Vychozi verze foxlib 1.7.81
  -- Top-level okna Gorgony jsou nyni vytvařena spravována FoxGHI knihovnou. Vychozi verze je 0.4.1  
  -- Nastrojové panely jsou nyni zpravovány FoxGHI Header Boxy
  -- Upraven layout top-level oken v návaznosti na změnu správy top-level oken
  -- Vyhledávání presunuto ze seznamu polozek do Header Baru 
  -- Přidán modul pro podporu spouštění her pomoci Steam clienta (zatím je nutno manulane vyhledat Steam app id a zadat do pole přikazu)
  -- Přidán modul pro podporu spouštění her nainstalovaných pomoci nástroje Legendary (Epic games a GOG.com. I zde je nutno manuálně zadat id hry)
  -- Jednoduchý nástin validace položek. Zatím kontroluje (a ne zcela správně) validnost podle existence spustitelneho souboru u nativnich polozek.
     Položky ktere jsou vzhodnoceny jako nevalidní, jsou vypsány do stdout.
  -- XML tagy reprezentující jednotlivé položky seznamu her jsou nyní seskupeny do xml tagu "Library". Tato zmena je momentálně nekompatibilní s předchozími 
     verzemi a původní datový xml soubor nebude správně načten. Je potreba jej ručně opravit, nebo založit nový. 
  -- Nekolik drobnych oprav a zmen
  
5. AUTOR A LICENCE
=====================
Copyright (C) 2015 - 2022 Pavel Šafařík <drakarax@seznam.cz> alias D.A.Tiger
všechna práva vyhrazena.

Tento soubor je součástí Gorgona.

Gorgona je svobodný software: můžete jej redistribuovat a/nebo upravovat za podmínek GNU General Public License, jak byla zveřejněna Free Software 
Foundation, buď verze 3 licence, nebo (dle vašeho uvážení) jakékoli 
pozdější verze.

Projekt Gorgona je distribuován v naději, že bude užitečný, ale BEZ JAKÉKOLI ZÁRUKY; dokonce bez předpokládané záruky PRODEJNOSTI nebo VHODNOSTI PRO 
KONKRÉTNÍ ÚČEL. Další podrobnosti najdete v GNU General Public License.

Spolu s Gorgonou byste měli obdržet kopii GNU General Public License. Pokud ne, podívejte se na <https://www.gnu.org/licenses/>.

6. INTERNET: 
===============
  Muj blog:                http://bfuplusplus.blogspot.cz/
  Morfeus:                 https://github.com/PGSafarik/Morfeus
  FoxGHI:                  https://github.com/PGSafarik/FoxGHI 
  Legendary:               https://github.com/derrod/legendary 
  SteamDB(EN):             https://steamdb.info/
  Parallel Realities (EN): https://www.parallelrealities.co.uk/
  Lutris (EN):             https://lutris.net/
  GameHub (EN):            https://tkashkin.github.io/projects/gamehub/

=== KONEC ===

