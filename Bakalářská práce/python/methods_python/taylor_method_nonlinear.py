import numpy as np
from numba import njit, prange

def taylor_method_nonlinear_optimized(A, B1, b, y_ij, y0, h, t_final, EPS=1e-6, max_terms=60):
    # Vytvoření časové osy
    t = np.arange(0, t_final + h/2, h)  # +h/2 pro případ zaokrouhlovacích chyb
    num_steps = len(t)

    # Inicializace řešení
    dim = len(y0)
    y_taylor = np.zeros((dim, num_steps))
    y_taylor[:, 0] = y0

    # Inicializace počtu členů řady pro každý krok
    ord_values = np.zeros(num_steps)

    # Extrakce indexů pro kvadratické členy (převod na 0-based indexing pokud jsou 1-based)
    i_indices = np.array(y_ij[:, 0], dtype=int)
    j_indices = np.array(y_ij[:, 1], dtype=int)

    # Kontrola, jestli indexy jsou 0-based nebo 1-based
    if np.min(i_indices) == 1 and np.min(j_indices) == 1:
        i_indices -= 1
        j_indices -= 1

    # Hlavní smyčka integrace
    for step in range(num_steps - 1):
        # Aktuální hodnota y
        y_curr = y_taylor[:, step]

        # Alokace pro všechny derivace (Taylorův polynom)
        DY = np.zeros((dim, max_terms + 1))
        DY[:, 0] = y_curr

        # První derivace: Ay + B1(y_iy_j) + b
        # Lineární část: Ay
        dy_linear = A @ y_curr

        # Kvadratické členy: B1 * (y_i * y_j)
        quad_products = y_curr[i_indices] * y_curr[j_indices]
        dy_quad = B1 @ quad_products

        # Celková první derivace
        DY[:, 1] = h * (dy_linear + dy_quad + b)

        # Inicializace y_new pro Taylorovu řadu
        y_new = y_curr + DY[:, 1]

        # Výpočet vyšších derivací a členů Taylorovy řady
        n = 1
        while n < max_terms - 1:
            # Lineární část
            lin_term = A @ DY[:, n]

            # Kvadratická část (konvoluce)
            prods = np.zeros(len(i_indices))
            for k in range(n+1):
                prods += DY[i_indices, k] * DY[j_indices, n-k]

            quad_term = B1 @ prods

            # Nový člen Taylorovy řady
            DY[:, n+1] = (h/(n+1)) * (lin_term + quad_term)

            # Přidání nového členu do sumy
            y_new = y_new + DY[:, n+1]

            # Kontrola ukončení - pokud je člen dostatečně malý
            if np.linalg.norm(DY[:, n+1], np.inf) < EPS:
                break

            # Přechod na další řád
            n += 1

        # Uložení výsledku a počtu použitých členů
        y_taylor[:, step+1] = y_new
        ord_values[step+1] = n + 1

    return t, y_taylor, ord_values