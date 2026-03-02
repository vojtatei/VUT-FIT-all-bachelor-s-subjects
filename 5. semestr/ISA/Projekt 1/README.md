# README

**Autor:** Vojtěch Teichmann  
**Login:** xteich02  
**Datum vytvoření:** 14. listopadu 2024  

## Popis programu
Tento program je implementací klienta pro přístup k e-mailové schránce prostřednictvím protokolu IMAP s podporou šifrování pomocí TLS. Program umožňuje připojení k IMAP serveru, autentizaci uživatele, stahování e-mailů a ukládání stažených zpráv do lokálního adresáře. Program je určen pro konzolové prostředí a je napsán v jazyce C++ s využitím knihovny OpenSSL pro šifrování.  

## Rozšíření a omezení
- Program podporuje volitelné šifrované připojení přes TLS.
- Program umí stáhnout pouze hlavičky e-mailů nebo pouze nové (nepřečtené) zprávy, což snižuje množství stahovaných dat.
- Program neobsahuje žádné rozšíření ani omezení

## Příklad spuštění
1. Stažení e-mailů z nešifrovaného připojení:
   ```bash
   ./imapcl eva.fit.vutbr.cz -p 143 -a auth_file -o maildir

2. Stažení pouze nových zpráv přes šifrované připojení TLS:
    ```bash
    ./imapcl eva.fit.vutbr.cz -T -p 993 -n -a auth_file -o maildir

3. Stažení pouze hlaviček ze složky Trash přes TLS:
    ```bash
    ./imapcl eva.fit.vutbr.cz -T -p 993 -b Trash -h -a auth_file -o maildir

## Seznam odevzdaných souborů
1. `main.cpp` - Hlavní soubor programu, obsahující vstupní bod aplikace.
2. `IMAPClient.cpp` - Implementace třídy pro připojení k IMAP serveru a stahování e-mailů.
3. `IMAPClient.h` - Hlavičkový soubor pro třídu IMAPClient.
4. `Config.cpp` - Implementace třídy pro zpracování parametrů příkazového řádku.
5. `Config.h` - Hlavičkový soubor pro třídu Config.
6. `AuthFile.cpp` - Implementace třídy pro načtení přihlašovacích údajů.
7. `AuthFile.h` - Hlavičkový soubor pro třídu AuthFile.
8. `TLSHandler.cpp` - Implementace třídy pro správu TLS připojení.
9. `TLSHandler.h` - Hlavičkový soubor pro třídu TLSHandler.
10. `Makefile` - Skript pro kompilaci projektu.
11. `README.md` - Tento popisný soubor.
12. `manual.pdf` - Soubor s dokumentací