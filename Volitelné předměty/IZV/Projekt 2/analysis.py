#!/usr/bin/env python3.12
# coding=utf-8

from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np
import zipfile

# muzete pridat libovolnou zakladni knihovnu ci knihovnu predstavenou na prednaskach
# dalsi knihovny pak na dotaz

# Ukol 1: nacteni dat ze ZIP souboru


def load_data(filename: str, ds: str) -> pd.DataFrame:
    """
    Načte data ze ZIP souboru, hledá soubory ve složkách '2023' a '2024'.
    
    Args:
        filename (str): Název ZIP souboru.
        ds (str): Název datasetu (např. "nehody").

    Returns:
        pd.DataFrame: DataFrame obsahující načtená data.

    Raises:
        FileNotFoundError: Pokud nejsou nalezeny žádné odpovídající soubory v archivu.
    """
    with zipfile.ZipFile(filename, 'r') as z:
        file_paths = [f"2023/I{ds}.xls", f"2024/I{ds}.xls"]
        dfs = []
        for file_path in file_paths:
            try:
                with z.open(file_path) as file:
                    df = pd.read_html(file, encoding="cp1250")[0]
                    df = df.dropna(axis=1, how="all")
                    dfs.append(df)
            except KeyError:
                print(f"Soubor {file_path} nebyl nalezen v archivu.")
                continue
        if not dfs:
            raise FileNotFoundError(f"Žádný soubor I{ds}.xls nebyl nalezen.")
        return pd.concat(dfs, ignore_index=True)

# Ukol 2: zpracovani dat


def parse_data(df: pd.DataFrame, verbose: bool = False) -> pd.DataFrame:
    """
    Zpracuje data přidáním regionů a převodem dat na vhodné formáty.

    Args:
        df (pd.DataFrame): Vstupní DataFrame.
        verbose (bool): Pokud True, vypíše velikost datového rámce.

    Returns:
        pd.DataFrame: Upravený DataFrame s přidanými sloupci 'date' a 'region'.

    Raises:
        KeyError: Pokud chybí povinné sloupce.
    """
    new_df = df.copy()
    if 'p2a' in new_df.columns:
        new_df['date'] = pd.to_datetime(new_df['p2a'], errors='coerce')
    else:
        raise KeyError("Sloupec 'p2a' nebyl nalezen.")

    region_mapping = {
        0: "PHA", 1: "STC", 2: "JHC", 3: "PLK", 4: "ULK", 5: "HKK", 
        6: "JHM", 7: "MSK", 14: "OLK", 15: "ZLK", 16: "VYS", 17: "PAK", 
        18: "LBK", 19: "KVK"
    }
    if 'p4a' in new_df.columns:
        new_df['region'] = new_df['p4a'].map(region_mapping)
    else:
        raise KeyError("Sloupec 'p4a' nebyl nalezen.")

    if verbose:
        new_size = new_df.memory_usage(deep=True).sum() / 1_000_000
        print(f"new_size = {new_size:.1f} MB")

    return new_df

# Ukol 3: počty nehod v jednotlivých regionech podle stavu vozovky
def plot_state(df: pd.DataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Vykreslí graf počtu nehod podle stavu vozovky v jednotlivých krajích.

    Args:
        df (pd.DataFrame): DataFrame obsahující zpracovaná data.
        fig_location (str, optional): Cesta k uložení grafu. Defaultně None.
        show_figure (bool, optional): Pokud True, zobrazí graf. Defaultně False.
    """
    state_mapping = {
        1: "Povrch suchý",
        2: "Povrch suchý",
        3: "Povrch mokrý",
        4: "Bláto",
        5: "Náledí/ujetý sníh",
        6: "Náledí/ujetý sníh"
    }
    states_order = ["Bláto", "Náledí/ujetý sníh", "Povrch mokrý", "Povrch suchý"]

    df_filtered = df[df["p16"].isin(state_mapping.keys())].copy()
    df_filtered["state"] = df_filtered["p16"].map(state_mapping)
    grouped = df_filtered.groupby(["state", "region"]).size().unstack(fill_value=0)
    grouped = grouped.reindex(states_order)

    fig, axes = plt.subplots(2, 2, figsize=(16, 10), facecolor='#f7f7f7')
    axes = axes.flatten()

    for i, state in enumerate(states_order):
        if state in grouped.index:
            data = grouped.loc[state]
            ax = axes[i]
            data.plot(kind="bar", ax=ax, color=sns.color_palette("muted")[i % 4], alpha=0.9)
            ax.set_title(f"Stav vozovky: {state}", fontsize=14, fontweight="bold")
            ax.set_xlabel("Kraje", fontsize=12)
            ax.set_ylabel("Počet nehod", fontsize=12)
            ax.set_xticklabels(data.index, rotation=45, ha="right", fontsize=10)
            ax.grid(visible=True, linestyle="--", alpha=0.6)
            ax.set_facecolor('#ffffff')

    fig.suptitle("Počet nehod podle povrchu vozovky a krajů", fontsize=18, fontweight="bold")
    fig.tight_layout(pad=4.0)

    if fig_location:
        plt.savefig(fig_location, dpi=300, bbox_inches="tight")
    if show_figure:
        plt.show()
    plt.close(fig)

# Ukol4: alkohol a následky v krajích
def plot_alcohol(df: pd.DataFrame, df_consequences : pd.DataFrame, 
                 fig_location: str = None, show_figure: bool = False):
    
    """
    Vykreslí graf počtu nehod pod vlivem alkoholu v jednotlivých krajích
    podle následků (zranění) a role (řidič/spolujezdec).

    Args:
        df (pd.DataFrame): DataFrame obsahující informace o nehodách.
        df_consequences (pd.DataFrame): DataFrame obsahující následky nehod.
        fig_location (str, optional): Cesta k uložení grafu. Defaultně None.
        show_figure (bool, optional): Pokud True, zobrazí graf. Defaultně False.
    """

    df_filtered = df[df["p11"] >= 3].copy()

    df_merged = df_filtered.merge(df_consequences, on="p1", how="inner")


    injury_mapping = {
        1: "Usmrcení",
        2: "Těžké zranění",
        3: "Lehké zranění",
        4: "Bez zranění"
    }
    df_merged["injury_level"] = df_merged["p59g"].map(injury_mapping)

    df_merged = df_merged.dropna(subset=["injury_level"])

    df_merged["role"] = df_merged["p59a"].apply(lambda x: "Řidič" if x == 1 else "Spolujezdec")


    grouped = df_merged.groupby(["injury_level", "region", "role"]).size().unstack(fill_value=0)

    injury_order = ["Bez zranění", "Lehké zranění", "Těžké zranění", "Usmrcení"]

    fig, axes = plt.subplots(2, 2, figsize=(16, 12), facecolor='#f7f7f7')
    axes = axes.flatten()

    role_colors = {"Řidič": "#1f77b4", "Spolujezdec": "#ff7f0e"}

    for i, injury in enumerate(injury_order):
        ax = axes[i]
        if injury in grouped.index:
            grouped_data = grouped.loc[injury]
            grouped_data.plot(kind="bar", ax=ax, color=[role_colors[col] for col in grouped_data.columns], alpha=0.85)

            ax.set_title(f"Následky nehody: {injury}", fontsize=14, fontweight='bold')
            ax.set_xlabel("Kraje", fontsize=12)
            ax.set_ylabel("Počet nehod pod vlivem", fontsize=12)
            ax.set_xticklabels(grouped_data.index, rotation=45, ha="right", fontsize=10)
            ax.legend(title="Role", fontsize=10)
            ax.grid(visible=True, linestyle="--", alpha=0.6)
            ax.set_facecolor('#ffffff')
        else:
            ax.axis("off")

    fig.suptitle("Počet nehod pod vlivem alkoholu podle následků a role", fontsize=18, fontweight='bold')

    fig.tight_layout(pad=4.0)

    if fig_location:
        plt.savefig(fig_location, dpi=300, bbox_inches="tight")

    if show_figure:
        plt.show()

    plt.close(fig)

# Ukol 5: Druh nehody (srážky) v čase
def plot_type(df: pd.DataFrame, fig_location: str = None,
              show_figure: bool = False):
    type_mapping = {
        1: "s jedoucím nekolejovým vozidlem",
        2: "s vozidlem zaparkovaným, odstaveným",
        3: "s pevnou překážkou",
        4: "s chodcem",
        5: "s lesní zvěří",
        6: "s domácím zvířetem",
        7: "s vlakem",
        8: "s tramvají"
    }

    df_filtered = df[df["p6"].isin(type_mapping.keys())].copy()
    df_filtered["accident_type"] = df_filtered["p6"].map(type_mapping)

    df_filtered["month"] = df_filtered["date"].dt.to_period("M")

    df_filtered = df_filtered[(df_filtered["date"] >= "2023-01-01") & (df_filtered["date"] <= "2024-10-01")]

    grouped = (
        df_filtered.groupby(["month", "region", "accident_type"])
        .size()
        .reset_index(name="count")
    )

    pivot_table = grouped.pivot_table(
        index="month",
        columns=["region", "accident_type"],
        values="count",
        fill_value=0
    )

    selected_regions = ["JHM", "ZLK", "OLK", "MSK"]
    pivot_table = pivot_table.loc[:, pivot_table.columns.get_level_values("region").isin(selected_regions)]

    valid_months = pivot_table.index.to_timestamp()

    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    fig.suptitle("Druh nehody v čase pro vybrané kraje", fontsize=16, fontweight="bold")

    lines, labels = [], []
    for i, region in enumerate(selected_regions):
        ax = axes[i // 2, i % 2]
        region_data = pivot_table.xs(region, axis=1, level="region")

        for col in region_data.columns:
            line, = ax.plot(region_data.index.to_timestamp(), region_data[col], label=col)
            if i == 0:
                lines.append(line)
                labels.append(col)

        ax.set_title(f"Kraj: {region}", fontsize=14, fontweight="bold")
        if i % 2 == 0:
            ax.set_ylabel("Počet nehod", fontsize=12)
        ax.set_xlabel("Měsíc", fontsize=12)
        ax.grid(visible=True, linestyle="--", alpha=0.7)

        ax.set_xlim(valid_months[0], valid_months[-1])
        ax.set_xticks(valid_months)
        ax.set_xticklabels([date.strftime("%m/%y") for date in valid_months], rotation=45, ha="right", fontsize=8)

    for ax in axes.flatten():
        ax.legend().remove()

    fig.legend(lines, labels, loc="lower center", bbox_to_anchor=(0.5, -0.05), ncol=3, fontsize=10, title="Druh nehody")

    if fig_location:
        plt.savefig(fig_location, dpi=300, bbox_inches="tight")
    if show_figure:
        plt.show()

    plt.close(fig)

if __name__ == "__main__":
    # zde je ukazka pouziti, tuto cast muzete modifikovat podle libosti
    # skript nebude pri testovani pousten primo, ale budou volany konkreni
    # funkce.

    try:
        df = pd.read_pickle("nehody.pkl")
        df_consequences = pd.read_pickle("nasledky.pkl")
        print("Data načtena z uložených Pickle souborů.")
    except FileNotFoundError:

        print("Pickle soubory nenalezeny, načítám data z archivu...")
        df = load_data("data_23_24.zip", "nehody")
        df_consequences = load_data("data_23_24.zip", "nasledky")
        df.to_pickle("nehody.pkl")
        df_consequences.to_pickle("nasledky.pkl")
        print("Data byla načtena a uložena do Pickle souborů.")
    df2 = parse_data(df, verbose=True)

    plot_state(df2, "01_state.png")
    plot_alcohol(df2, df_consequences, "02_alcohol.png", False)
    plot_type(df2, "03_type.png")

# Poznamka:
# pro to, abyste se vyhnuli castemu nacitani muzete vyuzit napr
# VS Code a oznaceni jako bunky (radek #%%% )
# Pak muzete data jednou nacist a dale ladit jednotlive funkce
# Pripadne si muzete vysledny dataframe ulozit nekam na disk (pro ladici
# ucely) a nacitat jej naparsovany z disku
