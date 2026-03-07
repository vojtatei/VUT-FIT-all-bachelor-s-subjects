import numpy as np
import time
import sys
import os
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp
from tabulate import tabulate

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

# Import vlastních metod
from methods_python.taylor_method import taylor_method
from methods_python.euler_method import euler_method
from methods_python.rk4_method import rk4_method

def sin_cos_test(t_final=50, h_taylor=0.1, h_euler=0.01, h_rk4=0.01, EPS=1e-6, tolerance=1e-3):
    # Definice systému
    A = np.array([[0, 1], [-1, 0]])
    b = np.array([0, 0])
    y0 = np.array([0, 1])
    
    # Analytické řešení
    def analytical_solution(t):
        return np.vstack([np.sin(t), np.cos(t)])
    
    # Parametry pro vestavěné solvery
    options = {'rtol': tolerance, 'atol': tolerance}
    
    # Test pro Taylorovu metodu
    print("Výpočet Taylorovy metody")
    tic = time.time()
    t_taylor, y_taylor = taylor_method(A, b, y0, h_taylor, t_final, EPS)
    taylor_time = time.time() - tic
    taylor_steps = len(t_taylor) - 1
    y_ana_taylor = analytical_solution(t_taylor)
    taylor_error = np.max(np.linalg.norm(y_taylor - y_ana_taylor, axis=0))
    
    # Test pro Eulerovu metodu
    print("Výpočet Eulerovy metody")
    tic = time.time()
    t_euler, y_euler = euler_method(A, b, y0, h_euler, t_final)
    euler_time = time.time() - tic
    euler_steps = len(t_euler) - 1
    y_ana_euler = analytical_solution(t_euler)
    euler_error = np.max(np.linalg.norm(y_euler - y_ana_euler, axis=0))
    
    # Test pro RK4 metodu
    print("Výpočet RK4 metody")
    tic = time.time()
    t_rk4, y_rk4 = rk4_method(A, b, y0, h_rk4, t_final)
    rk4_time = time.time() - tic
    rk4_steps = len(t_rk4) - 1
    y_ana_rk4 = analytical_solution(t_rk4)
    rk4_error = np.max(np.linalg.norm(y_rk4 - y_ana_rk4, axis=0))
    
    # Test pro vestavěné solvery v SciPy
    def ode_system(t, y):
        return A @ y + b
    
    # RK23 (ekvivalent ode23 v MATLABu)
    print("Výpočet scipy ode23 (RK23)")
    tic = time.time()
    sol_rk23 = solve_ivp(ode_system, [0, t_final], y0, method='RK23', **options)
    rk23_time = time.time() - tic
    rk23_steps = len(sol_rk23.t) - 1
    y_ana_rk23 = analytical_solution(sol_rk23.t)
    rk23_error = np.max(np.linalg.norm(sol_rk23.y - y_ana_rk23, axis=0))
    
    # RK45 (ekvivalent ode45 v MATLABu)
    print("Výpočet scipy ode45 (RK45)")
    tic = time.time()
    sol_rk45 = solve_ivp(ode_system, [0, t_final], y0, method='RK45', **options)
    rk45_time = time.time() - tic
    rk45_steps = len(sol_rk45.t) - 1
    y_ana_rk45 = analytical_solution(sol_rk45.t)
    rk45_error = np.max(np.linalg.norm(sol_rk45.y - y_ana_rk45, axis=0))
    
    # DOP853 (ekvivalent ode78 v MATLABu)
    print("Výpočet scipy ode78 (DOP853)")
    tic = time.time()
    sol_dop853 = solve_ivp(ode_system, [0, t_final], y0, method='DOP853', **options)
    dop853_time = time.time() - tic
    dop853_steps = len(sol_dop853.t) - 1
    y_ana_dop853 = analytical_solution(sol_dop853.t)
    dop853_error = np.max(np.linalg.norm(sol_dop853.y - y_ana_dop853, axis=0))
    
    # LSODA s nastavením non-stiff (ekvivalent ode113 v MATLABu)
    print("Výpočet scipy ode113 (LSODA)")
    tic = time.time()
    sol_lsoda = solve_ivp(ode_system, [0, t_final], y0, method='LSODA', **options)
    lsoda_time = time.time() - tic
    lsoda_steps = len(sol_lsoda.t) - 1
    y_ana_lsoda = analytical_solution(sol_lsoda.t)
    lsoda_error = np.max(np.linalg.norm(sol_lsoda.y - y_ana_lsoda, axis=0))
    
    # Vyhodnocení výsledků
    methods = [
        "Taylor", "Euler", "RK4", 
        "scipy ode23", "scipy ode45", "scipy ode78", "scipy ode113"
    ]
    
    steps = [
        taylor_steps, euler_steps, rk4_steps, 
        rk23_steps, rk45_steps, dop853_steps, lsoda_steps
    ]
    
    errors = [
        taylor_error, euler_error, rk4_error, 
        rk23_error, rk45_error, dop853_error, lsoda_error
    ]
    
    times = [
        taylor_time, euler_time, rk4_time, 
        rk23_time, rk45_time, dop853_time, lsoda_time
    ]
    
    ratios = [time / taylor_time for time in times]
    
    # Vytvoření tabulky výsledků
    results = list(zip(methods, steps, errors, times, ratios))
    
    print("\nVýsledky pro systém sin/cos:")
    print(tabulate(results, 
                  headers=["Metoda", "Počet kroků", "Chyba", "Čas výpočtu [s]", "Poměr času"],
                  floatfmt=".6g"))
    
    # Uložení výsledků do slovníku
    return {
        'taylor': (t_taylor, y_taylor, taylor_error, taylor_time),
        'euler': (t_euler, y_euler, euler_error, euler_time),
        'rk4': (t_rk4, y_rk4, rk4_error, rk4_time),
        'ode23': (sol_rk23.t, sol_rk23.y, rk23_error, rk23_time),
        'ode45': (sol_rk45.t, sol_rk45.y, rk45_error, rk45_time),
        'ode78': (sol_dop853.t, sol_dop853.y, dop853_error, dop853_time),
        'ode113': (sol_lsoda.t, sol_lsoda.y, lsoda_error, lsoda_time)
    }

if __name__ == "__main__":
    # Test s výchozími parametry
    results1 = sin_cos_test()
    
    # Test s nižší tolerancí pro přesnější výsledky
    print("\n\nTest s vyšší přesností (tolerance 1e-7):")
    results2 = sin_cos_test(tolerance=1e-7, EPS=1e-6)
    
    # Test s větším krokem pro Taylorovu metodu
    print("\n\nTest s větším krokem pro Taylorovu metodu (h=10):")
    results3 = sin_cos_test(tolerance=1e-7, h_taylor=10, EPS=1e-4)