from z3 import *


# První úloha: De Morganův zákon
# Deklarace proměnných
A = Bool('A')
B = Bool('B')

# Vytvoření solveru
s = Solver()

# Výrazy pro De Morganův zákon
de_morgan_expr = Not(Or(A, B)) == And(Not(A), Not(B))

# Přidání výrazu do solveru a kontrola
s.add(de_morgan_expr)
result = s.check()

# Výstup
print("De Morganův zákon je platný:", result == sat)


# Druhý De Morganův zákon
de_morgan_expr2 = Not(And(A, B)) == Or(Not(A), Not(B))

# Přidání druhého výrazu a kontrola
s.reset()  # Resetování solveru pro nové ověřování
s.add(de_morgan_expr2)
result2 = s.check()

# Výstup
print("Druhý De Morganův zákon je platný:", result2 == sat)


# Druhá úloha: kinematická rovnice pro brouka Kvapníka
# Deklarace proměnných
vi = Real('vi')  # Počáteční rychlost
a = Real('a')    # Zrychlení
vf = Real('vf')  # Koncová rychlost
t = Real('t')    # Čas
d = Real('d')    # Vzdálenost

# Resetování a nová inicializace solveru
s.reset()

# Kinematické rovnice
eq1 = vf == vi + a * t
eq2 = d == vi * t + 0.5 * a * t**2

# Zadání hodnot
s.add(vi == 25.0, a == -9.00, vf == 0)

# Přidání rovnic do solveru
s.add(eq1, eq2)

# Řešení
if s.check() == sat:
    m = s.model()
    # Použití eval() pro vyhodnocení zlomku na desetinné číslo
    t_val = eval(m.eval(t, model_completion=True).__str__())
    d_val = eval(m.eval(d, model_completion=True).__str__())
    print(f"Čas potřebný k zastavení (t): {t_val:.3f} sekund")  
    print(f"Vzdálenost ujetá při brzdění (d): {d_val:.3f} metrů") 
else:
    print("Nelze najít řešení.")

# Třetí úloha: Hypersudoku

# 9x9 mřížka Sudoku
X = [[Int("x_%s_%s" % (i + 1, j + 1)) for j in range(9)] for i in range(9)]

s.reset()

# Základní omezení pro Sudoku
# Hodnoty musí být mezi 1 a 9
s.add([And(1 <= X[i][j], X[i][j] <= 9) for i in range(9) for j in range(9)])

# Každý řádek musí obsahovat unikátní čísla
for i in range(9):
    s.add(Distinct(X[i]))

# Každý sloupec musí obsahovat unikátní čísla
for j in range(9):
    s.add(Distinct([X[i][j] for i in range(9)]))

# Každý 3x3 blok musí obsahovat unikátní čísla
for i in range(0, 9, 3):
    for j in range(0, 9, 3):
        s.add(Distinct([X[i + k][j + l] for k in range(3) for l in range(3)]))

# Přidání Hyper-bloků
hyper_blocks = [(1, 1), (1, 5), (5, 1), (5, 5)]
for (i, j) in hyper_blocks:
    s.add(Distinct([X[i + k][j + l] for k in range(3) for l in range(3)]))

instance = [
    [0, 0, 0, 3, 6, 0, 0, 8, 0],
    [7, 0, 4, 2, 0, 5, 0, 9, 3],
    [0, 8, 0, 0, 0, 0, 7, 0, 0],
    [0, 0, 0, 0, 0, 8, 2, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 9, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 7, 4],
    [2, 7, 0, 0, 3, 0, 0, 0, 0],
    [4, 0, 1, 8, 0, 0, 0, 0, 0]
]

# Vložení instance do solveru
for i in range(9):
    for j in range(9):
        if instance[i][j] != 0:
            s.add(X[i][j] == instance[i][j])

# Řešení a výstup
if s.check() == sat:
    m = s.model()
    r = [[m.evaluate(X[i][j]) for j in range(9)] for i in range(9)]
    print_matrix(r)
else:
    print("Nelze najít řešení")

# Čtvrtá úloha: Knapsack

# Parametry batohu
N = 50  # Nosnost batohu
C = 60  # Cílová cena

# Věci (hmotnost, cena)
items = [(10, 20), (20, 30), (30, 50)]

# Solver
s.reset()

# Proměnné reprezentující, zda položka byla vzata (1) nebo ne (0)
x = [Bool(f"x_{i}") for i in range(len(items))]

# Omezení: celková hmotnost nesmí přesáhnout N
s.add(Sum([If(x[i], items[i][0], 0) for i in range(len(items))]) <= N)

# Omezení: celková cena musí být alespoň C
s.add(Sum([If(x[i], items[i][1], 0) for i in range(len(items))]) >= C)

# Řešení a výpis
if s.check() == sat:
    m = s.model()
    chosen_items = [i for i in range(len(items)) if m[x[i]]]
    print("Vybrané položky:", chosen_items)
else:
    print("Není možné splnit požadavky.")


# Optimalizační solver
opt = Optimize()

# Proměnné a parametry stejně jako výše
opt.add(Sum([If(x[i], items[i][0], 0) for i in range(len(items))]) <= N)

# Místo dosahování pevně dané ceny se snažíme maximalizovat celkovou cenu
opt.maximize(Sum([If(x[i], items[i][1], 0) for i in range(len(items))]))

# Řešení a výpis
if opt.check() == sat:
    m = opt.model()
    chosen_items = [i for i in range(len(items)) if m[x[i]]]
    print("Optimalizované vybrané položky:", chosen_items)
else:
    print("Není možné najít optimální řešení.")


# Pátá úloha: Plánování

# Počet dní (7 dní v týdnu)
days = 7
# Počet směn za den (3 směny - ráno, odpoledne, noc)
shifts = 3
# Počet zaměstnanců
employees = 9

# Vytvoření proměnných
# X[d][s][e] je True, pokud zaměstnanec e pracuje ve směně s dne d
X = [[[Bool(f"x_{d}_{sh}_{e}") for e in range(employees)] for sh in range(shifts)] for d in range(days)]

# Solver
s = Solver()
s.reset()

# Každá směna každý den musí být obsazena právě dvěma zaměstnanci
for d in range(days):
    for sh in range(shifts):
        s.add(Sum([If(X[d][sh][e], 1, 0) for e in range(employees)]) == 2)

# Zaměstnanec může mít maximálně jednu směnu denně
for d in range(days):
    for e in range(employees):
        s.add(Sum([If(X[d][sh][e], 1, 0) for sh in range(shifts)]) <= 1)

# Příklady specifických omezení
# Zaměstnanec 1 nechce noční směny (směna 2)
for d in range(days):
    s.add(X[d][2][0] == False)

# Zaměstnanec 2 nechce pracovat o víkendech (dny 5 a 6)
for sh in range(shifts):
    s.add(X[5][sh][1] == False)
    s.add(X[6][sh][1] == False)

# Řešení a výstupy
if s.check() == sat:
    m = s.model()
    for d in range(days):
        print(f"Den {d+1}:")
        for sh in range(shifts):
            shift_employees = [e for e in range(employees) if m.evaluate(X[d][sh][e])]
            print(f"  Směna {sh+1}: Zaměstnanci {shift_employees}")
else:
    print("Není možné vytvořit platný rozvrh.")



# Šestá úloha: No-three-in-line

# Parametry
grid_size = 5  # Velikost mřížky
num_points = 6  # Počet bodů k umístění

s.reset()

# Definování proměnných
points = [[Bool(f"p_{i}_{j}") for j in range(grid_size)] for i in range(grid_size)]

# Omezení: Přesný počet bodů na mřížce
s.add(Sum([If(points[i][j], 1, 0) for i in range(grid_size) for j in range(grid_size)]) == num_points)

# Omezení: Žádné tři body nesmí být kolineární
for i in range(grid_size):
    for j in range(grid_size):
        for k in range(grid_size):
            if k != i:
                # Všechny body ve stejném sloupci
                if j + k - i < grid_size and j + k - i >= 0:
                    s.add(Implies(And(points[i][j], points[k][j]), Not(points[j+k-i][j])))

                # Všechny body v jedné řadě
                if j + k - i < grid_size and j + k - i >= 0:
                    s.add(Implies(And(points[i][j], points[i][k]), Not(points[i][j+k-i])))

                # Diagonály
                if k - i + j < grid_size and k - i + j >= 0 and j - k + i < grid_size and j - k + i >= 0:
                    s.add(Implies(And(points[i][j], points[k][k-i+j]), Not(points[j-k+i][j-k+i])))

# Řešení a výstupy
if s.check() == sat:
    m = s.model()
    solution = [[m.evaluate(points[i][j]) for j in range(grid_size)] for i in range(grid_size)]
    print("Je možné umístit body takto:")
    for row in solution:
        print(" ".join(["X" if row[j] else "." for j in range(grid_size)]))
else:
    print("Není možné umístit zadaný počet bodů bez kolinearity.")

# Sedmá úloha: Různé Paní

# Kterou cestu by mi doporučila druhá paní, kdybych se jí zeptal, která cesta vede k východu?