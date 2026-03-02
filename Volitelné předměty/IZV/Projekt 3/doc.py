#!/usr/bin/env python3
# coding=utf-8

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

def parse_time(p2b):
    """Parses custom time format (HHMM)."""
    try:
        p2b = int(p2b)
        hours = p2b // 100
        minutes = p2b % 100
        if 0 <= hours <= 23 and 0 <= minutes <= 59:
            return hours + minutes / 60.0
        else:
            return np.nan
    except ValueError:
        return np.nan

def main():
    # 1) Načtení základních souborů
    df_accidents = pd.read_pickle("accidents.pkl.gz")
    df_conseq = pd.read_pickle("consequences.pkl.gz")

    # 2) Sloučení (merge) na klíči p1, abychom měli i sloupce p13a/p13b/p13c/p14*100
    df_merged = pd.merge(df_accidents, df_conseq, on="p1", how="left")

    # Ověření, že máme potřebné sloupce
    for col in ["p9", "p13a", "p13b", "p13c", "p14*100", "p36", "p2b", "region"]:
        if col not in df_merged.columns:
            print(f"[VAROVÁNÍ] Sloupec {col} nebyl nalezen ve sloučených datech!")

    # 3) Vyhodíme řádky, kde pro klíčové sloupce chybí data
    df_merged_clean = df_merged.dropna(
        subset=["p9","p13a","p13b","p13c","p14*100","p2b","region"]
    )

    # Převod času z p2b
    df_merged_clean["parsed_time"] = df_merged_clean["p2b"].apply(parse_time)

    # Vyhození nevalidních časů
    df_merged_clean = df_merged_clean.dropna(subset=["parsed_time"])

    # 4) Získáme pár statistik
    total_accidents = len(df_merged_clean)
    sum_dead   = df_merged_clean["p13a"].sum()  # usmrcených
    sum_heavy  = df_merged_clean["p13b"].sum()  # těžce zraněných
    sum_light  = df_merged_clean["p13c"].sum()  # lehce zraněných

    # p9=1 => nehoda s následky, p9=2 => jen škoda
    accidents_injury = (df_merged_clean["p9"] == 1).sum()
    df_merged_clean["p14_kc"] = df_merged_clean["p14*100"] /100
    avg_damage = df_merged_clean["p14_kc"].mean()  # průměrná hmotná škoda v Kč

    # 5) Graf 1: Počet nehod podle času dne
    time_counts = df_merged_clean["parsed_time"].value_counts(bins=np.arange(0, 24.1, 1), sort=False)
    plt.figure(figsize=(10, 5))
    plt.bar(time_counts.index.mid, time_counts.values, width=0.8, color="steelblue", edgecolor="black", linewidth=1)
    plt.xlabel("Čas dne (hodiny)")
    plt.ylabel("Počet nehod")
    plt.title("Nehody podle času dne (p9=1/2)")
    plt.xticks(np.arange(0, 24, 1))
    plt.grid(axis="y", linestyle="--", alpha=0.7)  # Přidání mřížky na osu Y
    plt.tight_layout()
    plt.savefig("fig1.svg")
    plt.close()

    # 6) Graf 2: Podíl nehod p9=1 vs. p9=2
    p9_counts = df_merged_clean["p9"].value_counts()
    labels = ["S následky (p9=1)", "Pouze škoda (p9=2)"] if len(p9_counts) == 2 else p9_counts.index

    plt.figure(figsize=(5, 5))
    plt.pie(p9_counts, labels=labels, autopct="%1.1f%%")
    plt.title("Podíl nehod podle p9")
    plt.savefig("fig2.svg")
    plt.close()

    # 7) Tabulka: TOP 5 krajů podle p13a (usmrcených)
    top_region_dead = (
        df_merged_clean
        .groupby("region")["p13a"]
        .sum()
        .sort_values(ascending=False)
        .head(5)
    )

    # Výpis do konzole
    print("Tabulka: TOP 5 krajů podle usmrcených (p13a)")
    print("region\tp13a_sum")
    for r, val in top_region_dead.items():
        print(f"{r}\t{val}")

    print("\n[INFO] Vypočtené hodnoty:")
    print(f"Počet nehod (po merge):\t{total_accidents}")
    print(f"Celkem usmrcených:\t{sum_dead}")
    print(f"Celkem těžce zraněných:\t{sum_heavy}")
    print(f"Celkem lehce zraněných:\t{sum_light}")
    print(f"Nehod s následky (p9=1):\t{accidents_injury}")
    print(f"Průměrná škoda (p14_kc) [Kč]:\t{int(avg_damage) if not np.isnan(avg_damage) else 0}")
    print("Průměrná škoda (p14*100):", df_merged_clean["p14*100"].mean() / 100)
    print("Medián škody (p14*100):", df_merged_clean["p14*100"].median() /100)
    print("Maximální škoda (p14*100):", df_merged_clean["p14*100"].max() / 100)


    # 8) Vygenerování doc.html (2 grafy, 1 tabulka, min. 10 vět textu)
    text_body = f"""
<p>
V tomto dokumentu se věnujeme dopravním nehodám, které jsme získali sloučením
statistických dat. Analýza zahrnuje informace o příčinách a následcích nehod.
Celkem evidujeme {total_accidents} dopravních nehod. Z nich bylo {sum_dead} osob usmrceno,
{sum_heavy} těžce zraněných a {sum_light} lehce zraněných.
</p>
<p>
Průměrná škoda na majetku činila {int(avg_damage)} Kč. Nejčastější čas, kdy dochází k nehodám,
je odpoledne mezi 14. a 17. hodinou. Tato doba odpovídá období největšího provozu na komunikacích.
Grafy ilustrují podíl nehod s následky oproti těm s pouze hmotnou škodou
(p9=2) a ukazují počet nehod v průběhu dne. 
</p>
<p>
Tabulka zahrnuje pět krajů s nejvyšším počtem usmrcených osob.
Tento pohled může sloužit jako podklad pro preventivní opatření v oblastech
s vysokou intenzitou tragických nehod. Analýza nabízí pohled na dopravní nehody
z různých úhlů, včetně finančních dopadů a časových souvislostí.
</p>
    """

    html_out = f"""
<html>
<head>
    <meta charset="UTF-8">
    <title>Infografika - Nehody p9, p13a,b,c, p14*100</title>
</head>
<body>
<h1>Dopravní nehody - sloučené statistiky</h1>

{text_body}

<h2>Graf 1: Počet nehod podle času dne</h2>
<img src="fig1.svg" width="600" alt="Graf1 hodina"/>

<h2>Graf 2: Podíl nehod (p9=1) vs (p9=2)</h2>
<img src="fig2.svg" width="400" alt="Graf2 p9 piechart"/>

<h2>Tabulka: TOP 5 krajů podle usmrcených (p13a)</h2>
<table border="1" cellpadding="4" cellspacing="0">
<tr><th>Region</th><th>Usmrceno</th></tr>
"""
    # Vložení řádků s kraji
    for r, val in top_region_dead.items():
        html_out += f"<tr><td>{r}</td><td>{val}</td></tr>\n"

    html_out += """
</table>
</body>
</html>
"""

    with open("doc.html", "w", encoding="utf-8") as f:
        f.write(html_out)

if __name__ == "__main__":
    main()
