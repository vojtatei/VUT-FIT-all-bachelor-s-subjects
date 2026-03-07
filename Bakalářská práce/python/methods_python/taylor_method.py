import numpy as np

def taylor_method(A, b, y0, h, t_final, EPS=1e-6, *args):
    # Vytvoření časové osy
    t = np.arange(0, t_final + h/2, h)
    num_steps = len(t)
    
    # Inicializace řešení
    y_taylor = np.zeros((len(y0), num_steps))
    y_taylor[:, 0] = y0
    
    max_terms = 60
    
    # Diagnostické proměnné
    terms_used = []
    max_errors = []
    
    print(f"Parametry: h={h}, EPS={EPS}, t_final={t_final}")
    
    # Hlavní výpočetní smyčka
    for i in range(num_steps - 1):
        # Inicializace součtu Taylorovy řady současným stavem
        y_current = y_taylor[:, i].copy()
        
        # Vyhodnocení b v aktuálním čase
        if callable(b):
            b_val = b(t[i])
        else:
            b_val = b
        
        DY = A @ y_current + b_val
        
        Tay_sum = y_current.copy()
        
        term = h * DY
        Tay_sum += term

        j = 1
        terms_magnitude = [np.linalg.norm(term, np.inf)]
        
        # Výpočet dalších členů řady
        while True:
            j += 1
            
            DY = A @ DY
            
            term = (h**j) / np.math.factorial(j) * DY
            terms_magnitude.append(np.linalg.norm(term, np.inf))
            
            Tay_sum += term
            
            rel_error = np.linalg.norm(term, np.inf) / np.linalg.norm(Tay_sum, np.inf)
            
            
            if rel_error < EPS:
                break
                
            if j > max_terms:
                print(f"Varování: Dosaženo maximálního počtu členů ({max_terms}) v kroku {i}")
                break
        
        # Uložení počtu použitých členů a maximální chyby
        terms_used.append(j)
        max_errors.append(np.max(terms_magnitude))
        
        # Uložení výsledku
        y_taylor[:, i+1] = Tay_sum

    
    # Souhrnná diagnostika
    print(f"\nSouhrn:")
    print(f"Průměrný počet členů: {np.mean(terms_used):.2f}")
    print(f"Maximální počet členů: {np.max(terms_used)}")
    print(f"Minimální počet členů: {np.min(terms_used)}")
    print(f"Maximální velikost členu: {np.max(max_errors):.3e}")
    
    return t, y_taylor