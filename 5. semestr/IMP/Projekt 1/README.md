# Zadání projektu – WiFi metronom (ESP32)

## Základní informace

- Cílem je vytvořit zařízení s bzučákem, které bude vytvářet metronom pro hudebníky.
- Desku ESP32 s periferiemi si vyzvednete u vedoucího (před tím vám pošle e-mail).
- Požadavky na výslednou aplikaci jsou uvedeny níže.

---

## Požadavky na implementaci

1. K programování můžete využít prostředí **Arduino** nebo **ESP-IDF**.  
   Na hodnocení se bude promítat i komplexnost řešení – pokud použijete Arduino, očekává se, že pro získání plného počtu bodů se budete snažit aplikaci co nejvíce „dotáhnout“.

2. Bude nastavitelná úroveň hlasitosti (PWM výstup).

3. Nastavení metronomu (rychlost, hlasitost a rytmus) bude probíhat přes webové rozhraní.

4. Ovládací stránku bude poskytovat přímo vestavěné zařízení.  
   Pokuste se doplnit stylování (zejména pro mobilní zařízení) a např. obrázek.

5. Vlastní spínání bzučáku realizujte pomocí vnitřního časovače.

---

## Dokumentace a odevzdání

- Vytvořte přehlednou dokumentaci k přípravě, způsobu realizace, funkčnosti a vlastnostem řešení projektu.
- Řešení (projekt bez binárních souborů sestavitelných na základě zdrojových souborů v projektu, dokumentace ve zdrojové i binární formě – tj. PDF) odevzdávejte prostřednictvím IS v jednom ZIP archivu.
- Předvedení řešení se předpokládá:
  - buď v osobní podobě s následným vrácením zapůjčeného vybavení,
  - nebo odevzdáním videa (např. YouTube, Google Drive apod.).
- V případě nejasností můžete být vyzváni k osobní prezentaci řešení.
- Zařízení po prezentaci vrátíte vedoucímu.

---

## Hodnocení

Hodnocení projektu bude provedeno na základě:

- dokumentace (4 body),
- funkčnosti (5 bodů),
- prezentace (1 bod),
- kvality řešení (4 body).

### Poznámky k hodnocení

- U nefunkčního řešení nemůže být plné hodnocení dokumentace, pokud zjevně nepopisuje reálný stav věci.
- V rámci kvality řešení se hodnotí:
  - zda řešení splňuje minimální požadavky,
  - zda nejde pouze o volání malého množství knihovních funkcí bez vlastního přínosu,
  - zda je řešení promyšlené (ošetření chyb, návrh ovládání, případná rozšíření).