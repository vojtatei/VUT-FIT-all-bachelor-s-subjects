import numpy as np

def rk4_method(A, b, y0, h, t_final, *args):

    # Vytvoření časové osy
    t = np.arange(0, t_final + h/2, h)  # +h/2 pro případ zaokrouhlovacích chyb
    num_steps = len(t)
    
    # Inicializace řešení
    y_rk4 = np.zeros((len(y0), num_steps))
    y_rk4[:, 0] = y0
    
    # Hlavní výpočetní smyčka
    for i in range(num_steps - 1):
        y_curr = y_rk4[:, i]
        
        # Získání hodnoty b pro aktuální a mezikroky
        if callable(b):
            b1 = b(t[i])
            b2 = b(t[i] + 0.5 * h)
            b3 = b(t[i] + 0.5 * h)
            b4 = b(t[i] + h)
        else:
            b1 = b2 = b3 = b4 = b
        
        # Výpočet mezikroků metody RK4
        k1 = A @ y_curr + b1
        k2 = A @ (y_curr + 0.5 * h * k1) + b2
        k3 = A @ (y_curr + 0.5 * h * k2) + b3
        k4 = A @ (y_curr + h * k3) + b4
        
        # Aktualizace řešení
        y_rk4[:, i+1] = y_curr + (h/6) * (k1 + 2*k2 + 2*k3 + k4)
    
    return t, y_rk4