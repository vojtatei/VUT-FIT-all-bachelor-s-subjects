# Projekt: imapcl – IMAP4rev1 klient

## Popis

Napište program **imapcl**, který umožní čtení elektronické pošty pomocí protokolu **IMAP4rev1 (RFC 3501)**.

Program po spuštění:
- stáhne zprávy uložené na serveru,
- uloží je do zadaného adresáře (každou zprávu zvlášť),
- na standardní výstup vypíše počet stažených zpráv.

Pomocí dodatečných parametrů je možné funkcionalitu měnit.

---

## Povolené knihovny

Při vytváření programu je povoleno použít:
- hlavičkové soubory pro práci se sockety a další obvyklé funkce používané v síťovém prostředí (např. inetnet/*, sys/*, arpa/* apod.),
- knihovnu pro práci s vlákny (pthread), signály, časem,
- standardní knihovnu jazyka C (ISO/ANSI, POSIX),
- C++ a STL,
- knihovnu OpenSSL pro práci se SSL/TLS.

Jiné knihovny nejsou povoleny.

---

# Spuštění aplikace

## Použití

    imapcl server [-p port] [-T [-c certfile] [-C certaddr]] [-n] [-h] -a auth_file [-b MAILBOX] -o out_dir

Pořadí parametrů je libovolné.

---

## Popis parametrů

- **server**  
  Povinný parametr – název serveru (IP adresa nebo doménové jméno).

- **-p port**  
  Volitelný parametr – číslo portu. Zvolte výchozí hodnotu dle specifikace IMAP a registru portů IANA.

- **-T**  
  Zapíná šifrování (IMAPS). Pokud není uveden, použije se nešifrovaná varianta.

- **-c certfile**  
  Soubor s certifikáty pro ověření SSL/TLS certifikátu serveru.

- **-C certaddr**  
  Adresář, kde se mají vyhledávat certifikáty. Výchozí hodnota: `/etc/ssl/certs`.

- **-n**  
  Stahují se pouze nové zprávy.

- **-h**  
  Stahují se pouze hlavičky zpráv.

- **-a auth_file**  
  Povinný parametr – soubor s autentizací (LOGIN).

- **-b MAILBOX**  
  Název schránky (výchozí hodnota: INBOX).

- **-o out_dir**  
  Povinný parametr – výstupní adresář pro stažené zprávy.

---

# Soubor s autentizačními údaji

Konfigurační soubor obsahuje uživatelské jméno a heslo ve formátu:

    username = jmeno
    password = heslo

Předpokládejte unixový textový soubor zakončený znakem nového řádku.

---

# Výstup aplikace

Po spuštění aplikace:
- vypište pouze informaci o počtu stažených zpráv,
- text nesmí být delší než 1 řádek,
- text upravte podle použití parametrů `-n` a `-h`.

---

## Formát ukládaných zpráv

Zprávy budou ukládány ve formátu **Internet Message Format (RFC 5322)**  
do vhodně pojmenovaných souborů v adresáři specifikovaném parametrem `-o`.

Každá zpráva bude uložena v samostatném souboru.

### Příklad obsahu souboru

    Date: Wed, 14 Sep 2016 03:54:39 -0700
    From: Sender <sender@example.com>
    To: receiver@example.com
    Subject: Message
    Message-ID: <20160914035439.03264562@mininet-vm>

    Toto je tělo e-mailu. Před tělem e-mailu jsou hlavičky a prázdný řádek.

---

# Příklad spuštění programu

    $ imapcl eva.fit.vutbr.cz -o maildir -a cred
    Staženo 15 zpráv ze schránky INBOX.

    $ imapcl 10.10.10.1 -p 9993 -T -n -b Important -o maildir -a cred
    Staženy 2 nové zprávy ze schránky Important.

    $ imapcl eva.fit.vutbr.cz -o maildir -a /dev/null
    Není možné ověřit identitu serveru eva.fit.vutbr.cz.

---

# Referenční prostředí pro překlad

Referenční překlad projektů proběhne na stroji:

    merlin.fit.vutbr.cz

---

# Doporučená literatura

- Registr portů transportních protokolů  
- RFC3501 – INTERNET MESSAGE ACCESS PROTOCOL - VERSION 4rev1  
- RFC5322 – Internet Message Format  
- Základy použití knihovny OpenSSL  

---

# Možná rozšíření projektu

Rozšíření může kompenzovat drobné nedostatky základní implementace.

Možná rozšíření:
- podpora STARTTLS,
- interaktivní režim programu.

---

# Interaktivní režim

Aktivuje se parametrem:

    -i

Program:
- se připojí ke schránce,
- běží až do ukončení příkazem `QUIT`.

## Dostupné příkazy

- DOWNLOAD (NEW|ALL) [MAILDIR]
- READNEW [MAILDIR]

Při přijetí nové zprávy informujte uživatele na standardním výstupu.

---

## Příklad interaktivního režimu

    $ imapcl eva.fit.vutbr.cz -a auth -o maildir -i
    * Na serveru je 10 zpráv ve schránce INBOX, 3 nové.
    * Na serveru je 5 zpráv ve schránce Important, 2 nové.

    DOWNLOADNEW
    * Staženy 3 nové zprávy ze schránky INBOX.

    DOWNLOADNEW Important
    * Staženy 2 nové zprávy ze schránky Important.

    DOWNLOADALL
    * Staženo 10 zpráv ze schránky INBOX.

    DOWNLOADALL Important
    * Staženo 5 zpráv ze schránky Important.

    READNEW
    * Zprávy ve schránce INBOX označeny jako přečtené.

    DOWNLOADNEW
    * Ve schránce INBOX není žádná nová zpráva.
    * Přijata 1 nová zpráva ve schránce INBOX.
    * Přijaty 2 nové zprávy ve schránce INBOX.

    DOWNLOADNEW
    * Staženy 2 nové zprávy ze schránky INBOX.