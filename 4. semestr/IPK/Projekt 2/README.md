# Dokumentace Projektu: PacketSniffer

## Obsah
1. [Úvod](#úvod)
2. [Teoretický základ](#teoretický-základ)
3. [Testování](#testování)
4. [Diagram](#diagram)
5. [Popis Implementace](#popis-implementace)
6. [Bibliografie](#bibliografie)


## Úvod
Tato dokumentace popisuje implementaci a použití PacketSnifferu - nástroje pro zachytávání síťových paketů v reálném čase. Aplikace je schopna analyzovat provoz přes různé síťové protokoly jako TCP, UDP, ARP, ICMPv4, ICMPv6, NDP, MLD a IGMP.

## Teoretický základ
PacketSniffer je nástroj, který umožňuje síťovým administrátorům, bezpečnostním specialistům a vývojářům analyzovat a monitorovat síťový provoz. Využívá se pro diagnostiku síťových problémů, audit bezpečnosti a vzdělávací účely.

### Síťové monitorování
Síťové monitorování je proces sledování a analýzy síťového provozu s cílem identifikovat a řešit problémy, zlepšit výkon nebo zjistit neautorizovanou aktivitu. Monitorování provozu je nezbytné pro udržení plynulého a efektivního chodu síťové infrastruktury a pro ochranu proti hrozbám a útokům.

#### Protokoly a architektura sítě
Základem pro chápání a analýzu síťového provozu jsou znalosti síťových protokolů a principů fungování sítí. Mezi klíčové protokoly patří:

- **Ethernet:** Protokol linkové vrstvy, který je základem pro většinu LAN sítí.
- **IP (Internet Protocol):** Základní protokol síťové vrstvy, který umožňuje směrování dat mezi různými zařízeními a sítěmi.
- **TCP a UDP (Transport Control Protocol a User Datagram Protocol):** Dva hlavní transportní protokoly používané pro odesílání dat mezi aplikacemi v síti.

#### Analýza paketů
Analýza paketů je proces, při kterém jsou jednotlivé pakety, neboli základní přenosové jednotky v síti, rozklíčovány a zkoumány pro získání detailních informací o síťovém provozu. Tento proces zahrnuje:

- **Rozklíčování hlaviček:** Každý paket obsahuje hlavičky, které poskytují metadata o paketu, včetně zdrojových a cílových adres, portů a dalších kontrolních informací.
- **Rekonstrukce toku:** Pro spojové protokoly jako TCP je důležitá schopnost rekonstruovat tok dat pro analýzu spojení.

### Filtrace a selekce dat
Vzhledem k obrovskému množství dat procházejících síťovým rozhraním je nezbytné mít schopnost efektivně filtrovat a selektovat data podle určitých kritérií. PacketSniffer toto umožňuje pomocí dynamických filtrů, které mohou být nastaveny pro zachycení pouze relevantních dat podle:

- **Protokolu:** Filtrace dat podle síťového protokolu, jako je TCP, UDP, ARP atd.
- **Adresy:** Možnost selekce dat podle zdrojové nebo cílové IP adresy.
- **Portu:** Filtrace podle specifických zdrojových nebo cílových portů.


## Testování
Testování probíhalo jak na WSL Ubuntu 20.04, tak na Windows 11 v PowerShellu. V poslední fázi i na virtuálním stroji.

### Co bylo testováno

- Vypsání jednotlivých interfaceu
- Vypsání formátu paketu
- Vypsání jednotlivých typů protokolu zvlášť
- Kombinace více argumentů, např.: počet zobrazených paketů, s konkrétním protokolem a portem
- Chybové zadání nějaké hodnoty argumentů

### Proč to bylo testováno

Testoval jsem tyhle základní funkce, které byly v zadání, aby jsem ověřil základní funkcionalitu, která byla zadaná.

### Jak to bylo testováno

Jak jsem začínal, tak původně jsem to testoval ručně, protože mi chvíli trvalo než jsem začal něco vypisovat (nějaké pakety). Poté, co zobrazení paketu vypadalo dobře a fungovali mi základní protokoly tcp a udp, argumenty -p a -n, tak jsem přešel k základnímu scriptu, který byl již na discordu a kterého nejsem autor. Script jsem pouze poupravil na můj projekt. Poté co mi prošly základní testy, které tenhle script obsahoval jsem ještě musel otestovat MLD paket, který v testech nebyl. Otestoval jsem to tak, že v jednom terminálu jsem si spustil svůj python script testMLD, který mi poslal MLD paket a v druhém termínalu jsem měl spuštěný sniffer. Tohle jsem otestoval z důvodu, že MLD pakety se v běžném síťovém provozu vyskytují méně často a je obtížné je zachytit.


#### Výstupy z testů

[Část výstupu z testu](/img/vystup1.png)

[Test na MLD paket](/img/mldtest.png)

## Diagram

[Diagram](/img/Diagram.png)

## Popis Implementace

Implementace snifferu je rozdělena do několika modulů a tříd, které umožňují zachytávání a analýzu síťového provozu.

### Modul `CommandLineParser`
Modul `CommandLineParser` obsahuje logiku pro parsování argumentů z příkazové řádky. Pomocí třídy `CommandLineParser` je možné interpretovat různé příkazové parametry, jako jsou porty, typy protokolů a počet zachytávaných paketů.

#### `CommandLineParser.cs`
- Třída `CommandLineParser` implementuje rozhraní `ICommandLineParser` a zajišťuje rozpoznání a zpracování argumentů, které uživatel zadá při spuštění snifferu. Podporuje různé přepínače pro specifikaci síťových rozhraní, portů a typů protokolů.

### Modul `Filter`
Modul `Filter` je odpovědný za vytváření filtrů, které omezují zachytávaný síťový provoz na základě uživatelem definovaných kritérií.

#### `FilterBuilder.cs`
- `FilterBuilder` implementuje rozhraní `IFilterBuilder` a poskytuje metody pro konstrukci textových řetězců, které reprezentují filtry použitelné v nástroji `pcap` pro omezení zachytávaného provozu na požadované protokoly a porty.

### Modul `PacketCapture`
Tento modul je jádrem snifferu a obsahuje logiku pro zachytávání paketů na síťovém rozhraní.

#### `PacketCapture.cs`
- Třída `PacketCapture` implementuje rozhraní `IPacketCapture` a využívá knihovnu `SharpPcap` pro přímý přístup k síťovým rozhraním a zachytávání paketů, které procházejí těmito rozhraními. Poskytuje metody pro spuštění a zastavení zachytávání, jakož i pro výpis dostupných síťových rozhraní.

### Modul `CommandLineOptions`
Tento modul obsahuje definici nastavení a parametrů, které jsou získány z příkazové řádky.

#### `CommandLineOptions.cs`
- Třída `CommandLineOptions` slouží jako datová struktura, která uchovává nastavení extrahovaná z argumentů příkazové řádky. Zahrnuje nastavení pro síťové rozhraní, porty, typy protokolů a další.

### Modul `PacketSniffer`
Modul `PacketSniffer` koordinuje celkový proces zachytávání a analýzy paketů.

#### `PacketSniffer.cs`
- Třída `PacketSniffer` slouží jako řídící centrum snifferu. Koordinuje procesy nastavení filtrů, spouštění a zastavování zachytávání a zpracování příchozích paketů. Tato třída také zajišťuje správné zacházení s argumenty příkazové řádky a uživatelem definovanými parametry pro běh snifferu.

### Hlavní vstupní bod `Program`
Vstupní bod aplikace, který zahajuje běh snifferu.

#### `Program.cs`
- Třída `Program` obsahuje statickou metodu `Main`, která je vstupním bodem aplikace. Spouští hlavní smyčku pro zachytávání paketů.


## Bibliografie

Tato sekce obsahuje odkazy na zdroje, které byly použity při tvorbě této dokumentace a tohohle projektu.

- Přispěvatelé Wikipedie. (bez data). Analyzátor paketů. Na Wikipedii, česká encyklopedie. Získáno z https://cs.wikipedia.org/wiki/Analyz%C3%A1tor_paket%C5%AF
- Dokumentace SharpPcap. (bez data). Na SourceForge. Získáno z https://sharppcap.sourceforge.net/htmldocs/SharpPcap/index.html



