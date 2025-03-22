
# Gorgona, knihovna her pro OS Linux   
    - 22.03.2025 -

*Obsah:*

  1. [Základní informace](#1-Základní-informace)
  2. [Vlastnosti](#2-Vlastnosti)
  3. [Dokumentace](#3-Dokumentace)
  4. [Autor a licence](#4-Autor-a-licence)
  
---

## 1. ZÁKLADNÍ INFORMACE:
Program Gorgona slouží k vedení knihovny nainstalovaných her pod operačním systémem Linux, jejich třídění do kategorii a spouštění. Vývoj Gorgony začal někdy kolem roku 2015 jako náhrada za Java aplikaci JGameLauncher od společnosti Parallel Realities. Po několikaleté přestávce je vývoj projektu zase obnoven.

Vyvíjena je v jazyce C++ a GUI obstarává framework Fox. Obsahuje také interpretr jazyka Lua a definuje jednoduché API, které umožňují psát vytvářet pluginy (zásuvné moduly) pro obsluhu specifických požadavků her. Knihovna herních titulů je realizovaná pomocí značkovacího jazyka XML, s nímž program pracuje přes C++ DOM parser TinyXML.

Prosím, vezměte na vědomí, že program Gorgona se neustále vyvíjí a proto může - mimo stable vydání - obsahovat (a obsahuje) chyby, některé funkce programu se mohou bez varování změnit, či být rovnou vyřazeny. Více informací naleznete v uživatelské dokumentaci. 

<p align="center"><img src="docs/images/screenshot.png?raw=true" width="70%" />

## 2. VLASTNOSTI
1. Vkládaní, editace, odebírání spouštěčů her do knihovny
2. Zařazení položky v knihovně do uživatelsky definované kategorie
3. Gorgona nabízí zásuvné moduly (pluginy) pro ošetření spouštění specifických her.  
    1. **Console**   - umožňuje spouštět hry v textovém režimu (pozn.1).
    2. **Dolom**     - Modul pro spouštění her typu Doom 1, 2, FreeDoom. *Vyžaduje nainstalovaný Pr boom*
    3. **Dosbox**    - Modul pro spouštění DOSových her. *Vyžaduje nainstalovaný emulátor Dosbox* 
    4. **Legenadry** - Umožní spouštět hry instalované z Epic store. *Vyžaduje nainstalovanou aplikaci legendary*
    5. **Steam**     - Spouští hry ze služby Steam (pozn 2). *Vyžaduje klienta služby Steam*
    6. **Q4Wine**    - Slouží ke spouštění her přes Wine. *Vyžaduje utilitu Q4Wine*
    7. **Morfeus**   - Umožní přímo volat GUI Launcher Morfeus (pozn. 3). *Musí být nainstalovaný Morfeus*

poznámky:  

1. Gorgona již umožňuje spouštět programy v terminálu přímo, byť zatím jen v Xtermu. Proto časem bude tento modul odstraněn
2. Bohužel je zatím nutné vyhledat a ručně zadat AppID Steam hry
3. Morfeus je GUI nadstavba pro spouštěče her. Původně byl zamýšlen jako součást projektu Gorgona, ale nakonec byl přesunut do samostatného projektu. 

## 3. DOKUMENTACE
Součásti projektu Gorgona je i uživatelská (technická) dokumentace. Dokumentace je vedena ve formátu Markdown (*.md), s výjimkou uživatelských manuálu, které jsou plánovány ve formátu PDF (*.pdf) a možná v budoucnu též manuálových stránek, které mají svůj vlastní formát (*.man).

Pro soubory dokumentace je v projektu určena složka docs. Pouze soubory README jsou uloženy v kořenovém adresáři projektu. Dokumentační soubory jsou primárně psány v Českém jazyce (za názvem souboru je přípona _CS) a postupně překládaný do Anglického jazyka.

Seznam současných souborů uživatelské dokumentace:  
**README**       - Základní informace o projektu, Licence  
**docs/INSTALL** - Získání programu Gorgona, závislosti, kompilace a instalace   
**docs/STATUS**  - Verzování projektu, popis novinek v uvolněné verzi, seznam známých bugů a výhled do budoucna.  
**docs/AUTHORS** - Seznam autoru, kteří na projektu pracuji / pracovali  
**docs/COPYING** - Licence, seznam součástí projektu Gorgona od třetích stran a informace o jejich licenci  

#### Seznam souvisejících odkazů: 

  * [Muj blog](http://bfuplusplus.blogspot.cz/)
  * [Morfeus](https://github.com/PGSafarik/Morfeus)
  * [FoxGHI](https://github.com/PGSafarik/FoxGHI)
  * [Legendary](https://github.com/derrod/legendary)
  * [SteamDB](https://steamdb.info/)
  * [Parallel Realities (EN)](https://www.parallelrealities.co.uk/)
  * [Lutris (EN)](https://lutris.net/)
  * [GameHub (EN)](https://tkashkin.github.io/projects/gamehub/)

## 4. AUTOR A LICENCE
Copyright (C) 2015 - 2023 Pavel Šafařík <drakarax@seznam.cz> alias D.A.Tiger
všechna práva vyhrazena.

Tento soubor (a všechny soubory dokumentace) je součástí Gorgona.

Gorgona je svobodný software: můžete jej redistribuovat a/nebo upravovat za podmínek GNU General Public License, jak byla zveřejněna Free Software 
Foundation, buď verze 3 licence, nebo (dle vašeho uvážení) jakékoli 
pozdější verze.

Projekt Gorgona je distribuován v naději, že bude užitečný, ale BEZ JAKÉKOLI ZÁRUKY; dokonce bez předpokládané záruky PRODEJNOSTI nebo VHODNOSTI PRO 
KONKRÉTNÍ ÚČEL. Další podrobnosti najdete v GNU General Public License.

Spolu s Gorgonou byste měli obdržet kopii GNU General Public License. Pokud ne, podívejte se na <https://www.gnu.org/licenses/>.

  
---

    = KONEC =

