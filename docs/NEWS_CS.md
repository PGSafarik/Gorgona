# Novinky
    - 19/03/2023 -

---
    
#### 13/03/2023 Aktualizace 2 - 1.6.0.0
* Realizace vnitrniho modulu PERSEUS a prepracovani mechanismu spousteni novych procesu a jejich zpravy ve spolupraci s tridou Gorgona
* Prvni zmeny ve vnitrni mechanice API pro jazyka Lua  
* Priprava na implementaci skriptovanych modulu 2.0, ktere budou moci poskytovat vice sluzeb; a zaroven nad nimi Gorgona prevezme 
vetsi kontrolu. 
* Inicializacni procedura aplikace (vcetne Lua interpreteru a skriptovanych modulu) je presunut do tridy Gorgona, jeste pred vytvorenim 
primarniho okna.
* Pridan modul pro aplikaci Morfeus
* Pridana moznost spoustet hry pres okno terminalu (zatim pouze xterm)
* Upravy v zobrazovani seznamu her
* Hru v seznam je mozno nyni spustit dvouklikem na danou polozku
* Gorgona registruje pocet spusteni a datum posledniho spusteni dane hry
* V rezimu zobrazeni "detaily" se krome ikony, nazvu a poctu spusteni, zobrazuje tez datum posledniho spusteni a zanr hry
* Úprava a revieze dodávané dokumentace. Soubory, u kterých to má smysl, mají i v české verzi (README_CS, NEWS_CS a INSTALL_CS)
* Odebran stautus projektu "Pozastaveno"
* Drobné úpravy

#### 11/01/2023 Aktualizace 1 - 1.5.0.1
* Vychozi verze foxlib 1.7.81
* Top-level okna Gorgony jsou nyni vytvařena a spravována FoxGHI knihovnou. Vychozi verze je 0.4.1  
* Nastrojové panely jsou nahrazeny FoxGHI Header Boxy
* Upraven layout top-level oken v návaznosti na změnu správy top-level oken
* Vyhledávání presunuto ze seznamu polozek do Header Baru 
* Přidán modul pro podporu spouštění her pomoci Steam clienta (zatím je nutno manulane vyhledat Steam app id a zadat do pole přikazu)
* Přidán modul pro podporu spouštění her nainstalovaných pomoci nástroje Legendary (Epic games a GOG.com. I zde je nutno manuálně zadat id hry)
* Jednoduchý nástin validace položek. Zatím kontroluje (a ne zcela správně) validnost podle existence spustitelneho souboru u nativnich polozek.
  Položky ktere jsou vzhodnoceny jako nevalidní, jsou vypsány do stdout.
* XML tagy reprezentující jednotlivé položky seznamu her jsou nyní seskupeny do xml tagu "Library". Tato zmena je momentálně nekompatibilní s předchozími 
  verzemi a původní datový xml soubor nebude správně načten. Je potreba jej ručně opravit, nebo založit nový. 
* Nekolik drobnych oprav a zmen
    

---

    = Konec =
