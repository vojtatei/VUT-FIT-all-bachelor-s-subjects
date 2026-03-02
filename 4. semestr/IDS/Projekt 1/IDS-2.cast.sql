/*
IDS FIT VUT 2024 - 2. Cast projektu
Autori:
Jakub Jerabek (xjerab28)
Vojtech Teichmann (xteich02)
*/

DROP TABLE FEEDBACK;
DROP TABLE OBJEDNAVKAPROSTOR;
DROP TABLE POLOZKYOBJEDNAVKY;
DROP TABLE OBJEDNAVKA;
DROP TABLE POLOZKYMENU;
DROP TABLE REZERVACE_PROSTOR;
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

CREATE TABLE Rezervace_Prostor (
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
    cenaZaJednotku DECIMAL(10, 2),
    PRIMARY KEY (objednavkaID, polozkaID),
    FOREIGN KEY (objednavkaID) REFERENCES Objednavka(id),
    FOREIGN KEY (polozkaID) REFERENCES PolozkyMenu(id)
);


INSERT INTO Prostor (lokace) VALUES ('Hlavní sál');
INSERT INTO Prostor (lokace) VALUES ('Soukromý salonek');
INSERT INTO Prostor (lokace) VALUES ('Zahrada');

INSERT INTO Sal (id, kapacita, vybaveni) VALUES (1, 100, 'Projektor, ozvuèení');
INSERT INTO Sal (id, kapacita, vybaveni) VALUES (2, 20, 'TV, soukromí');
INSERT INTO Stul (id, pocet_mist) VALUES (3, 5);

INSERT INTO Uzivatel (jmeno, prijmeni, email, telefon) VALUES
('Jan', 'Novák', 'jan.novak@email.cz', '123456789');
INSERT INTO Uzivatel (jmeno, prijmeni, email, telefon) VALUES
('Eva', 'Svobodová', 'eva.svobodova@email.cz', '987654321');

INSERT INTO Zamestnanec (id, rodne_cislo) VALUES (1, '890101/1234');
INSERT INTO Zakaznik (id, vernostni_body) VALUES (2, 100);

INSERT INTO Rezervace (pocet_lidi, zacatek, konec, stav, druh_akce, id_zakaznik) VALUES
(20, TO_DATE('2023-10-15 14:00:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-10-15 18:00:00', 'YYYY-MM-DD HH24:MI:SS'), 'Potvrzen', 'Firemni', 2);

INSERT INTO Rezervace_Prostor (rezervaceID, prostorID) VALUES (1, 2);

INSERT INTO Objednavka (utrata_dph, utrata_bez_dph, stav, zakaznikID, zamestnanecID) VALUES
(5000, 4300, 'Zaplaceno', 2, 1);

INSERT INTO ObjednavkaProstor (objednavkaID, prostorID) VALUES (1, 2);

INSERT INTO Feedback (jidlo_hodnoceni, obsluha_hodnoceni, prostredi_hodnoceni, objednavkaID) VALUES
(8, 9, 10, 1);

INSERT INTO PolozkyMenu (nazev, cena_dph, cena_bez_dph) VALUES ('Steak', 350, 300);
INSERT INTO PolozkyMenu (nazev, cena_dph, cena_bez_dph) VALUES ('Vegetariánské risotto', 250, 215);

INSERT INTO PolozkyObjednavky (objednavkaID, polozkaID, mnozstvi, cenaZaJednotku) VALUES (1, 1, 2, 350.00);
INSERT INTO PolozkyObjednavky (objednavkaID, polozkaID, mnozstvi, cenaZaJednotku) VALUES  (1, 2, 3, 250.00);
