

TODO:
* Vytvořit skript pro spouštění GIS serveru a bufferovací aplikace
* Nastavení režimu stroje, nesmí být změněn při tisku
* Na základě nastaveného režimu ošetřit posílání pdf a csv do subsystémů
* Upravit kartu tiskové paramtry a přidat nastavení pro volby zdroje prokladového archu
* Vytvořit automatizované unit testy pro krytické funkce a vyladit jádro
* Ošetřit aby se v hotfolderu mohl nacházet pouze jeden job
* Upravit řídící panel pro zobrazení pouze jednoho jobu připraveného k tisku
* Upravit vizuální vzhled ovládacích tlačítek v řídícím panelu
* Napojit konfiguraci do jádra
* Odstranit ze zdrojových kódů nepotřebný kód z části rozhraní příkazové řádky
* Zpomalit tisk pokus se blíží fixně nastavená hodnota počtu vyhozených archů ve výhybce
* Kontrola síťového spojení s počítačem quadient
* kontrola síťového spojení s počítačem pci



DONE:

* Upravit kartu řídícího rozhraní pro potvrzení nabraní prokladového archu z hlavního nakladače
* Zrychlit odezvu resetu
* Zobrazit na řídícím panelu hodnotu počítadel
* Přidat nastavitelnou možnost výběru archu pro prokladový list buď ze separator sheet feeder nebo s main feeder
* Přidat nastavitelnou možnost pro potvrzení nabrání archu na prokladový list z main feeder (možnost ručního vložení správného archu)
* Přidán sledovací prvek pro úniky paměti, log je vždy ukládán do souboru v adresáři "$HOME/host-bk/bin/valgrind_log.txt"
* Doplnit infromace o jobu - celkový počet kolků, celkový počet archů v Jobu
* Vytvořit kartu pro nastavení parametrů tisku
* Vytvořit kartu s hlavním řídícím panelem
* Přidat do řídícího panelu seznam reportovacích csv, které jsou při kliknutí zobrazeny v interním editoru
* Vytvořit kartu pro nastavení hotfolderů
* Vytvořit dialog pro výběr adresářové cesty pro nastavení cesty k hotfolderům
* Vytvořit kartu pro nastavení sítě
* Kontrola připojení GISového počítače, hlášení s chybou
* Vytvořit kartu pro nastavení jazka
* Zpomalit tisk pokud se tisk blíží 2500 archů ve vykladači (konfigurovatelná proměnná)
* Vytvořit opatření při x chybně detekovaných arších po sobě se objěví varovná hláška a po y chybně detekovaných arších se tisk stopne
* Pojmenovat chybové stavy v jádře
* Vytvořit počítadla archů
* Upravit rušení tiskového jobu (poslat zpětnovazební csv s přiznakem 'e', analyzovat csv z kamery, vytvořit reportovací csv, vyčistit hotforldr)
* Na jakékoliv přerušení tisku reagovat stejnou sekvencí ukončení
* Vytvořit vícejazyčný překlad pro popis chyb
* Implementovat logování průběhu tisku, výsledkem bude inspekční csv soubor, který bude obsahovat veškeré informace z jednotlivých zpětnovazebních csv posílaných do quadientu
* Implementovat logování procesů v jádře a ukládat jej do souboru
* Implementovat detekci chyby definovanou blikáním vstupu frekcencí ~2Hz (Tab insert, companion sheet feeder feedback input)
* Vytvořit kartu pro vizualizaci stavů vstupů a výstupů


