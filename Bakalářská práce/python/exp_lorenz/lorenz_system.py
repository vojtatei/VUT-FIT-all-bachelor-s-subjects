import numpy as np
import time
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp
from tabulate import tabulate
import argparse
import os
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

parser = argparse.ArgumentParser(description='Lorenzův systém testovací skript')
parser.add_argument('-v', '--version', choices=['optimized', 'notoptimized'],
                    help='Vyberte verzi Taylorovy metody (optimized nebo notoptimized)')
args = parser.parse_args()

# Kontrola, zda byl zadán argument -v
if args.version is None:
    print("❌ Nebyla zadána žádná verze Taylorovy metody.")
    print("Použití: python3 lorenz_system.py -v [optimized|notoptimized]")
    sys.exit(1)  

# Import Taylorovy metody na základě argumentu
if args.version == 'optimized':
    from methods_python.taylor_method_nonlinear_optimized import taylor_method_nonlinear_optimized as taylor_method
elif args.version == 'notoptimized':
    from methods_python.taylor_method_nonlinear import taylor_method_nonlinear_optimized as taylor_method

def lorenz_test(rho_values=[28, 160, 23.7], target_steps=[2000, 4000, 500], t_final=100):
    """
    Test řešení Lorenzova systému pomocí optimalizované Taylorovy metody
    a vestavěných řešičů ze SciPy.
    """
    # Definice parametrů Lorenzova systému
    sigma = 10          # Prandtlovo číslo
    beta = 8/3          # Parametr související s fyzikální velikostí systému
    
    # Nastavení tolerance
    EPS = 1e-10
    options = {'rtol': 1e-10, 'atol': 1e-10}
    
    # Simulace pro každou hodnotu rho
    for idx, rho in enumerate(rho_values):
        print(f"\n\nSimulace pro rho = {rho}")
        
        # Nastavení kroku pro vizualizaci
        if rho == 23.7:
            h_plot = 0.001
        else:
            h_plot = 0.005
        
        # Krok pro měření
        steps = target_steps[idx]
        h_calc = t_final / steps
        
        # Nastavení lineární části matice A pro aktuální rho
        A = np.array([
            [-sigma, sigma, 0],
            [rho, -1, 0],
            [0, 0, -beta]
        ], dtype=np.float64)
        
        # Matice B1 pro kvadratické členy
        B1 = np.array([
            [0, 0],
            [-1, 0],
            [0, 1]
        ], dtype=np.float64)
        
        # Indexy pro kvadratické členy
        y_ij = np.array([
            [0, 2],  # x*z
            [0, 1]   # x*y
        ], dtype=np.int64)
        
        # Konstantní člen
        b = np.array([0, 0, 0], dtype=np.float64)
        
        # Počáteční podmínka
        v = np.array([0, 2, 0], dtype=np.float64)
        
        if rho == 23.7:
            eq_point = np.array([np.sqrt(beta*(rho-1)), np.sqrt(beta*(rho-1)), rho-1], dtype=np.float64)
            y0 = eq_point + v
        else:
            y0 = np.array([1, 1, 1], dtype=np.float64)
        
        # Definice Lorenzova systému pro SciPy
        def lorenz_func(t, y):
            return [
                sigma * (y[1] - y[0]),
                rho * y[0] - y[1] - y[0] * y[2],
                y[0] * y[1] - beta * y[2]
            ]
        
        # Simulace s jemným krokem pro vykreslování 
        print("Výpočet Taylorovy metody pro vizualizaci")
        t_plot, y_plot, ord_plot = taylor_method(A, B1, b, y_ij, y0, h_plot, t_final, EPS)
        
        # Simulace s krokem pro měření
        print("Výpočet Taylorovy metody pro porovnání")
        start_time = time.time()
        t, y, ord_values = taylor_method(A, B1, b, y_ij, y0, h_calc, t_final, EPS)
        mtsm_time = time.time() - start_time
        mtsm_steps = len(t) - 1
        
        # Vykreslení yz-roviny
        plt.figure(figsize=(10, 8))
        
        if rho == 23.7:
            plt.plot(y_plot[1, :], y_plot[2, :], '-', linewidth=0.2, color='blue', alpha=0.2)
            # Vyznačení rovnovážného bodu
            plt.plot(eq_point[1], eq_point[2], 'w.', markersize=10)
        else:
            plt.plot(y_plot[1, :], y_plot[2, :], '-', color='blue', linewidth=0.3)
        
        plt.title(f'Lorenzův systém v yz-rovině, ρ = {rho}', fontsize=14)
        plt.xlabel('y', fontsize=12)
        plt.ylabel('z', fontsize=12)
        
        # Nastavení rozsahu os podle režimu
        if rho == 28:
            plt.xlim(-25, 25)
            plt.ylim(0, 50)
        elif rho == 160:
            plt.xlim(-100, 100)
            plt.ylim(100, 220)
        else:  # rho = 23.7
            plt.xlim(6, 9.5)
            plt.ylim(20.5, 25)
        
        plt.grid(True)
        plt.savefig(f'lorenz_rho{rho}_yz.png')
        plt.close()  # Uzavření grafu pro uvolnění paměti
        
        # Vykreslení časových průběhů x, y, z
        plt.figure(figsize=(10, 8))
        
        plt.subplot(3, 1, 1)
        plt.plot(t_plot, y_plot[0, :], 'b-', linewidth=0.5)
        plt.title('x(t)', fontsize=12)
        plt.ylabel('x', fontsize=11)
        plt.grid(True)
        plt.ylim(-50, 50)
        plt.xlim(0, 100)
        
        plt.subplot(3, 1, 2)
        plt.plot(t_plot, y_plot[1, :], 'b-', linewidth=0.5)
        plt.title('y(t)', fontsize=12)
        plt.ylabel('y', fontsize=11)
        plt.grid(True)
        plt.ylim(-50, 50)
        plt.xlim(0, 100)
        
        plt.subplot(3, 1, 3)
        plt.plot(t_plot, y_plot[2, :], 'b-', linewidth=0.5)
        plt.title('z(t)', fontsize=12)
        plt.xlabel('t', fontsize=11)
        plt.ylabel('z', fontsize=11)
        plt.grid(True)
        plt.ylim(-50, 50)
        plt.xlim(0, 100)
        
        plt.tight_layout()
        plt.savefig(f'lorenz_rho{rho}_time.png')
        plt.close()  # Uzavření grafu pro uvolnění paměti
        
        # Graf funkce ORD
        plt.figure(figsize=(10, 6))
        plt.plot(t[:len(ord_values)], ord_values, 'r.', markersize=4)
        plt.title(f'Funkce ORD pro ρ = {rho}', fontsize=14)
        plt.xlabel('t', fontsize=12)
        plt.ylabel('ORD', fontsize=12)
        plt.grid(True)
        plt.ylim(0, 60)
        plt.savefig(f'lorenz_rho{rho}_ord.png')
        plt.close()  # Uzavření grafu pro uvolnění paměti
        
        # Měření SciPy solverů
        methods = [
            ('ode23', 'RK23'),
            ('ode45', 'RK45'),
            ('ode78', 'DOP853'),
            ('ode89', 'DOP853'), 
            ('ode113', 'LSODA')
        ]
        
        vysledky = [['Taylor', mtsm_steps, mtsm_time, 1]]
        
        for method_name, scipy_method in methods:
            print(f"Výpočet {method_name} ({scipy_method})")
            start_time = time.time()
            sol = solve_ivp(lorenz_func, [0, t_final], y0, method=scipy_method, **options)
            solve_time = time.time() - start_time
            steps = len(sol.t) - 1
            ratio = solve_time / mtsm_time
            
            vysledky.append([scipy_method, steps, solve_time, ratio])
        
        # Výpis výsledků do konzole
        print(f"\nVýsledky pro rho = {rho}:\n")
        print(tabulate(vysledky, 
                      headers=["Metoda", "Počet kroků", "Čas výpočtu [s]", "Poměr"],
                      floatfmt=".6g"))

if __name__ == "__main__":
    lorenz_test()
