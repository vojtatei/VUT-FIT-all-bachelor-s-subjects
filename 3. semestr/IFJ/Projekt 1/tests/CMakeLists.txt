# CMakeLists.txt

# Nastavení názvu vaší testovací aplikace a všech testovacích souborů
add_executable(testScanner scannerTest.cpp)

# Nastavení cesty ke zdrojovému kódu a hlavičkovým souborům
target_include_directories(testScanner PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Připojení Google Test jako závislosti
target_link_libraries(testScanner gtest gtest_main pthread)

# Přidání testu
add_test(NAME testScanner COMMAND testScanner)
include(GoogleTest)
gtest_discover_tests(testScanner)
