

**TODO**
* vytvořit funkcionalitu na ovládání mechanismu na nastřelovací proužky
* přidat do karty nastavení tiskových parametrů možnost nastavit čas nastřelování oddělovacího proužku ovlivňující jeho délku 
* nafakeovat tisk prokladu na jedno tlačítko
* přidat informaci na hlavní ovládací panel o nastaveném zdroji pro prokladový arch
* přidat do nastavení tiskových parametrů, při x-tém vyloženém archu nastřelit oddělovací proužek
* ošetřit aby se job korektně ukončil a soubory se vymazaly z hotfolderu
* zpomalit tisk pokud se náhrady blíží nastavení hodnotě (500) ve vykladači

* vytvořit dokumentaci zdrojových kódů
* (ošetřit že gremser vrací správně nastavené bity pro režim (MRB0 a MRB1) napojení do databáze omronu v gremser stroji)



**DONE**

* přidat do nastavení tiskových parametrů zpomalení nakládání -> nastavení rozestupů archů na pásu
* vytvořit kartu pro přehled stroje kde budou zobrazeny podrobnosti o stavu stroje (počítadla, režim, stav nakladače, vykladače, ...)
* vytvořit funkcionalitu, která bude zapínat větrák na separaci prokladových archů
* zrychlit ukončování jobu
* přidat do ovládacího panelu tlačítko na vyprázdnění vykladače (vynulování počítadla na vyložené archy) (implementováno ale netestováno)
* rozšířit statistiku dobrých a špatných archů na denní produkci a za danou časovou dobu plus počet naložených, vytisknutých a vyhozených archů za den a za danou časovou dobu
* vytvořit modul pro správu statistiky tisku
* přidat dodatečné informace do logu
* nastavit když při zapnuté ena je nakladač nebo vykladač ve stavu připraven, pak je nutné upozornit obsluhu
* upravit maják stavu nakladače a vykladače aby blikal v případě že očekává jiný stav než jaký se na něm nachází
* nastavit program aby při zácpě jakéhokoli druhu vyčistil pás, a vygeneroval náhrady do quadientu
* vytvořit filter, který bude filtrovat podle data slučování reportovacích csv
* vytvořit statistiku špatných a dobrých archů
* zprovoznit vizualizaci stroje v hlavním panelu
* přidat do nastavení tiskových parametrů nastavení zpomalení nakládání (časová prodleva naložení ovlivňující vzdálenost jednotlivých archů)
* do každého modulu vytvořit unit testy a vytvořit testovací režim pro kontrolu funkčnosti všech klíčových funkcí
* upravit obsah reportovacího csv a sjednotit popisky jobů k tisku a vytisknutých jobů
* vytvořit skript který vymaže soubory logu a reportu starší než 30 dnů
* při ukončování programu pokud se program nachází ve stavu tisku, pak se musí nejprve tisk korektně ukončit aby se mohl ukočit program
* přidat informační popisky do hlavního panelu
* upravit ovládací tlačítka hlavního panelu - talčítko vymazat job sloučit s tlačítkem pro zrušení tisknutého jobu a naložit jeden arch automaticky spustí tisk jobu pokud neběží
* automatické připojení gisu po jeho odpojení s chybou
* přidat tlačítko na korektní ukončení programu
* přidat na hlavní panel informaci o počtu náhrad v daném podjobu
* přidat na hlavní panel časový odhad pro tisk daného jobu
* přida položku job id a sjednotit názvy ve výpisu jobů
* přidat tlačítko naložit jeden arch a nastavit pauzu
* při stavu low print quality nepřerušovat tisk, ale pouze pozastavit a počkat na potvrzení, že byl problém vyřešen
* až Petr Malý upraví bkcore csv upravit vyčítání a ukládání hodnot 
* do reportovacího csv doplnit celkový počet naložených archů a celkový počet vyložených archů
* při stavu sheet messing z nakladače po přechodu do stavu feeder_error po x vteřinách vypnout nakladač -> přejít do pauzy pro opětovné spuštění jobu
* opravit výpis reportovacích csv
* ošetřit aby se v hotfolderu pro gis mohlo nacházet pouze jedno pdf, vytvořit kontrolu když je již uvnitř pdf vyhodit hlášku že hotfolder není prázdný
* upravit kontrolu csv z kamery a quadientu tak aby bylo možné přistupovat k jednotlivým řádků a buňkám v řádku a porovnávat jednotlivé parametry archu
* při signálu z nakladače missing sheet, nebo double sheet, nebo feeder jam přejít do stavu feeder_error ve kterém se čeká na odstranění problému a na opětovné zahájení nakládání
* pokud se nacházím nějakou dobu v pauze, pak vypnout ENA a potom opět spustit ENA když se chci navrátit do procesu tisku
* když se tisk pozastaví po chvíli se vyplne nakladač, aby mohl tisk poračovat je nutné aby se nakladač nejprvě opět zapnul, v případě, že se nejprve dá pokračovat v tisku a pak zapne nakladač je nutné nastavit čekací dobu aktivace nakladače -> urgentní
* prozkoumat počítání archu při nastavení stavu pause a obnovení z pause
* při přerušení tisku nechat dojet všechny archy ze stroje (a nebo to byl gerhard kdo vypnul dopravník?)
* když se zasekne tisk, pak po x vteřinách přejít do pauzy 
* upravit proceduru pro ukončování tisku, posílat do gisu jinou sekvenci kódů
* přidat upozornění na chybu nakladače jako warning
* přidat informace o jobu do reportovacího csv, vytvořit v něm informační hlavičku, tu pak načíst a zobrazit v okně se seznamem reportů v ovládacím panelu
* vytvořit opatření aby se do hotfolderu gisu nedalo poslat pdf v případě, že hotfolder není prázdný
* Nastavit, aby si program při spuštění vyčistil hotfolder, při spuštění se v něm nesmějí nacházet žádné joby (mohou být již částečně vytištěné), v průběhu tisku pak již němůže nastat, že by se job nevymazal
* Nastavit signál PRN_rdy hned po spuštění programu na true a do řídícího panelu vytáhnout informativní signál o jeho stavu
* Do řídícího panelu přidat tlačtko na vyčištění hotfolderu
* přidat možnost sloučení všech reportovacích csv do jednoho velkého csv, k tomuto účelu bude třeba vytvořit samostatnou stránku kde bude možné nastavovat parametry exportu
* přidat obsluhu události kdy se nenastaví stav feeding při nakládání archu
* do řídícího panelu přidat loga partnerů (km, bk, stc)
* upravit reset chyby nastavení E-STOP aby se vyresetovala ihned po tom co stroj přejde zpět do provozního režimu
* zobrazit stavy nakladače a vykladače na hlavní obrazovku
* v režimu printing main a printing companion dát časový interval ve kterém stroj dá odpověď FEEDING pokud nedá do dané doby odpověď, přejít do chyby
* při inicializaci stroje přidat časový interval ve kterém se může nastavit a pokud tento interval překročí, přejít do chyby
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

* stav nakladače sheet missing, neshoda počítadel, co nastane když se korektně nepodá -> vyřešeno

* signály z reject binu nepřicházeji a nebo je v programu chyba -> zeptat se Gremseru na stav -> nebylo naprogramováno

* Chyba v dopravníku Gremseru, pokud se zapne a vypne dopravníkový pás ručně z HMI, nelze jej ovládat z host-bk -> vyřešeno

* Pokud je při spuštění aplikace problém se síťovým spojením a v nastavení sítě je nastavena IP adresa 48.48.48.48 došlo k pokusu o nastavení neplatné IP adresy -> je nutné zkontrolovat configurační soubor na adrese "/home/stc/host-bk/bin/sys_cfg", pravděpodobně došlo k narušení struktury konfiguračního souboru.

* V případě přerušení elektrického proudu dojde k přepnutí počítačů v reku na sekundární zdroj z UPS, který slouží k bezpečnému ukončení počítačů bez ztráty dat, monitory ale nejsou na UPS napojeny a při výpadku elektrického proudu nelze s počítači pracovat -> k monitorům bude natažen prodlužovací kabel s rozdvojkou z reku, který bude napojen na UPS a bude napájet monitory při výpadku elektrického proudu.









**BUG report**:

* BUG 00011 v některých případech je přeskočen tisk prokladových archů z hlavního nakladače
  - podle výpislu logu je korektně inicializován stroj a čten řádek z přečteného csv souboru, ale následně je přeskočen krok tisku a přejde se rovnou do dokončování daného jobu,
	to může být způsobeno chybným načtením obsahu csv souboru

* BUG 00010 při tisku dojde k narušení paměti a pádu softwaru
  - chyba je velice ojedinělá a nastane pouze při procesu automatického tisku. Následkem narušení paměti je pád programu. Podle výpisu z logu dojde k narušení paměti ve stavu read_hotfolder. 
	Chyba může souvisest s BUG 00011. Příčinou chyby by mohl být proces čtení hotfolderu, kdy jsou soubory jobu kopírovány do hotfolderu, ale kopírování samotné není ještě dokončeno (při vzorkovací 	
	frekvenci 1KHz, každou 1ms se provede jeden cyklus) je pravděpodobné, že čtení hotfolderu trefí do stavu kdy je modifikován souborový systém, ale data ještě nejsou nakopírována.

* BUG 00009 na konci jobu systém vyhodí chybu nesoulad pořadových indexů jobu - 
  - chyba vzniká pouze při ukončování jobu ve stavu kdy se očekává ukončovací csv s příznakem 'e'. Tato chyba se nastaví ve stavu read_hotfolder kde se kontroluje zda aktuální soubory 
	tisknutého jobu mají správnou hodnotu inkrementu. Požadovaný soubor se správným inkrementem a příznakem 'e' se v hotfolderu nachází, ale obsah hotfolderu nebyl aktualizován a 
	pořadový index je porovnáván se soubory z předchozíjo jobu.
	-> Chyba byla způsobena úpravou funkce controler_machine_state_read_hotfolder, aby bylo možné manuálně posálat archy. Funkce je vykonávána v plném taktu stavového automatu programu (1ms),
	 	ale čtení hotfolderu je v každém stavu pouze v intervalu 1000ms pomocí funkce c_timer_delay, problém tak byl způsoben tím že se tak výpis hotfolderu nestihl znova načíst, protože od
		posledního čtení hotfolderu neuplynulo více jak 1000ms, ale při tisku archů je tato doba mnohem delší a tak se při následujícím průchodu nejprve aktualizuje obsah hotfoleru a teprve 
		poté se rozhodne co se bude dít (to by mohlo dělat problémy při mírném zpoždění). Problém jsem odstranil tak, že se ve stavu MACHINE_STATE_NEXT hotfolder vyčítá v plném taktu 
		stavového automatu

* BUG 00008 zamrznutí grafického rozhraní při pauze a pokračování v tisku, jádro běželo grafické rozhraní neodpovídalo - OPRAVENO
  - při opětovném obnovení tiskového jobu ze stavu pauze jádro reagovalo správně, ale grafické rozhraní přestalo reagovat
	-> Chyba byla způsobena logckou chybou podmínky	v cyklu který kontroloval zda bylo obnovení tisku v dané době úspěšné, došlo tak k zaciklení a zamrznutí hlavního vlákna

* BUG 00007 parametry nastavení jsou uloženy do konfiguračního souboru, ale při opětovném spuštění nejsou načteny, jsou načteny předchozí hodnoty - OPREAVENO 
 - To se stává pouze u adresy datového hotfolder pro quadient, hotfolder pro gis a hotfolder pro reportovací csv. Bližším zkoumáním bylo zjištěno, že nastává pouze problém při 
	nastavování adresy pro datový hotfolder pro quadient a hotfolder pro reportovací csv. Při nastavení nové adresy pro reportovací csv, dojde v konfiguračním souboru k přepsání 
	adresy datového hotfolderu pro quadient a adresa pro reportovací hotfolder je nezměněná, dochází tedy k záměně cíle pro ukládání konfigurace do souboru
	-> chyba byla způsobena chybným identifikátorem pro ukládání konfigurace do souboru

* BUG 00006 nelze aktualizovat výpis jobů v adresáři, ani automaticky ani manuálně - OPRAVENO
	-> chyba byla způsobena chybným relačním znaménkem nerovnosti při porovnání času a resetování timeru po uplynutí dané doby

* BUG 00005 při nastavování adres hotfolderů došlo k pádu prorgamu, pravděpodobně vlivem narušení paměti (program běžel bez výstupu do přákazové řádky) - NEAKTIVNÍ
	chyba se objevila pouze jednou a od té doby již při nastavování adres nedošlo k potížím, problém mohl souviset s BUG 00007

* BUG: 00004 v reportovacím csv se nezobrazuje stav PASS - OPRAVENO
 - Je zobrazován pouze stav FAIL v případě, že prokladový arch neprošel kamerovou kontrolou, vypadá to, že tento stav nebyl do datové struktury job_info vůbec při kontrole zadán.
	-> chyba byla způsobena tím, že v datovém csv z IA quadient pro kolky byl ukončen znakem '\n' a pro prokladové archy nikoli. Chyba způsobila přeskočení naložení prokladového archu. 
	(vis. BUG 00001)

* BUG: 00003 chybné generování zpětnovazenbího csv - OPRAVENO
 - porovnávání zdá se funguje správně, ale jsou rozbyté indexy ve výstupním csv 
 -> při úpravě systému pro ukládání indexů archů jsem neupravil jejich načítání v části pro generování, nebyl čten řetězec obsahující index archu ale adresa na strukturu ve které byl uložen


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


























