
import sys

def pos_to_var(row, col, size):
    variable = row * size + col + 1
    return variable

def generate_dimacs(size):
    clauses = []

    # Pro každý řádek na šachovnici
    for row in range(size):
        row_clause = []  # Vytvoříme prázdný seznam pro řádek
        for col in range(size):  # A pro každý sloupec v tom řádku
            row_clause.append(pos_to_var(row, col, size))  # Přidáme pozici do řádku
        clauses.append(row_clause)  # Přidáme celý řádek do podmínek

    # Nyní pro každý sloupec a diagonálu
    for row in range(size):
        for col in range(size):
            # Pro každý další řádek ve sloupci
            for i in range(row + 1, size):
                clauses.append([-pos_to_var(row, col, size), -pos_to_var(i, col, size)])  # Přidáme podmínku pro sloupce

            # A nyní diagonály
            for i in range(1, size):
                if row + i < size and col + i < size:
                    clauses.append([-pos_to_var(row, col, size), -pos_to_var(row + i, col + i, size)])
                if row + i < size and col - i >= 0:
                    clauses.append([-pos_to_var(row, col, size), -pos_to_var(row + i, col - i, size)])

    # Sestavení DIMACS formátu
    dimacs = "p cnf " + str(size * size) + " " + str(len(clauses)) + "\n"
    for clause in clauses:
        clause_str = ""  # Začneme s prázdným řetězcem pro každou podmínku
        for num in clause:
            clause_str += str(num) + " "  # Přidáme čísla a mezery
        clause_str += "0"  # Každá podmínka končí nulou
        dimacs += clause_str + "\n"  # Přidáme podmínku do výstupu

    return dimacs

# Velikost šachovnice z argumentu
N = int(sys.argv[1])
dimacs_output = generate_dimacs(N)
print(dimacs_output)
