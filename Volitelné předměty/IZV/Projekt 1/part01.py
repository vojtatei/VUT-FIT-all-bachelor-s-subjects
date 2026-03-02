#!/usr/bin/env python3
"""
IZV cast1 projektu
Autor: Vojtěch Teichmann (xteich02)

Detailni zadani projektu je v samostatnem projektu e-learningu.
Nezapomente na to, ze python soubory maji dane formatovani.

Muzete pouzit libovolnou vestavenou knihovnu a knihovny predstavene na prednasce
"""
from bs4 import BeautifulSoup
import requests
import numpy as np
import matplotlib.ticker as ticker
from numpy.typing import NDArray
import matplotlib.pyplot as plt
from typing import List, Callable, Dict, Any


def distance(a: np.array, b: np.array) -> np.array:

    """
    Vypočítá euklidovskou vzdálenost mezi odpovídajícími body ve dvou polích.

    Parameters:
    - a (np.array): Pole bodů jako np.array.
    - b (np.array): Pole bodů jako np.array.

    Returns:
    - np.array: Pole vzdáleností mezi body v a a b.
    """

    return np.sqrt(np.sum((a - b) ** 2, axis=1))


def generate_graph(a: List[float], show_figure: bool = False, save_path: str = None) -> None:
    """
    Generuje grafy pro funkci a^2 * sin(x) pro každou hodnotu v a.

    Parameters:
    - a (List[float]): Seznam koeficientů, které se použijí k vytvoření funkcí.
    - show_figure (bool): Pokud True, zobrazí graf po vygenerování. Default je False.
    - save_path (str): Cesta k uložení grafu. Pokud není uvedeno, graf se neuloží.

    Returns:
    - None
    """
    x = np.linspace(0, 6 * np.pi, 1000) 
    results = np.array([a_val**2 * np.sin(x) for a_val in a])
    
    plt.figure(figsize=(10, 6))
    

    x = x[x <= 6 * np.pi] 
    for idx, a_val in enumerate(a):
        y = results[idx][:len(x)]  
        plt.plot(x, y, label=fr'$\gamma_{{{a_val}}}(x)$')
        plt.fill_between(x, y, alpha=0.2)
    

    plt.xticks([0, np.pi/2, np.pi, 3*np.pi/2, 2*np.pi, 5*np.pi/2, 3*np.pi, 7*np.pi/2, 4*np.pi,  9*np.pi/2, 5*np.pi, 11*np.pi/2, 6*np.pi], 
               ['$0$', r'$\frac{\pi}{2}$', r'$\pi$', r'$\frac{3\pi}{2}$', r'$2\pi$', r'$\frac{5\pi}{2}$', r'$3\pi$' , r'$\frac{7\pi}{2}$', r'$4\pi$', r'$\frac{9\pi}{2}$', r'$5\pi$', r'$\frac{11\pi}{2}$', r'$6\pi$'])
    
    plt.xlim(0, 6 * np.pi)
    plt.xlabel(r'$x$', fontsize=12)
    plt.ylabel(r'$f_a(x)$', fontsize=12)
    plt.legend(fontsize=12, loc='upper center', bbox_to_anchor=(0.5, 1.15), ncol=3)
    plt.grid(False)
    
    if save_path:
        plt.savefig(save_path)
    if show_figure:
        plt.show()
    plt.close()


def generate_sinus(show_figure: bool = False, save_path: str | None = None):

    """
    Generuje graf dvou funkcí f1 a f2 a jejich součtu f_sum, přičemž
    barví oblasti grafu podle toho, zda je f_sum nad nebo pod f1.

    Parameters:
    - show_figure (bool): Pokud True, zobrazí graf po vygenerování. Default je False.
    - save_path (str): Cesta k uložení grafu. Pokud není uvedeno, graf se neuloží.

    Returns:
    - None
    """
    
    t = np.linspace(0, 100, 1000)
    f1 = 0.5 * np.cos(1/50 * np.pi * t)
    f2 = 0.25 * (np.sin(np.pi * t) + np.sin((3/2) * np.pi * t))
    f_sum = f1 + f2

    fig, axs = plt.subplots(3, 1, sharex=True, figsize=(10, 8))

    # Graf f1
    axs[0].plot(t, f1, color='blue')
    axs[0].set_ylabel(r'$f_1(t)$', fontsize=12, labelpad=20)
    axs[0].set_ylim(-0.8, 0.8)

    # Graf f2
    axs[1].plot(t, f2, color='blue')
    axs[1].set_ylabel(r'$f_2(t)$', fontsize=12, labelpad=20)
    axs[1].set_ylim(-0.8, 0.8)

    # Vykreslení barevných oblastí pro f_sum v závislosti na f1
    axs[2].fill_between(t[t < 50], f1[t < 50], f_sum[t < 50], where=f_sum[t < 50] > f1[t < 50], color='green', alpha=0.6)
    axs[2].fill_between(t[t < 50], f1[t < 50], f_sum[t < 50], where=f_sum[t < 50] < f1[t < 50], color='red', alpha=0.6)
    axs[2].fill_between(t[t >= 50], f1[t >= 50], f_sum[t >= 50], where=f_sum[t >= 50] > f1[t >= 50], color='green', alpha=0.6)
    axs[2].fill_between(t[t >= 50], f1[t >= 50], f_sum[t >= 50], where=f_sum[t >= 50] < f1[t >= 50], color='orange', alpha=0.6)

    axs[2].set_ylabel(r'$f_1(t) + f_2(t)$', fontsize=12, labelpad=20)
    axs[2].set_xlabel('t', fontsize=12)
    axs[2].set_ylim(-0.8, 0.8)

    for ax in axs:
        ax.yaxis.set_major_locator(ticker.MultipleLocator(0.4))
        ax.grid(False)

    axs[2].set_xlim(0, 100)

    fig.suptitle('Vizualizace sinusových signálů')

    if save_path:
        plt.savefig(save_path)
    if show_figure:
        plt.show()
    plt.close()




def clean_value(value: str) -> str:
    """
    Vyčistí zadanou hodnotu odstraněním zbytečných znaků a konvertuje čárku na tečku.

    Parameters:
    - value (str): Hodnota jako text.

    Returns:
    - str: Vyčištěná hodnota.
    """
 
    return value.replace(",", ".").translate(str.maketrans('', '', '°() '))


def download_data() -> Dict[str, List[Any]]:
    """
    Stáhne geografická data ze stránky a uloží je do slovníku.

    Returns:
    - Dict[str, List[Any]]: Slovník obsahující polohy, zeměpisné šířky a délky a výšky.
    """

    url = "https://ehw.fit.vutbr.cz/izv/st_zemepis_cz.html"
    response = requests.get(url)


    soup = BeautifulSoup(response.content, "html.parser")
    data = {"positions": [], "lats": [], "longs": [], "heights": []}

    # Najdeme všechny tabulky na stránce
    tables = soup.find_all("table")
    
    if len(tables) < 2:
        print("Tabulka nebyla nalezena nebo není dostatek tabulek.")
        return data

    # Použijeme druhou tabulku
    table = tables[1]

    # Získáme všechny řádky tabulky
    rows = table.find_all("tr")

    for idx, row in enumerate(rows[1:]):
        cols = row.find_all("td")
        if len(cols) == 7:  
            data["positions"].append(cols[0].text.strip())
            
            try:
                lat_str = clean_value(cols[2].text)
                long_str = clean_value(cols[4].text)
                height_str = clean_value(cols[6].text)

                lat = round(float(lat_str), 4)
                long = round(float(long_str), 4)
                height = float(height_str)

                data["lats"].append(lat)
                data["longs"].append(long)
                data["heights"].append(height)
            except ValueError as e:
                print(f"Chyba při konverzi na float v řádku {idx + 2}: {e}")
        else:
            print(f"Řádek {idx + 2} nemá 7 sloupců. Počet sloupců: {len(cols)}")

    return data

if __name__ == "main":
    generate_graph([7, 4, 3])
    generate_sinus()
