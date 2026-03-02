# Informační systém pro správu konferencí

## Zadání

Úkolem je vytvořit informační systém pro tvorbu událostí a správu účastníků konferencí obsahujících prezentace vybraných přednášejících.

### Konference

Každá konference obsahuje:

- **Označení (název)** – pro jednoznačné odlišení
- **Popis**
- **Žánr / oblast zájmu**
- **Místo konání**
- **Časový interval konání**
- **Cena vstupného**
- **Kapacita návštěvníků**

Dále konference obsahuje:

- Libovolný počet **místností**
- **Časový rozpis přednášek**
- Přijaté **prezentace přednášejících**

### Prezentace

Každá prezentace obsahuje:

- Název
- Přednášejícího
- Popis
- Tagy (volitelné)
- Fotku / logo / poster (volitelné)
- Přesně vymezený čas konání
- Konkrétní místnost na konferenci

---

# Role uživatelů a jejich oprávnění

## Administrátor

- Spravuje uživatele.
- Má práva všech níže uvedených rolí.

---

## Registrovaný uživatel

### Role pořadatele

- Zakládá konference (stává se jejich pořadatelem).
- Spravuje místnosti a obsah konference.
- Schvaluje prezentace přednášejících.
- Vytváří časový rozpis přednášek v místnostech  
  - Je nutné ošetřit časové kolize.
- Spravuje rezervace návštěvníků.
- Po úhradě vstupného potvrzuje rezervace.
- Případně vydává vstupenky (pokud je hrazeno na místě).

### Role přednášejícího

- Registruje své příspěvky na konference (zasílá návrh prezentace – postačí textová forma).
- Po schválení se stává přednášejícím.
- Vidí uživatelsky přívětivý rozpis svých přednášek.
- Pořadatel se může přihlásit na svou konferenci jako přednášející.

### Role návštěvníka

- Rezervuje **1 až N vstupenek**  
  - Je nutné zvolit vhodné omezení (např. maximální množství).  
  - Úhrada musí proběhnout ve stanoveném intervalu (kontroluje pořadatel).  
- Sleduje stav svých rezervací (provedení úhrady).
- Vidí uživatelsky přívětivý rozpis přednášek konference.
- Může si vytvářet osobní rozvrh.

---

## Neregistrovaný uživatel

- Vidí dostupné konference a jejich obsah.
- Vidí, které konference jsou vyprodané.
- V případě souběžných rezervací:
  - Pokud dojde k překročení kapacity,
  - Pomalejší uživatel je upozorněn, že registrace nebyla provedena.
- Může provést rezervaci **1 až N vstupenek bez registrace**:
  - Je nutné zadat vhodné údaje.
  - Má možnost dokončit registraci a stát se návštěvníkem konference.

---

# Náměty na rozšíření

- Možnost vložení otázek k dané přednášce.
- Hlasování o nejlepší přednášku.

---

# Doplnění

Potvrzení rezervace pořadatelem slouží pouze pro **simulaci potvrzení platby**.

Implementace skutečných plateb (např. přes platební brány) **není součástí zadání**.

Potvrzení rezervace může proběhnout kdykoliv:
- od registrace
- až po zahájení události