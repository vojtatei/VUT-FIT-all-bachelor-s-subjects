import numpy as np

def euler_method(A, b, y0, h, t_final, *args):

    # Vytvoření časové osy
    t = np.arange(0, t_final + h/2, h)  # +h/2 pro případ zaokrouhlovacích chyb
    num_steps = len(t)
    
    # Inicializace řešení
    y_euler = np.zeros((len(y0), num_steps))
    y_euler[:, 0] = y0
    
    # Hlavní výpočetní smyčka
    for i in range(num_steps - 1):
        # Vyhodnocení b v aktuálním čase, pokud b je funkce
        if callable(b):
            b_val = b(t[i])
        else:
            b_val = b
        
        # Eulerův krok: y_{n+1} = y_n + h * (A * y_n + b)
        y_euler[:, i+1] = y_euler[:, i] + h * (A @ y_euler[:, i] + b_val)
    
    return t, y_euler