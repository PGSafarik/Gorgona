Gorgona – Správce herních kolekcí a spouštěč her pro OS Linux 
================================================================================
13.03.2023 

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
  Viz. soubor docs/INSTALL_CS.md   

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

11/01/2023 Aktualizace 1
13/03/2023 Aktualizace 2
  
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

