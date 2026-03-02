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
  email VARCHAR(100) NOT NULL,
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
(5000, 4300, 'Zaplaceno', 4, 1);

INSERT INTO Objednavka (utrata_dph, utrata_bez_dph, stav, zakaznikID, zamestnanecID) VALUES
(5000, 4300, 'Zaplaceno', 3, 1);

INSERT INTO Objednavka (utrata_dph, utrata_bez_dph, stav, zakaznikID, zamestnanecID) VALUES
(1500, 800, 'Zaplaceno', 3, 1);

INSERT INTO Objednavka (utrata_dph, utrata_bez_dph, stav, zakaznikID, zamestnanecID) VALUES
(2750, 2200, 'Zaplaceno', 3, 1);

INSERT INTO Objednavka (utrata_dph, utrata_bez_dph, stav, zakaznikID, zamestnanecID) VALUES
(4100, 3400, 'Zaplaceno', 3, 1);

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
(1, 1, 3, 1);

INSERT INTO Feedback (jidlo_hodnoceni, obsluha_hodnoceni, prostredi_hodnoceni, objednavkaID) VALUES
(2, 3, 6, 2);

INSERT INTO Feedback (jidlo_hodnoceni, obsluha_hodnoceni, prostredi_hodnoceni, objednavkaID) VALUES
(2, 3, 6, 3);

INSERT INTO Feedback (jidlo_hodnoceni, obsluha_hodnoceni, prostredi_hodnoceni, objednavkaID) VALUES
(2, 3, 6, 4);

INSERT INTO Feedback (jidlo_hodnoceni, obsluha_hodnoceni, prostredi_hodnoceni, objednavkaID) VALUES
(1, 1, 1, 8);


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



--------------------------------------------------------------------------------------------
-------------------------------- 4. část ---------------------------------------------------
--------------------------------------------------------------------------------------------


-- TRIGGERY

DROP TRIGGER Adjust_Loyalty_Points;

CREATE OR REPLACE TRIGGER Adjust_Loyalty_Points
AFTER UPDATE OF stav ON Objednavka
FOR EACH ROW
BEGIN
    -- Když se stav změní z 'Zaplaceno' na něco jiného, odečtou se body
    IF :OLD.stav = 'Zaplaceno' AND :NEW.stav != 'Zaplaceno' THEN
        UPDATE Zakaznik
        SET vernostni_body = vernostni_body - (:OLD.utrata_dph / 10)
        WHERE ID = :OLD.zakaznikId;
    -- Když se stav změní na 'Zaplaceno', přidají se body
    ELSIF :OLD.stav != 'Zaplaceno' AND :NEW.stav = 'Zaplaceno' THEN
        UPDATE Zakaznik
        SET vernostni_body = vernostni_body + (:NEW.utrata_dph / 10)
        WHERE ID = :NEW.zakaznikId;
    END IF;
END;
/


CREATE TABLE RezervaceProstorHistory (
  rezervaceID INT,
  prostorID INT,
  akce CHAR(1),
  cas_akce DATE
);

CREATE OR REPLACE TRIGGER TrackRezervaceProstorHistory
AFTER INSERT OR UPDATE OR DELETE ON RezervaceProstor
FOR EACH ROW
DECLARE
  akce_typ CHAR(1);
BEGIN
  -- Určení typu akce: 'I' pro INSERT, 'U' pro UPDATE, 'D' pro DELETE
  IF INSERTING THEN
    akce_typ := 'I';
  ELSIF UPDATING THEN
    akce_typ := 'U';
  ELSIF DELETING THEN
    akce_typ := 'D';
  END IF;

  -- Vložení záznamu do historické tabulky
  INSERT INTO RezervaceProstorHistory (
    rezervaceID,
    prostorID,
    akce,
    cas_akce
  ) VALUES (
    :NEW.rezervaceID,
    :NEW.prostorID,
    akce_typ,
    SYSDATE
  );
END;
/


CREATE TABLE RezervaceHistory (
  rezervaceID INT,
  pocet_lidi INT,
  zacatek DATE,
  konec DATE,
  stav VARCHAR(20),
  akce CHAR(1),
  cas_akce DATE
);


CREATE OR REPLACE TRIGGER TrackRezervaceHistory
AFTER INSERT OR UPDATE OR DELETE ON Rezervace
FOR EACH ROW
DECLARE
  akce_typ CHAR(1);
BEGIN
  -- Určení typu akce: 'I' pro INSERT, 'U' pro UPDATE, 'D' pro DELETE
  IF INSERTING THEN
    akce_typ := 'I';
  ELSIF UPDATING THEN
    akce_typ := 'U';
  ELSIF DELETING THEN
    akce_typ := 'D';
  END IF;

  -- Vložení záznamu do tabulky historie
  INSERT INTO RezervaceHistory (
    rezervaceID,
    pocet_lidi,
    zacatek,
    konec,
    stav,
    akce,
    cas_akce
  ) VALUES (
    :NEW.id,
    :NEW.pocet_lidi,
    :NEW.zacatek,
    :NEW.konec,
    :NEW.stav,
    akce_typ,
    SYSDATE
  );
END;
/

--PROCEDURY

CREATE OR REPLACE PROCEDURE ZobrazObjednavkyZakaznika (p_zakaznikID IN Zakaznik.id%TYPE)
IS
  CURSOR objednavky_cursor IS
    SELECT o.id, o.utrata_dph, COUNT(poi.polozkaID) AS pocet_polozek
    FROM Objednavka o
    JOIN PolozkyObjednavky poi ON o.id = poi.objednavkaID
    WHERE o.zakaznikID = p_zakaznikID
    GROUP BY o.id, o.utrata_dph;

  v_objednavka objednavky_cursor%ROWTYPE;
  data_found BOOLEAN := FALSE;  -- Přidáno pro sledování, zda byl načten nějaký řádek
BEGIN
  OPEN objednavky_cursor;
  LOOP
    FETCH objednavky_cursor INTO v_objednavka;
    EXIT WHEN objednavky_cursor%NOTFOUND;
    data_found := TRUE;  -- Aktualizace, když najdeme alespoň jeden řádek
    DBMS_OUTPUT.PUT_LINE('Objednavka ID: ' || v_objednavka.id ||
                         ', Utrata s DPH: ' || v_objednavka.utrata_dph ||
                         ' Kc, Pocet polozek: ' || v_objednavka.pocet_polozek);
  END LOOP;
  CLOSE objednavky_cursor;

  IF NOT data_found THEN  -- Pokud nebyl načten žádný řádek, informujeme o tom
    DBMS_OUTPUT.PUT_LINE('Nebyly nalezeny zadne objednavky pro daneho zakaznika.');
  END IF;
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('Doslo k chybe: ' || SQLERRM);
END ZobrazObjednavkyZakaznika;
/

CREATE OR REPLACE PROCEDURE AktualizujStavRezervace (p_rezervaceID IN Rezervace.id%TYPE, p_novy_stav IN Rezervace.stav%TYPE)
IS
  v_puvodni_stav Rezervace.stav%TYPE;
BEGIN
  SELECT stav INTO v_puvodni_stav FROM Rezervace WHERE id = p_rezervaceID;

  IF v_puvodni_stav = p_novy_stav THEN
    RAISE_APPLICATION_ERROR(-20001, 'Novy stav je stejny jako puvodni stav.');
  ELSE
    UPDATE Rezervace SET stav = p_novy_stav WHERE id = p_rezervaceID;


      INSERT INTO RezervaceHistory (rezervaceID, stav, akce, cas_akce)
      VALUES (p_rezervaceID, p_novy_stav, 'U', SYSDATE);
  END IF;
EXCEPTION
  WHEN NO_DATA_FOUND THEN
    DBMS_OUTPUT.PUT_LINE('Rezervace s danym ID nebyla nalezena.');
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('Doslo k chybe: ' || SQLERRM);
END AktualizujStavRezervace;
/

call AktualizujStavRezervace(23, 'Potvrzen');

call ZobrazObjednavkyZakaznika(3);

--Práva pro druhého člena týmu
BEGIN
  FOR t IN (SELECT table_name FROM user_tables) LOOP
    EXECUTE IMMEDIATE 'GRANT SELECT, INSERT, UPDATE, DELETE  ON ' || t.table_name || ' TO XJERAB28';
  END LOOP;
END;
/

BEGIN
  FOR mv IN (SELECT mview_name FROM user_mviews) LOOP
    EXECUTE IMMEDIATE 'GRANT SELECT, INSERT, UPDATE, DELETE ON ' || mv.mview_name || ' TO XJERAB28';
  END LOOP;
END;
/

--vytvoření mterializovaného pohledu

CREATE MATERIALIZED VIEW XTEICH02.mv_rezervace_souhrn
REFRESH COMPLETE ON DEMAND
AS
SELECT stav, COUNT(*) AS pocet_rezervaci
FROM XTEICH02.Rezervace
GROUP BY stav;

--druhý člen týmu pak zadá tohle: SELECT * FROM xteich02.mv_rezervace_souhrn;



-- Porovnani, jak indexace zrychluje vyhledávání uživatele pomocí e-mailu
-- Přístup k tabulce 'Objednavka' je pořád realizován pomocí TABLE ACCESS FULL
-- Možným dalším zrychlením by tedy bylo indexování sloupce 'zakaznikID' v tabulce Objednavka
EXPLAIN PLAN FOR
SELECT u.email, AVG(o.utrata_dph) AS prumerna_utrata
FROM Uzivatel u
JOIN Zakaznik z ON u.id = z.id
JOIN Objednavka o ON z.id = o.zakaznikID
GROUP BY u.email;
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

-- Indexace sloupce e-mail v tabulce Uzivatel
-- tahle indexace vede ke snizeni az o 300 BYTU (ze 455 na 144) i snizeni CPU narocnosti
CREATE INDEX idx_user_email ON Uzivatel(email);


-- Získává průměrné hodnocení zákazníků, dle toho kolik učinili objednávek 
WITH CustomerFeedback AS (
    SELECT
        z.id AS zakaznik_id,
        AVG(f.jidlo_hodnoceni) AS avg_jidlo,
        AVG(f.obsluha_hodnoceni) AS avg_obsluha,
        AVG(f.prostredi_hodnoceni) AS avg_prostredi
    FROM
        Zakaznik z
    JOIN Objednavka o ON z.id = o.zakaznikID
    JOIN Feedback f ON o.id = f.objednavkaID
    GROUP BY z.id
), OrderCategories AS (
    SELECT
        z.id,
        COUNT(o.id) AS pocet_objednavek,
        CASE
            WHEN COUNT(o.id) < 3 THEN 'Nízké'
            WHEN COUNT(o.id) BETWEEN 3 AND 5 THEN 'Střední'
            ELSE 'Vysoké'
        END AS objednavkova_kategorie
    FROM
        Zakaznik z
    JOIN Objednavka o ON z.id = o.zakaznikID
    GROUP BY z.id
)
SELECT
    oc.objednavkova_kategorie,
    AVG(cf.avg_jidlo) AS prumerne_hodnoceni_jidlo,
    AVG(cf.avg_obsluha) AS prumerne_hodnoceni_obsluha,
    AVG(cf.avg_prostredi) AS prumerne_hodnoceni_prostredi
FROM
    CustomerFeedback cf
JOIN
    OrderCategories oc ON cf.zakaznik_id = oc.id
GROUP BY
    oc.objednavkova_kategorie;

