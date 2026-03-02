DROP TABLE FEEDBACK;
DROP TABLE OBJEDNAVKAPROSTOR;
DROP TABLE POLOZKYOBJEDNAVKY;
DROP TABLE OBJEDNAVKA;
DROP TABLE POLOZKYMENU;
DROP TABLE REZERVACEPROSTOR;
DROP TABLE REZERVACE;
DROP TABLE SAL;
DROP TABLE STUL;
DROP TABLE PROSTOR;
DROP TABLE ZAKAZNIK;
DROP TABLE ZAMESTNANEC;
DROP TABLE UZIVATEL;

/*
Tuto generalizaci-specializaci jsme provedli timto zpusobem z duvodu, ze pujde snadno pridavat nove tabulky, pokud prijde dalsi prostor,
napr. bowling, kulecnik, ci sipky. Mame tedy tabulku pro nadtyp + podtypy s primarnim klicem nadtypu
*/
CREATE TABLE Prostor (
  id INT GENERATED AS IDENTITY PRIMARY KEY,
  lokace VARCHAR(255) NOT NULL
);

CREATE TABLE Sal (
  id INT PRIMARY KEY,
  kapacita INT NOT NULL,
  vybaveni VARCHAR(255) NOT NULL,
  FOREIGN KEY (id) REFERENCES Prostor(id)
);

CREATE TABLE Stul (
  id INT PRIMARY KEY,
  pocet_mist INT NOT NULL,
  FOREIGN KEY (id) REFERENCES Prostor(id)
);


/*
Reseni teto generalizace/specializace jsme zvolili stejne jako v predchozim pripade.
Aby se neduplikovaly atributy v pripade pridani noveho typu uzivatele, vyuzivame spolecne tabulky pro
nadtyp a nasledny podtypu s primarnim klice nadtypu.
*/
CREATE TABLE Uzivatel (
  id INT GENERATED AS IDENTITY PRIMARY KEY ,
  jmeno VARCHAR(80) NOT NULL,
  prijmeni VARCHAR(100) NOT NULL,
  email VARCHAR(100) NOT NULL UNIQUE,
  telefon VARCHAR(20) NOT NULL
);

CREATE TABLE Zamestnanec (
  id INT NOT NULL,
  rodne_cislo VARCHAR(11) UNIQUE NOT NULL,
  CONSTRAINT chk_rodne_cislo CHECK (REGEXP_LIKE(rodne_cislo, '^\d{6}/\d{3}\d$')),
  FOREIGN KEY (id) REFERENCES Uzivatel(id),
  PRIMARY KEY (id)
);

CREATE TABLE Zakaznik (
  id INT NOT NULL,
  vernostni_body INT DEFAULT 0,
  FOREIGN KEY (id) REFERENCES Uzivatel(id),
  PRIMARY KEY (id)
);

CREATE TABLE Rezervace (
    id INT GENERATED AS IDENTITY PRIMARY KEY,
    pocet_lidi INT NOT NULL,
    vytvoren DATE DEFAULT SYSDATE NOT NULL,
    zacatek DATE NOT NULL,
    konec DATE NOT NULL,
    stav VARCHAR(20) NOT NULL,
    CONSTRAINT chk_rezervaceStav CHECK (stav IN ('Vytvoren', 'Potvrzen', 'Zrusen')),
    poznamka VARCHAR(512),
    jmeno VARCHAR(80),
    prijmeni VARCHAR(100),
    telefon VARCHAR(20),
    druh_akce VARCHAR(20) NOT NULL,
    CONSTRAINT chk_druh_akce CHECK (druh_akce IN ('Firemni', 'Rodinna', 'Oslava', 'Jine')),
    id_zakaznik INT,
    FOREIGN KEY (id_zakaznik) REFERENCES Zakaznik(id)
);

CREATE TABLE RezervaceProstor (
  rezervaceID INT NOT NULL ,
  prostorID INT NOT NULL ,
  FOREIGN KEY (rezervaceID) REFERENCES Rezervace(id),
  FOREIGN KEY (prostorID) REFERENCES Prostor(id),
  PRIMARY KEY (rezervaceID, prostorID)
);

CREATE TABLE Objednavka (
  id INT GENERATED AS IDENTITY PRIMARY KEY,
  utrata_dph INT NOT NULL,
  utrata_bez_dph INT NOT NULL,
  poznamka VARCHAR(512),
  vytvorena DATE DEFAULT SYSDATE NOT NULL,
  stav VARCHAR(20) NOT NULL,
  CONSTRAINT chk_objednavkaStav CHECK (stav IN ('Nova', 'Stornovano', 'Zaplaceno', 'Chyba')),
  sleva INT DEFAULT 0,
  zakaznikID INT,
  FOREIGN KEY (zakaznikID) REFERENCES Zakaznik(id),
  zamestnanecID INT NOT NULL ,
  FOREIGN KEY (zamestnanecID) REFERENCES Zamestnanec(id)
);

CREATE TABLE ObjednavkaProstor (
  objednavkaID INT NOT NULL,
  prostorID INT NOT NULL,
  PRIMARY KEY (objednavkaID, prostorID),
  FOREIGN KEY (objednavkaID) REFERENCES Objednavka(id),
  FOREIGN KEY (prostorID) REFERENCES Prostor(id)
);

CREATE TABLE Feedback (
  id INT GENERATED AS IDENTITY PRIMARY KEY,
  jidlo_hodnoceni INT NOT NULL,
  obsluha_hodnoceni INT NOT NULL,
  prostredi_hodnoceni INT NOT NULL,
  CONSTRAINT chk_hodnoceni CHECK (
    obsluha_hodnoceni BETWEEN 0 AND 10 AND
    jidlo_hodnoceni BETWEEN 0 AND 10 AND
    prostredi_hodnoceni BETWEEN 0 AND 10
  ),
  poznamka VARCHAR(512),
  objednavkaID INT NOT NULL,
  FOREIGN KEY (objednavkaID) REFERENCES  Objednavka(id)
);

CREATE TABLE PolozkyMenu (
  id INT GENERATED AS IDENTITY PRIMARY KEY,
  nazev VARCHAR(50) NOT NULL,
  cena_dph INT NOT NULL,
  cena_bez_dph INT NOT NULL,
  alergeny VARCHAR(255)
);

CREATE TABLE PolozkyObjednavky (
    objednavkaID INT,
    polozkaID INT,
    mnozstvi INT,
    cena_za_jednotku DECIMAL(10, 2),
    PRIMARY KEY (objednavkaID, polozkaID),
    FOREIGN KEY (objednavkaID) REFERENCES Objednavka(id),
    FOREIGN KEY (polozkaID) REFERENCES PolozkyMenu(id)
);


INSERT INTO Prostor (lokace) VALUES ('Hlavní sál');
INSERT INTO Prostor (lokace) VALUES ('VIP sál');
INSERT INTO Prostor (lokace) VALUES ('Terasa');
INSERT INTO Prostor (lokace) VALUES ('Stůl 1');
INSERT INTO Prostor (lokace) VALUES ('Stůl 2');
INSERT INTO Prostor (lokace) VALUES ('Stůl 3');


INSERT INTO Sal (id, kapacita, vybaveni) VALUES (1, 100, 'Projektor, Ozvučení');
INSERT INTO Sal (id, kapacita, vybaveni) VALUES (2, 20, 'Soukromý bar, TV');

INSERT INTO Stul (id, pocet_mist) VALUES (4, 4);
INSERT INTO Stul (id, pocet_mist) VALUES (5, 6);
INSERT INTO Stul (id, pocet_mist) VALUES (6, 6);


-- Zam?stnanci
INSERT INTO Uzivatel (jmeno, prijmeni, email, telefon) VALUES ('Jan', 'Novák', 'jan.novak@restaurace.cz', '123456789');
INSERT INTO Uzivatel (jmeno, prijmeni, email, telefon) VALUES ('Eva', 'Svobodová', 'eva.svobodova@restaurace.cz', '987654321');

-- Z?kazn?ci
INSERT INTO Uzivatel (jmeno, prijmeni, email, telefon) VALUES ('Petr', 'Kovář', 'petr.kovar@example.com', '112233445');
INSERT INTO Uzivatel (jmeno, prijmeni, email, telefon) VALUES ('Lucie', 'Horáková', 'lucie.horakova@example.com', '556677889');

INSERT INTO Zamestnanec (id, rodne_cislo) VALUES (1, '860505/1234');
INSERT INTO Zamestnanec (id, rodne_cislo) VALUES (2, '740201/5678');


INSERT INTO Zakaznik (id, vernostni_body) VALUES (3, 150);
INSERT INTO Zakaznik (id, vernostni_body) VALUES (4, 300);


INSERT INTO Rezervace (pocet_lidi, zacatek, konec, stav, druh_akce, id_zakaznik) VALUES
(20, TO_DATE('2023-10-15 14:00:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-10-15 18:00:00', 'YYYY-MM-DD HH24:MI:SS'), 'Potvrzen', 'Firemni', 3);
INSERT INTO Rezervace (pocet_lidi, vytvoren, zacatek, konec, stav, druh_akce, id_zakaznik) VALUES
(5, CURRENT_DATE, TO_DATE('2023-01-15', 'YYYY-MM-DD'), TO_DATE('2023-01-15 22:00', 'YYYY-MM-DD HH24:MI:SS'), 'Potvrzen', 'Oslava', 4);
INSERT INTO Rezervace (pocet_lidi, vytvoren, zacatek, konec, stav, druh_akce, jmeno, prijmeni, telefon) VALUES
(10, CURRENT_DATE, TO_DATE('2023-01-20', 'YYYY-MM-DD'), TO_DATE('2023-01-20 22:00', 'YYYY-MM-DD HH24:MI:SS'), 'Vytvoren', 'Firemni', 'Jan', 'Kuča', '605158415');


INSERT INTO RezervaceProstor (rezervaceID, prostorID) VALUES (1, 1);
INSERT INTO RezervaceProstor (rezervaceID, prostorID) VALUES (2, 2);
INSERT INTO RezervaceProstor (rezervaceID, prostorID) VALUES (3, 4);
INSERT INTO RezervaceProstor (rezervaceID, prostorID) VALUES (3, 5);


INSERT INTO Objednavka (utrata_dph, utrata_bez_dph, stav, zakaznikID, zamestnanecID) VALUES
(5000, 4300, 'Zaplaceno', 3, 1);

INSERT INTO Objednavka (utrata_dph, utrata_bez_dph, stav, zamestnanecID) VALUES
(500, 410, 'Zaplaceno', 1);

INSERT INTO Objednavka (utrata_dph, utrata_bez_dph, stav, zamestnanecID) VALUES
(800, 680, 'Zaplaceno', 2);

INSERT INTO Objednavka (utrata_dph, utrata_bez_dph, stav, zakaznikID, zamestnanecID) VALUES
(120, 80, 'Nova', 4, 2);

INSERT INTO ObjednavkaProstor (objednavkaID, prostorID) VALUES (1, 1);
INSERT INTO ObjednavkaProstor (objednavkaID, prostorID) VALUES (2, 2);
INSERT INTO ObjednavkaProstor (objednavkaID, prostorID) VALUES (3, 4);
INSERT INTO ObjednavkaProstor (objednavkaID, prostorID) VALUES (4, 5);

INSERT INTO Feedback (jidlo_hodnoceni, obsluha_hodnoceni, prostredi_hodnoceni, objednavkaID) VALUES
(8, 9, 10, 1);

INSERT INTO Feedback (jidlo_hodnoceni, obsluha_hodnoceni, prostredi_hodnoceni, objednavkaID) VALUES
(2, 3, 6, 4);

INSERT INTO PolozkyMenu (nazev, cena_dph, cena_bez_dph) VALUES ('Steak', 350, 300);
INSERT INTO PolozkyMenu (nazev, cena_dph, cena_bez_dph) VALUES ('Vegetariánský risotto', 250, 215);
INSERT INTO PolozkyMenu (nazev, cena_dph, cena_bez_dph, alergeny) VALUES ('Svíčková s knedlíky', 250, 215, 'mléko');
INSERT INTO PolozkyMenu (nazev, cena_dph, cena_bez_dph) VALUES ('Francouzská palačinky', 210, 180);
INSERT INTO PolozkyMenu (nazev, cena_dph, cena_bez_dph) VALUES ('Pivo Radegast 10', 45, 215);
INSERT INTO PolozkyMenu (nazev, cena_dph, cena_bez_dph) VALUES ('Pivo Radegast 12', 55, 215);

INSERT INTO PolozkyObjednavky (objednavkaID, polozkaID, mnozstvi) VALUES (1, 1, 2);
INSERT INTO PolozkyObjednavky (objednavkaID, polozkaID, mnozstvi) VALUES  (1, 6, 4);
INSERT INTO PolozkyObjednavky (objednavkaID, polozkaID, mnozstvi) VALUES (2, 1, 2);
INSERT INTO PolozkyObjednavky (objednavkaID, polozkaID, mnozstvi) VALUES  (3, 3, 3);
INSERT INTO PolozkyObjednavky (objednavkaID, polozkaID, mnozstvi) VALUES  (4, 5, 5);



-- Dotaz na nalezení celkových výdajů s DPH pro každého zákazníka, který provedl rezervaci
SELECT r.id_zakaznik AS id_zakaznika, SUM(o.utrata_dph) AS celkove_vydaje
FROM Rezervace r
INNER JOIN Objednavka o ON r.id_zakaznik = o.zakaznikID
GROUP BY r.id_zakaznik;

-- Dotaz na získání detailů zpětné vazby pro zaplacené objednávky
SELECT f.id, o.id AS id_objednavky, f.jidlo_hodnoceni, f.obsluha_hodnoceni, f.prostredi_hodnoceni
FROM Feedback f
INNER JOIN Objednavka o ON f.objednavkaID = o.id
WHERE o.stav = 'Zaplaceno';

-- Dotaz na výpis rezervací včetně informací o zákaznících a prostoru, ve kterém se rezervace koná.
SELECT r.id AS id_rezervace, u.jmeno, u.prijmeni, p.lokace
FROM Rezervace r
INNER JOIN Uzivatel u ON r.id_zakaznik = u.id
INNER JOIN RezervaceProstor rp ON r.id = rp.rezervaceID
INNER JOIN Prostor p ON rp.prostorID = p.id;

-- Dotaz na seznam počtu objednávek učiněných každým zaměstnancem
SELECT o.zamestnanecID AS id_zamestnance, COUNT(o.id) AS pocet_objednavek
FROM Objednavka o
GROUP BY o.zamestnanecID;

-- Dotaz na získání průměrného hodnocení jídla, služby a prostředí podle typu akce
SELECT r.druh_akce, AVG(f.jidlo_hodnoceni) AS prum_jidlo_hodnoceni,
       AVG(f.obsluha_hodnoceni) AS prum_obsluha_hodnoceni,
       AVG(f.prostredi_hodnoceni) AS prum_prostredi_hodnoceni
FROM Feedback f
JOIN Objednavka o ON f.objednavkaID = o.id
JOIN Rezervace r ON o.zakaznikID = r.id_zakaznik
GROUP BY r.druh_akce;

-- Dotaz na identifikaci zákazníků, kteří provedli rezervaci a zadali objednávku
SELECT DISTINCT u.id AS id_uzivatele, u.jmeno, u.prijmeni
FROM Zakaznik z
JOIN Uzivatel u ON z.id = u.id
WHERE EXISTS (SELECT 1 FROM Rezervace r WHERE r.id_zakaznik = z.id)
AND EXISTS (SELECT 1 FROM Objednavka o WHERE o.zakaznikID = z.id);

-- Dotaz na nalezení všech objednávek obsahující položky z menu s alergeny
SELECT o.id AS id_objednavky
FROM Objednavka o
WHERE o.id IN (SELECT po.objednavkaID
               FROM PolozkyObjednavky po
               JOIN PolozkyMenu pm ON po.polozkaID = pm.id
               WHERE pm.alergeny IS NOT NULL);