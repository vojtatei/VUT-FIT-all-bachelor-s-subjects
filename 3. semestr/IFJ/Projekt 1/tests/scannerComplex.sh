#!/bin/bash


# Složky obsahující soubory
#script_dir = $(dirname $0)
input_folder="testinput/"
output_folder="$(dirname "$(realpath "$0")")/testoutputs/"

green='\033[0;32m'
red='\033[0;31m'
reset='\033[0m'


# Najde všechny soubory ve formátu scanner-*-expected.txt ve složce testinputs
files_expected=($(find "$(dirname "$(realpath "$0")")/$input_folder" -name "scanner-*-expected.txt"))

# Porovná aktuální a očekávané soubory
for file_expected in "${files_expected[@]}"; do
    # Vytvoří název odpovídajícího aktuálního souboru
    file_actual="${output_folder}$(basename "$file_expected" -expected.txt)-actual.txt"
    
    # Kontroluje, zda existuje odpovídající aktuální soubor
    if [ -e "$file_actual" ]; then
        # Porovná aktuální a očekávaný soubor
        if diff_output=$(diff -u "$file_actual" "$file_expected"); then
            echo -e "${green}OK $(basename $file_actual)"
        else
            echo -e "${red}ERROR $(basename $file_actual)"
            echo -e "${reset}Difference:"
            echo "$diff_output"
        fi
    else
        echo -e "${red}Aktuální soubor pro $file_expected neexistuje."
    fi
done
