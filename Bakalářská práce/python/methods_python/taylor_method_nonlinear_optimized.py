import numpy as np
from numba import njit, prange

@njit
def compute_step(y_curr, A, B1, b, h, EPS, max_terms, i_indices, j_indices, dim):
    """
    Jit-kompilovaná funkce pro výpočet jednoho kroku Taylorovy metody.
    """
    # Alokace pro všechny derivace (Taylorův polynom)
    DY = np.zeros((dim, max_terms + 1))
    DY[:, 0] = y_curr
    
    # První derivace: A*y + B1*(y_i*y_j) + b
    # Lineární část: A*y
    dy_linear = A @ y_curr
    
    # Kvadratické členy: B1 * (y_i * y_j)
    quad_products = np.zeros(len(i_indices))
    for idx in range(len(i_indices)):
        quad_products[idx] = y_curr[i_indices[idx]] * y_curr[j_indices[idx]]
    
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
        for idx in range(len(i_indices)):
            for k in range(n+1):
                prods[idx] += DY[i_indices[idx], k] * DY[j_indices[idx], n-k]
        
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
    
    return y_new, n+1

def taylor_method_nonlinear_optimized(A, B1, b, y_ij, y0, h, t_final, EPS=1e-6, max_terms=60):

    # Pre-kompilace pro zahřátí JIT
    _ = compute_step(y0, A, B1, b, h, EPS, 10, 
                    np.array(y_ij[:, 0], dtype=np.int64), 
                    np.array(y_ij[:, 1], dtype=np.int64), 
                    len(y0))
    
    # Vytvoření časové osy - předalokace pro rychlost
    num_steps = int(np.ceil(t_final / h)) + 1
    t = np.linspace(0, t_final, num_steps)
    
    # Inicializace řešení - předalokace
    dim = len(y0)
    y_taylor = np.zeros((dim, num_steps), dtype=np.float64)
    y_taylor[:, 0] = y0
    
    # Inicializace počtu členů řady pro každý krok
    ord_values = np.zeros(num_steps, dtype=np.int64)
    
    # Extrakce indexů pro kvadratické členy a převod na 0-based indexování
    i_indices = np.array(y_ij[:, 0], dtype=np.int64)
    j_indices = np.array(y_ij[:, 1], dtype=np.int64)
    
    # Kontrola pro 1-based indexování
    if np.min(i_indices) == 1 and np.min(j_indices) == 1:
        i_indices -= 1
        j_indices -= 1
    
    # Konverze matic pro efektivní využití v numba funkci
    A_jit = np.ascontiguousarray(A)
    B1_jit = np.ascontiguousarray(B1)
    b_jit = np.ascontiguousarray(b)
    
    # Hlavní smyčka integrace
    for step in range(num_steps - 1):
        # Aktuální hodnota y
        y_curr = y_taylor[:, step]
        
        # Výpočet dalšího kroku pomocí JIT-kompilované funkce
        y_new, n_terms = compute_step(
            y_curr, A_jit, B1_jit, b_jit, h, EPS, 
            max_terms, i_indices, j_indices, dim
        )
        
        # Uložení výsledku a počtu použitých členů
        y_taylor[:, step+1] = y_new
        ord_values[step+1] = n_terms
    
    return t, y_taylor, ord_values