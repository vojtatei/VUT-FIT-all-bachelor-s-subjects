#!/bin/bash

green='\033[0;32m'
red='\033[0;31m'
reset='\033[0m'

SCRIPT_DIR="$(dirname "$0")"

(cd "$SCRIPT_DIR/.." && make)

count=0
failed=0
# LEX
echo -e "------------------------"
echo -e "SCANNER"
echo -e "------------------------"
find "$SCRIPT_DIR/testinput/lexError/" -name "*.ifj23" | while read test_file; do
  $SCRIPT_DIR/../build/compiler < "$test_file" > /dev/null
  compiler_exit_code=$?

  if [ $compiler_exit_code -ne 1 ]; then
    echo -e "${red}FAIL Expected return code 1, got $compiler_exit_code${reset}\n"

  else
    echo -e "${green}OK${reset} $(basename $test_file)"
  fi
done

echo -e "------------------------"
echo -e "SYNTAX"
echo -e "------------------------"


find "$SCRIPT_DIR/testinput/syntax/" -name "*.ifj23" | while read test_file; do
  file_name=$(basename "$test_file")
  $SCRIPT_DIR/../build/compiler < "$test_file" > /dev/null
  compiler_exit_code=$?

  if [ $compiler_exit_code -ne 2 ]; then
    echo -e "${red}$file_name - FAIL Expected return code 2, got $compiler_exit_code${reset}\n"
  else
    echo -e "${green}OK${reset} $file_name"
  fi
done


echo -e "------------------------"
echo -e "EXPRESSION"
echo -e "------------------------"

find "$SCRIPT_DIR/testinput/expression/" -name "*.ifj23" | while read test_file; do
  # Rozdělení názvu souboru a získání očekávané návratové hodnoty
  file_name=$(basename "$test_file")
  expected_return_code="${file_name%%-*}"

  $SCRIPT_DIR/../build/compiler < "$test_file" > /dev/null
  actual_return_code=$?

  if [ $actual_return_code -ne $expected_return_code ]; then
    echo -e "${red}$file_name - FAIL Expected return code $expected_return_code, got $actual_return_code${reset}\n"
  else
    echo -e "${green}OK${reset} $file_name"
  fi
done


echo -e "------------------------"
echo -e "SEMANTIC - TODO VICE DRUHU CHYB"
echo -e "------------------------"


find "$SCRIPT_DIR/testinput/semantic/" -name "*.ifj23" | while read test_file; do
  # Rozdělení názvu souboru a získání očekávané návratové hodnoty
  file_name=$(basename "$test_file")
  expected_return_code="${file_name%%-*}"

  $SCRIPT_DIR/../build/compiler < "$test_file" > /dev/null
  actual_return_code=$?

  if [ $actual_return_code -ne $expected_return_code ]; then
    echo -e "${red}$file_name - FAIL Expected return code $expected_return_code, got $actual_return_code${reset}\n"
  else
    echo -e "${green}OK${reset} $file_name"
  fi
done

echo -e "------------------------"
echo -e "COMPLEX TESTS"
echo -e "------------------------"

find "$SCRIPT_DIR/testinput/swiftTests/" -name "*.swift" | while read test_file; do
  # Rozdělení názvu souboru a získání očekávané návratové hodnoty
  file_name=$(basename "$test_file")

  if [ "$file_name" = "ifj23.swift" ]; then
      continue
  fi

  echo "Running $file_name"

  $SCRIPT_DIR/../build/compiler < "$test_file" > "$SCRIPT_DIR/testoutputs/compiler.in"
  # Vygenerovani swift vystupu
  # swift <(cat "$test_file" "$SCRIPT_DIR/testinput/swiftTests/ifj23.swift") > "$SCRIPT_DIR/testoutputs/$file_name.out"
  $SCRIPT_DIR/../ic23int "$SCRIPT_DIR/testoutputs/compiler.in" > "$SCRIPT_DIR/testoutputs/compiler.out"

  # Porovnání výstupů kompilátoru a Swiftu
  if diff "$SCRIPT_DIR/testoutputs/compiler.out" "$SCRIPT_DIR/testoutputs/$file_name.out"; then
    echo -e "${green}OK${reset} $file_name"
  else
    echo -e "${red}FAIL - Output difference in $file_name:${reset}"
    # Zobrazení rozdílů
    #diff -u "$SCRIPT_DIR/testoutputs/compiler.out" "$SCRIPT_DIR/testoutputs/swift.out"
    echo -e "\n"
  fi

done


echo -e "----------------------------       ----------------"
