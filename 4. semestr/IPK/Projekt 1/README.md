# Dokumentace Projektu: IPK24-CHAT

## Obsah
1. [�vod](#�vod)
2. [Teoretick� z�klad](#teoretick�-z�klad)
3. [Testov�n�](#testov�n�)
4. [Popis Implementace](#popis-implementace)


## �vod
Tato dokumentace popisuje implementaci klienta pro komunikaci s chat serverem. Klient podporuje protokoly TCP a UDP podle specifikace `IPK24-CHAT`.
Aplikace umo��uje u�ivatel�m komunikovat v re�ln�m �ase, vyu��vaj�c s�t� jak p�es protokol TCP, tak UDP. TCP je vyu��v�n pro spolehliv�, bodov�-ke-bodov� spojen� a je vhodn� pro p�enos citliv�ch dat a zpr�v vy�aduj�c�ch potvrzen� doru�en�. UDP na druhou stranu poskytuje rychl� p�enosy s men�� z�t�� pro s�, co� je ide�ln� pro situace, kde okam�itost doru�en� p�eva�uje nad absolutn� spolehlivost�.

## Teoretick� z�klad
Klient pro chat server je postaven na z�kladech s�tov� komunikace a princip� distribuovan�ch aplikac�. V n�sleduj�c�ch odstavc�ch jsou pops�ny teoretick� koncepty, na nich� je klient zalo�en.

### S�ov� komunikace
Aplikace vyu��v� s�ovou komunikaci pro interakci mezi klientem a serverem. Komunikace prob�h� pomoc� dvou hlavn�ch transportn�ch protokol�:

- **TCP (Transmission Control Protocol):** Spolehliv�, orientovan� na spojen� protokol, kter� zaji��uje doru�en� dat ve spr�vn�m po�ad� a bez chyb. Je ide�ln� pro situace, kde je d�le�it� integrita dat, jako je odes�l�n� zpr�v v chatovac�ch aplikac�ch.
  
- **UDP (User Datagram Protocol):** Nespolehliv�, bezstavov� protokol, kter� odes�l� datagramy bez potvrzen� doru�en�. Je vhodn� pro aplikace vy�aduj�c� rychlou komunikaci, kde n�kter� ztracen� datagramy nezp�sob� kritick� probl�my, jako nap��klad p�i streamov�n� videa nebo hlasov� komunikaci.

### Protokoly a k�dov�n� zpr�v
Implementace klienta se ��d� vlastn�m aplika�n�m protokolem definovan�m pro tento projekt, kter� ur�uje form�t a pravidla pro v�m�nu zpr�v. Protokol m� n�sleduj�c� charakteristiky:

- **Form�t zpr�v:** Ka�d� typ zpr�vy (nap�. autentiza�n�, chybov� zpr�vy, chatov� zpr�vy) m� specifick� form�t, kter� je definov�n v k�du a dokumentaci. Form�t zpr�v je navr�en tak, aby byl jednozna�n� parsrovateln� na stran� klienta i serveru.
  
- **K�dov�n�:** Zpr�vy jsou k�dov�ny v UTF-8. TCP zpr�vy jsou k�dov�ny jako �ist� text s ukon�ovac�m sekvenc� CRLF, zat�mco UDP zpr�vy jsou ve form� bin�rn�ch dat.
  
- **Zpracov�n� chyb:** Aplikace obsahuje mechanizmy pro zachycen� a zpracov�n� chyb b�hem komunikace. Chybov� zpr�vy umo��uj� klientovi reagovat na probl�my, jako jsou probl�my s autentizac� nebo s�ov� spojen�.

### Multithreading a asynchronn� operace
Aplikace implementuje v�cevl�knovost a asynchronn� operace pro zlep�en� odezvy a v�konu:

- **Asynchronn� I/O operace:** Asynchronn� vstupn�-v�stupn� operace jsou vyu��v�ny pro �ten� a z�pis s�ov�ch proud�, co� umo��uje aplikaci pokra�ovat v b�hu, ani� by �ekala na dokon�en� t�chto operac�. To je kl��ov� pro udr�en� reaktivity u�ivatelsk�ho rozhran�.
  
- **V�cevl�knov� naslouch�n�:** Klient m��e naslouchat p��choz�m zpr�v�m na pozad�, zat�mco z�rove� umo��uje u�ivatel�m zad�vat p��kazy a odes�lat zpr�vy.

### Bezpe�nost a autentizace
Bezpe�nost je z�sadn� pro komunika�n� aplikace, a proto klient obsahuje funkce pro zabezpe�en� p�ihla�ov�n� a komunikace:

- **Autentizace u�ivatele:** P�ed zah�jen�m komunikace mus� u�ivatel� poskytnout autentiza�n� �daje. To zahrnuje u�ivatelsk� jm�no, heslo a zobrazovan� jm�no.
  
- **�ifrov�n� hesel:** Hesla jsou p�en�ena v �ifrovan� form� (v tomhle p��pad� Secret), aby se zabr�nilo jejich zachycen� b�hem p�enosu p�es s�.


## Testov�n�
Testov�n� prob�halo na lok�ln�m i vzd�len�m serveru.

Nad�le jsem pou�il testovac� skript na testov�n� v jazyce Python, kter� pos�l�m ve slo�ce Tests a z�rove� dod�v�m, �e nejsem vlastn�kem tohohle skriptu.
Skript byl pouze upraven na m�j projekt, p�id�n�m funkce Sleep.

### Co bylo testov�no

- P�ipojen� k serveru
- Autentizace u�ivatele
- Pos�l�n� a p��jem zpr�v
- P�ep�n�n� mezi kan�ly
- Odpojen� ze serveru

### Pro� to bylo testov�no

Testoval jsem tyhle z�kladn� instrukce, kter� byly v zad�n�, aby jsem ov��il z�kladn� funkcionalitu, kter� byla zadan�.

### Jak to bylo testov�no

#### TCP

S protokolem TCP bylo testov�n� velmi pohodln� a snadn� oproti UDP.
Pou��val jsem n�stroje netcat a wireshark. Nejd��ve jsem podle zad�n� zprovoznil ob� dv� prost�ed�.
U netcatu jsem m�l chv�li probl�m, jak to rozjet na Windows, ale nakonec bylo jednodu��� to spustit ve WSL v jednom terminalu, kter� slou�il jako Server, kter� naslouchal a n�sledn� jsem spustil program v druh�m termin�lu ve Windows.
To mi umo�nilo kr�sn� vid�t, co odes�l�m a jak to vypad� na stran� Serveru. Velik� v�hoda je, �e jsem mohl testovat, kdy� mi p�i�la �patn� zpr�va od Serveru, co� u referen�n�ho Serveru nebylo mo�n� tuhle vlastnost otestovat.
T�m p�dem jsem byl schopn� si simulovat v�t�inu situac�.
V moment kdy jsem m�l dostate�n� otestov�no lok�ln�, p�ipojil jsem se na referen�n� discord server a fungovalo to dob�e. Z�rove� jsem ve Wiresharku sledoval komunikaci, co pos�l�m Serveru a co on pos�l� mn�.
N�sledn� jsem pou�il i testovac� skript, kter� mi odhalil krajn� p��pady, kter� jsem nem�l o�et�en�, nap��klad autentifikace po tom, co� jsem ji� autentifikovan�.



[TCP Netcat](https://imgur.com/2I4QCl3)
[Wireshark](https://imgur.com/QBmtugR)

#### UDP

S protokolem UDP bylo testov�n� mnohem obti��j��, jak i samotn� v�voj. Testoval jsem to na serveru, kter� vytvo�il jeden z m�ch spolu��ku a sledov�n�m wiresharku.
Jinak jsem postupoval obdobn�, jak u TCP. Pos�lal jsem zpr�vy a sledoval komunikaci ve Wiresharku. Obecn� jsem r�d, �e jsem zjistil, jak mocn� je Wireshark. Jak jsem ho spustil poprv�, byl jsem docela zmaten�, ale kdy� se s t�m �lov�k z�ije, tak je to kr�sn� a bez n�ho by jsem v�bec nev�d�l, kde m�m nap��klad chybu.
V pozd�j�� f�zi jsem pou�il pr�v� ji� d��ve zm�n�n� testovac� skript, kter� mi odhalil docela dost chyb, kter� jsem se n�sledn� pokusil opravit.

[UDP server test](https://imgur.com/Okwk78U)

#### V�stupy z test�

[UDP ��st v�stupu](https://imgur.com/Mo0OGJH)

[TCP ��st v�stupu](https://imgur.com/doRoorF)



### Testovac� prost�ed�
Server b�el na adrese `127.0.0.1` s portem `4567`. Klient byl testov�n na opera�n�m syst�mu Windows 11 a WSL Ubuntu 20.04.
U n�stroje netcat byl Server spu�t�n ve WSL Ubuntu 20.04 a Klient na Windows 11 v n�stroji PowerShell.


## Popis Implementace

Implementace klienta pro chat server je rozd�lena do n�kolika modul� a t��d, kter� umo��uj� komunikaci a interakci s u�ivatelem.

### Klient
Adres�� `Client` obsahuje t��dy, kter� jsou z�kladem pro s�ov� komunikace.

#### `Client.cs`
- Abstraktn� z�kladn� t��da pro v�echny typy klient�.

#### `TCPClient.cs`
 - `TCPClient` je implementov�n s vyu�it�m t��d `TcpClient` a `NetworkStream` pro ��zen� spojen� a v�m�nu dat. Tato t��da zaji��uje spolehliv�, sekven�n� a bezchybn� doru�en� zpr�v d�ky nav�z�n� trval�ho spojen� se serverem. Zahrnuje metody pro asynchronn� odes�l�n� zpr�v, naslouch�n� p��choz�m zpr�v�m a zachycov�n� v�jimek p�i s�ov�ch operac�ch.

#### `UDPClient.cs`
- `UDPClient` pou��v� `UdpClient` pro odes�l�n� a p�ij�m�n� datagram� v r�mci bezstavov� komunikace. Tato t��da je optimalizov�na pro rychl� p�enosy, kde okam�it� reakce a n�zk� s�ov� re�ie jsou d�le�it�j�� ne� spolehlivost doru�en�. Implementuje mechanismy pro potvrzov�n� doru�en� zpr�v a op�tovn� odes�l�n�, aby bylo dosa�eno lep�� spolehlivosti p�i zachov�n� charakteristik protokolu UDP.

### P��kazy
Adres�� `Commands` obsahuje implementace u�ivatelsk�ch p��kaz�.

#### `AuthCommand.cs`
- Zpracov�v� autentiza�n� p��kazy a pos�l� p�ihla�ovac� zpr�vy na server.

#### `HelpCommand.cs`
- Poskytuje n�pov�du pro dostupn� p��kazy v aplikaci.

#### `ICommand.cs`
- Definuje rozhran�, kter� mus� implementovat v�echny p��kazy.

#### `JoinCommand.cs`
- Umo��uje klientovi p�ipojit se ke konkr�tn�mu chatovac�mu kan�lu.

#### `MessCommand.cs`
- ��d� odes�l�n� zpr�v do chatu.

#### `RenameCommand.cs`
- Umo��uje zm�nu zobrazovan�ho jm�na u�ivatele v chatu.

### Factory
Adres�� `Factory` poskytuje tov�rn� t��dy pro vytvo�en� p��kaz� a zpr�v.

#### `Command.cs`
- T��da pro vytv��en� instanc� p��kaz�.

#### `Message.cs`
- T��da pro vytv��en� instanc� zpr�v.

### Zpr�vy
Adres�� `Mess` obsahuje t��dy reprezentuj�c� r�zn� typy zpr�v v protokolu.

#### `AuthMess.cs`
- Struktura autentiza�n� zpr�vy pro TCP a UDP komunikaci.

#### `ByeMess.cs`
- T��da pro vytv��en� zpr�v o odpojen�.

#### `ConfirmMess.cs`
- Zpr�va slou��c� k potvrzen� p�ijet� v UDP protokolu.

#### `ErrorMess.cs`
- T��da pro form�tov�n� chybov�ch zpr�v.


### `ArgParse.cs`
- T��da pro analyzov�n� argument� a p�evod na strukturovan� nastaven�.

### `CommandLineSet.cs`
- Uchov�v� konfiguraci z�skanou z argument� p��kazov� ��dky.

### `ChatClient.cs`
- Je j�dro aplikace, kter� inicializuje s�ov�ho klienta na z�klad� protokolu, zpracov�v� u�ivatelsk� vstupy a ��d� p��kazy pro komunikaci s chat serverem.

### `Program.cs`
- Obsahuje metodu `Main`, kter� zpracov�v� argumenty p��kazov� ��dky, inicializuje nastaven� a spou�t� hlavn� smy�ku klienta.
- Vytv��� instanci `ChatClient` s nastaven�m z�skan�m z argument� p��kazov� ��dky.