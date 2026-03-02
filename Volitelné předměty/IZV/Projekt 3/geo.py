#!/usr/bin/python3.10
# coding=utf-8
# %%%
import pandas as pd
import geopandas
import matplotlib.pyplot as plt
import contextily as ctx
import sklearn.cluster
import numpy as np
from shapely.geometry import Point
from sklearn.cluster import KMeans


def make_geo(df_accidents: pd.DataFrame, df_locations: pd.DataFrame) -> geopandas.GeoDataFrame:
    
    """
    Vytvoří GeoDataFrame ze vstupních DataFrame tak, že:
      - Spojí data o nehodách (df_accidents) a jejich lokalitách (df_locations) podle společného klíče (p1).
      - Vynechá záznamy s neplatnými souřadnicemi (d == -1 nebo e == -1).
      - Pokud je d < e, prohodí je (záměna X a Y).
      - Nastaví CRS = EPSG:5514 (S-JTSK).
      - Volitelně může zkontrolovat, zda výsledné body leží v rozumném rozsahu pro ČR.
    """

    coords = []
    for d, e in zip(df_locations["d"], df_locations["e"]):
        # 1) Odfiltrovat neznámé souřadnice
        if d == -1 or e == -1:
            coords.append(None)
            continue

        # 2) Prohodit d,e, pokud d < e
        if d < e:
            d, e = e, d

        # 3) Vytvořit Point
        coords.append(Point(d, e))

    gdf_locations = geopandas.GeoDataFrame(
        df_locations.copy(),
        geometry=coords,
        crs="EPSG:5514"  # S-JTSK
    )

    # Smažeme řádky, které mají None v geometry (tj. nepoužitelné souřadnice)
    gdf_locations = gdf_locations.dropna(subset=["geometry"])

    # Sloučíme
    merged = pd.merge(df_accidents, gdf_locations, on="p1", how="inner")

    # Uděláme GeoDataFrame
    gdf = geopandas.GeoDataFrame(merged, geometry="geometry", crs="EPSG:5514")

    return gdf

def plot_geo(gdf: geopandas.GeoDataFrame, fig_location: str = None,
             show_figure: bool = False):
    """
    Vykreslí dva podgrafy s nehodami, které byly zaviněné pod vlivem alkoholu (p11 >= 4),
    a to pro Leden (1) a Červenec (7). Mapa je v S-JTSK, ale kvůli správnému
    zobrazení v OpenStreetMap se data převádí do EPSG:3857.
    """
    if not pd.api.types.is_datetime64_any_dtype(gdf["date"]):
        gdf["date"] = pd.to_datetime(gdf["date"], errors="coerce")

    n_before = len(gdf)
    gdf = gdf[gdf["region"] == "JHM"]

    # Filtr - nehody pod vlivem alkoholu
    gdf_alcohol = gdf[gdf["p11"] >= 4]

    # Měsíce: Leden(1), Červenec(7)
    gdf_january = gdf_alcohol[gdf_alcohol["date"].dt.month == 1]
    gdf_july    = gdf_alcohol[gdf_alcohol["date"].dt.month == 7]

    gdf_january_3857 = gdf_january.to_crs(epsg=3857)
    gdf_july_3857    = gdf_july.to_crs(epsg=3857)

    fig, axes = plt.subplots(1, 2, figsize=(15, 7))

    # Leden
    gdf_january_3857.plot(ax=axes[0], markersize=5, color='red', alpha=0.7)
    ctx.add_basemap(axes[0], crs="EPSG:3857")
    axes[0].set_title("JHM kraj - pod vlivem alkoholu (Leden)")
    axes[0].axis('off')

    # Červenec
    gdf_july_3857.plot(ax=axes[1], markersize=5, color='blue', alpha=0.7)
    ctx.add_basemap(axes[1], crs="EPSG:3857")
    axes[1].set_title("JHM kraj - pod vlivem alkoholu (Červenec)")
    axes[1].axis('off')

    if fig_location:
        plt.savefig(fig_location, dpi=300)

    if show_figure:
        plt.show()

    plt.close(fig)




def plot_cluster(gdf: geopandas.GeoDataFrame, fig_location: str = None,
                 show_figure: bool = False):
    """
    Vykreslí všechny nehody vybraného kraje (zde 'JHM'), které byly zaviněny zvěří.
    Použije shlukovací algoritmus (KMeans) pro zadaný počet clusterů.
    Každý cluster vykreslí jako konvexní polygon s poloprůhlednou výplní, 
    obarvenou podle počtu nehod v daném shluku.
    Dovnitř polygonů zároveň vykreslí pozice bodů (nehod).
    """


    # 1) Vybereme Jihomoravský kraj
    gdf_jhm = gdf[gdf["region"] == "JHM"]

    # 2) Filtr na nehody zaviněné zvěří
    gdf_animal = gdf_jhm[gdf_jhm["p10"] == 4]

    if len(gdf_animal) == 0:
        return

    # 3) Příprava pro shlukování:
    #    -> vezmeme souřadnice x,y a provedeme KMeans
    coords = np.column_stack([gdf_animal.geometry.x, gdf_animal.geometry.y])
    n_clusters = 7 
    kmeans = KMeans(n_clusters=n_clusters, random_state=42, n_init='auto')
    labels = kmeans.fit_predict(coords)

    gdf_animal = gdf_animal.copy()
    gdf_animal["cluster"] = labels

    # 4) Z každého shluku vytvoříme konvexní obálku (convex hull)
    #    a zjistíme, kolik nehod je ve shluku.
    clusters_polygons = []
    for cluster_label, group in gdf_animal.groupby("cluster"):
        hull = group.geometry.union_all().convex_hull
        count_accidents = len(group)
        clusters_polygons.append([cluster_label, count_accidents, hull])

    # 5) Vytvoříme GeoDataFrame s polygonovými shluky
    polygons_gdf = geopandas.GeoDataFrame(
        clusters_polygons,
        columns=["cluster", "count", "geometry"],
        crs=gdf_animal.crs
    )

    # 6) Převedeme polygony i body do EPSG:3857 pro vykreslení na OSM
    polygons_3857 = polygons_gdf.to_crs(epsg=3857)
    points_3857   = gdf_animal.to_crs(epsg=3857)

    # 7) Vykreslení
    fig, ax = plt.subplots(1, 1, figsize=(10, 8))

    # a) Nejprve polygonové shluky s alfa průhledností
    #    - Obarvíme podle sloupce "count" => tzv. thematic map
    polygons_3857.plot(
        ax=ax,
        column="count",
        cmap="coolwarm",      
        legend=True,           
        alpha=0.4,             
        edgecolor="black"
    )

    # b) Body (nehody)
    points_3857.plot(
        ax=ax,
        color="red",
        markersize=5
    )

    # c) Podkladová mapa
    ctx.add_basemap(ax, crs="EPSG:3857", source=ctx.providers.OpenStreetMap.France)

    # d) Popisy
    ax.set_title("Nehody v JHM kraji zaviněné lesní zvěří (shluky)")
    ax.axis('off')

    # e) Uložení / zobrazení
    if fig_location:
        plt.savefig(fig_location, dpi=300)

    if show_figure:
        plt.show()

    plt.close(fig)

if __name__ == "__main__":
    # zde muzete delat libovolne modifikace
    df_accidents = pd.read_pickle("accidents.pkl.gz")
    df_locations = pd.read_pickle("locations.pkl.gz")


    if "date" in df_accidents.columns:
        if not pd.api.types.is_datetime64_any_dtype(df_accidents["date"]):
            df_accidents["date"] = pd.to_datetime(df_accidents["date"], errors="coerce")

    # Tvorba GeoDataFrame
    gdf = make_geo(df_accidents, df_locations)

    # Vykreslení
    plot_geo(gdf, fig_location="geo1.png", show_figure=True)
    plot_cluster(gdf, "geo2.png", True)

    # testovani splneni zadani
    import os
    assert os.path.exists("geo1.png")
    assert os.path.exists("geo2.png")