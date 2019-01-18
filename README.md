

**TODO**:
* do každého modulu vytvořit unit testy a vytvořit testovací režim pro kontrolu funkčnosti všech klíčových funkcí
* do řídícího panelu přidat loga partnerů (cm, bk, stc)
* přidat nastavení pro automatické spouštění jobu
* přidat obsluhu události kdy se nenastaví stav feeding při nakládání archu

* obnovení tiskového jobu při chybě
* přidat možnost sloučení všech reportovacích csv do jednoho velkého csv, k tomuto účelu bude třeba vytvořit samostatnou stránku kde bude možné nastavovat parametry exportu
* vytvořit filter, který bude filtrovat podle data slučování reportovacích csv
* upravit proceduru pro ukončování tisku, posílat do gisu jinou sekvenci kódů
* ošetřit aby se v hotfolderu pro gis mohlo nacházet pouze jedno pdf, vytvořit kontrolu když je již uvnitř pdf vyhodit hlášku že hotfolder není prázdný
* ošetřit že gremser vrací správně nastavené bity pro režim (MRB0 a MRB1)
* napojení do databáze omronu v gremser stroji
* na základě napojení do omronu, upravit počítadla archů, která budou vyčtena 
* přidat informace o jobu do reportovacího csv, vytvořit v něm informační hlavičku, tu pak načíst a zobrazit v okně se seznamem reportů v ovládacím panelu
* Vytvořit skript pro spouštění GIS serveru a bufferovací aplikace
* Vytvořit automatizované unit testy pro krytické funkce a vyladit jádro
* Ošetřit aby se v hotfolderu mohl nacházet pouze jeden job?
* Upravit řídící panel pro zobrazení pouze jednoho jobu připraveného k tisku
* Upravit vizuální vzhled ovládacích tlačítek v řídícím panelu
* Zpomalit tisk pokud se blíží fixně nastavená hodnota počtu vyhozených archů ve výhybce



**DONE**:

* ošetřit stav kdy se program zasekne v tiskovém stavu po delší dobu (time out pro vykonání operace) 
* Přidat ukončovací sekvenci/povel pro korektní ukončení machine_handler vlákna při ukočnování programu
* nastavit klávesovou zkratku pro zobrazení a skrytí rozhraní manuálního ovládání
* přidat tlačítka pro manuální spínání jednotlivých vstupů + manuální režim spínání
* upravit systém vyčítání hotfolderu a propojení s vrstvou view
* na základě nastaveného režimu kontrolovat připojení sítě do gisu/iij pc a pci pc
* upravit machine_handler pro jednodušší manuální ovládání funkce XBF a ENA Gremseru 
* přepracovat architekturu projektu: monolitic architecture -> modular architecture, vytvořit moduly hotfolder, multi_lang, com_tcp, info_struct, io_card, q_job, util, řídící modul controler, grafické rozhraní view, každému modulu vytvořit komunikační API
* v řídícím panelu upravit zobrazování informací o reportovacích csv (stav ukončení jobu, čas ukončení, počet kolků, počet archů, počet špatně vytištěných archů)
* pro úpravu zobrazování informací o reportovacích csv při procesu tisku upravit část job_info kde je nutné vyčítat další informace
* upravit podprogram pro vyčítání obsahu hotfolderu s reportovacími csv, přečíst hlavičku a zobrazit informace do řídícího panelu
* pro úpravu zobrazování informací o dostupném jobu k tisku upravit čtení funkci read_hotfolder a vyčítat dodatečné informace z csv (počet archů, počet kolků, čas vytvoření)
* v řádícím panelu dle grafické předlohy upravit zobrazování informací o dostupném jobu k tisku
* upravit funkci pro generování reportovacího csv, přidat hlavičku s informace o jobu
* upravit zobrazení počítadel v řídícím panelu
* přidat zobrazení stavu připojení k subsystémům stroje do řídícího panelu (quadient pc, gis pc, pci pc)
* combobox pro nastavení pracovního režimu stroje přesunout z karty nastavení->parametry tisku do kontrolního panelu
* v řídícím panelu dle grafické předlohy upravit zobrazování chybového stavu
* Odstranit ze zdrojových kódů nepotřebný kód z části rozhraní příkazové řádky
* Kontrola síťového spojení s počítačem quadient
* kontrola síťového spojení s počítačem pci
* Nastavení režimu stroje, nesmí být změněn při tisku
* Napojit konfiguraci do jádra
* Upravit kartu tiskové paramtry a přidat nastavení pro volby zdroje prokladového archu
* Na základě nastaveného režimu ošetřit posílání pdf a csv do subsystémů (upravit stav prepare a na konci kontrolu kamerového csv a generování zpětnovazebního csv pro quadient)
* Vytvořit program pro testování zátěže GISu
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



**Poznámky**

* Chyba v dopravníku Gremseru, pokud se zapne a vypne dopravníkový pás ručně z HMI, nelze jej ovládat z host-bk

* Pokud je při spuštění aplikace problém se síťovým spojením a v nastavení sítě je nastavena IP adresa 48.48.48.48 došlo k pokusu o nastavení neplatné IP adresy -> je nutné zkontrolovat configurační soubor na adrese "/home/stc/host-bk/bin/sys_cfg", pravděpodobně došlo k narušení struktury konfiguračního souboru.

* V případě přerušení elektrického proudu dojde k přepnutí počítačů v reku na sekundární zdroj z UPS, který slouží k bezpečnému ukončení počítačů bez ztráty dat, monitory ale nejsou na UPS napojeny a při výpadku elektrického proudu nelze s počítači pracovat -> k monitorům bude natažen prodlužovací kabel s rozdvojkou z reku, který bude napojen na UPS a bude napájet monitory při výpadku elektrického proudu.









**BUG report**:

* BUG 00005 při nastavování adres hotfolderů došlo k pádu prorgamu, pravděpodobně vlivem narušení paměti (program běžel bez výstupu do přákazové řádky).

* BUG: 00004 v reportovacím csv se nezobrazuje stav PASS
 - Je zobrazován pouze stav FAIL v případě, že prokladový arch neprošel kamerovou kontrolou, vypadá to, že tento stav nebyl do datové struktury job_info vůbec při kontrole zadán.
	-> chyba byla způsobena tím, že v datovém csv z IA quadient pro kolky byl ukončen znakem '\n' a pro prokladové archy nikoli. Chyba způsobila přeskočení naložení prokladového archu. 
	(vis. BUG 00001)

* BUG: 00003 chybné generování zpětnovazenbího csv - OPRAVENO
 - porovnávání zdá se funguje správně, ale jsou rozbyté indexy ve výstupním csv 
 -> při úpravě systému pro ukládání indexů archů jsem neupravil jejich načítání v čísti pro generování, nebyl čten řetězec obsahující index archu ale adresa na strukturu ve které byl uložen


* BUG: 00002 při generování zpětnovazebního csv dojde po nějaké době k narušení paměti - OPRAVENO
 - zatím byl zaznamenám pouze případ při generování zpětnovazebního csv pro prokladový arch s 0% úspěšností kamerové revize, zpětnovazební csv nebyl vůbec vytvořeno 
 -> bylo nalezeno několik chyb:
 	1 - v bkcore csv na prokladový arch nejsou narozdíl od kolkového na prvních dvou pozicích metadata o jobu, bylo potřeba vytvořit výjimku 
		uvnitř funkce core_machine_state_prepare, kde se tato metadata četou, čtení metadat je povolenou pouze v případě, že se jedná o job s příznakem 'k'
	2 - bkcore csv na prokladový arch obsahuje index tisknutého prokladového archu, který vždy začíná od 1, proto je (v případě tisku jednoho archu) přeten pouze jeden znak,
		to způsobuje, že csv_size = offset a vůbec nedojde ke vstupu do části pro tisk prokladových archů, to způsobuje problém i v případě jednoarchových jobu s kolky.
		Tento problém byl způsoben uvnitř funkce core_machine_state_read_csv_line kde se fyzicky čte obsah bkcore csv souboru, v této části byly porovnávány špatné proměnné,
		byly porovnávány proměnné offset < bkcore_csv_size, kde offset udává aktuální pozici konce daného řádku, tato hodnota je ale přičítána k 
		proměnné bkcore_csv_pos += (offset+1) (ukončovací
		znak '\n'). Tím tak došlo k tomu, že v případě čtení posledního řádku již nebyla splněna podmínka pro vstup do tiskového podprogramu a programový tok byl 
		přesměrován do stavu čekání na dokončení tisku


* BUG: 00001 neúspěné rušení probíhajícího jobu - OPRAVENO
 -  log vypisuje hlášku: "Přerušení jobu stc-17898 zkončilo chybou! Obslužné vlákno se nachází ve stavu: 2 - Ready", nakonec dojde k narušení paměti. Při rušení tisku se podle
	výpisu logu obslužné vlákno dostalo do stavu read_csv_line ze kterého mělo jedinou možnost přejít do stavu print_break, v této funkci tedy musí být chyba
	-> bylo nalezeno několik chyb:
	1 - log vypisoval hlášku o chybě při ukončování jobu, protože se vlákno se může nacházet ve dvou časovacích režimech - realtime a standby, v realtime režimu je 
		frekvence volání smyčky v jednotkách milisekund a v režimu standby se jedná o stovky milisekund. Uvnitř funkce, která se stará o ukončení tisku byla nastavena
		prodleva pro nastavení ukončovacího příznaku print_cancel_req=true pouze v na 10 cyklů v realtime režimu, tisk lze ale přerušit pouze ze stavu read_csv_line nebo
		wait_next a do těchto stavů se dostávají vždy nejdříve po x vteřinách (po dokončení naložení archu a nebo po dokončení jobu a čekání na další data), proto 
		byla časová prodleva zrušena a v případě neúspěšného ukončení a zablokování rozhraní je nutné resetovat cyklus, tato řídící proměnná je vynulována ve stavu print_finish
	2 - uvnitř funkce machine_state_print_break byl volán podprogram pro analýzu kamerového csv i v případě, že se stroj nenacházel inspekčním režimu (žádné csv z kamery neexistuje),
		chyba byla tedy vyvolána i v režimu PRINT nebo SETUP, ošetřeno podmínkou nastaveného režimu.
	3 - chyba způsobující pád programu vlivem neoprávněného přístupu do paměti byla způsobena uvnitř funkce core_analyze_csv, která se volá v případě, že je job ukončen
		před jeho dokončením, tedy nejedná se o job s příznakem 'e'. Díky tomu se tento problém neprojevil při korektním ukončení jobu. Při nakládání archů jsou průběžně vytvářeny
		záznamy o naložených arších, ale pokud se tisk stopne předčasně, struktura neobsahuje záznamy o všech arších, které bkcore csv obsahuje a nakonci nemohou být ohodnoceny
		při analýze kamerového csv a v tomto bodě dojde k narušení paměti, protože se struktura vrátí hodnotu NULL a zde není ošetřena tato eventualita, možnost je buď před začátkem 
		nakládání archů vygenerovat celou strukturu obsahující informace o arších a nebo při této eventualitě složitě chybějící struktury vygenerovat. První možnost je nejjednodušší,
		protože díky tomu lze zrobustnit funkci read_csv_line ve které probíhá posílání jednotlivých archů z nakladače (vis. BUG 00002)
	4 - bkcore csv obsahuje u kolkového jobu na konci souboru znak odřádkování '\n', ale v případě prokladového archu již nikoli, z tohoto důvodu nevytváří modifikovaný kód pro prokladové
		archy záznamy ve strukturě s informace jobu a tím nejsou nakládány na dopravník archy a zároveň je na konci přistupováno na NULL který končí pádem programu. Přidán na konec
		kódu pro generování záznamů do struktury s informace o jobu podmínku, pokud se před koncem souboru nacházely nějaké číselné znaky vytvoř záznam.


























