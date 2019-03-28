Readme CS 

Popis programu
========================
Program Gorgona je určený jako správce a spouštěč her pod OS Linux. Projekt započal svou existenci pod jménem FXGameLauncher jako C++ náhrada za aplikaci JGameLauncher napsaný v Javě. Hlavními vadami tohoto programu byla nutnost psát pro mnoho her shellove spouštěče, nebo nemožnost třídit hry podle určitých kriterii (např. podle žánru).  

Gorgona v teto chvíli víceméně své původní cíle splnila a v některých ohledech svůj původní vzor předběhla. Obsahuje interpreter jazyka Lua a několik vložených funkcí, které umožňují psát univerzální spouštěče her. V tuto chvíli umožňuje pouze ruční vložení hry do seznamu, a zadáni potřebných dat. Gorgona potom zařadí hru do odpovídající sekce, odkud ji lze poté také spouštět. Seznam her je možno zobrazit ve dvou variantách (ikony a podrobný výpis). K přejmenování projektu došlo ve chvíli, kdy padlo rozhodnutí projekt dál vyvíjet a vylepšovat.

Počítejte s faktem, ze tato aplikace je v současné době v aktivním vývoji a mohou se objevit nečekané chyby (a to i presto, ze jsem momentálně žádné zásadní nezaznamenal) či nečekané chování. Mějte na paměti, ze během času se mohou změnit funkce, vzhled či ovládání projektu, tak jak bude dal vyvíjen. Některé funkce naopak mohou zaniknout.

Balíček projektu obsahuje projektové soubory IDE Code::Blocks 

Závislosti
========================
Foxtoolkit 1.7.64 (http://www.fox-toolkit.org)
TinyXml           (https://sourceforge.net/projects/tinyxml)
Lua 5.1           (https://www.lua.org/)  
cmake
g++

UPOZORNĚNÍ: Knihovna foxlib musí být v době kompilace nainstalována na vašem systému. Verze 1.7 je vývojová verze, je proto možné, ze ji nebudete mít v systému a nebude ani dostupná v repositáři, proto ji budete muset doinstalovat ručně. Více informací jak to udělat najdete na stránkách foxtoolkitu. Následující řádky se týkají případných úprav CMakeLists.

Mate-li knihovnu nainstalovanou na jiném, než standardním umístění, je nutno tuto skutečnost zanést v souboru CMakeLists.txt v adresáři projektu. Vyhledejte a změňte řádky nastavující cestu ke knihovně:
    INCLUDE_DIRECTORIES( "/Cesta/vasi/instalaci/foxlib/include" )   #Foxlib headers
    LINK_DIRECTORIES( "/Cesta/vasi/instalaci/foxlib/lib" )          #Foxlib library
Tyto řádky jsou ve výchozím stavu zakomentovány, po úpravě tedy ještě odstraňte počáteční znaky '#'.     

Kompilace a instalace
========================
Archiv s projektem rozbalte a přejděte do projektového adresáře. Následně spusťte příkazy pro kompilaci a instalaci projektu:
    cd build
    cmake .. && make && make install
Pokud vše proběhne v pořádku, výsledný binární soubor se nainstaluje do /opt-gorgona, a budou tam zkopírovány i soubory spouštěcích modulů. 

Direktivu 'make install' lze samozřejmě vynechat, soubor gorgona zůstane pak pouze v adresáři build. Toho lze využít, pokud chcete převzít kontrolu nad instalačním procesem (například vytvořit distribuční balík, apod.)

Kompilaci projektu lze provést též z Code::Blocks. Projekt je nastaven tak, aby výsledný binární soubor, včetně objektových souborů byly vytvářeny v adresáři build.

Kompletní vyčištění adresáře build provedete příkazem
    rm -r * 
(předpokládá se, že se aktuálně nacházíte v adresáři build)

Známé chyby
========================
- Lua spouštěče nevrací správně návratovou hodnotu po skončení jím spuštěné hry

Plány do budoucna
========================
(netříděno)
- Spouštění her pomocí parametrů z příkazové řádky
- Náhodný výběr (i z commandline)
- Nastavitelné pokročilé parametry procesu( mód procesoru, env.variables, priority, I/O priority, atd...)
- Přepracování Lua API
- Lua skripty při spouštění Gorgony
- detekce instalovaných her
- spolupráce s vestavěnými herními komponenty (např. KDE Games)
- Možnost skenování repositářů a instalace herních balíčku
- Sekce nastroje ( kooperující programy či skripty řešící například přístup do sítě Hamachi, atd...)






