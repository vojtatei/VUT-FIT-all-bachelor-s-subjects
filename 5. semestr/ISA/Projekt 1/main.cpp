/******************************************************************************
 *                                                                            *
 *                             Název souboru: main.cpp                        *
 *                             Předmět: ISA - IMAP Klient                     *
 *                             Autor: Vojtěch Teichmann (xteich02)            *
 *                                                                            *
 * Popis:                                                                     *
 *   Hlavní soubor aplikace IMAP klienta, který obsahuje funkce pro inicializaci  *
 *   konfigurace a autentizace. Tento soubor spouští hlavní logiku aplikace,  *
 *   včetně připojení k IMAP serveru, autentizace a stahování e-mailů.        *
 *                                                                            *
 ******************************************************************************/

#include <iostream>
#include <openssl/ssl.h>
#include "main.h"
#include "IMAPClient.h"

// Funkce pro inicializaci objektu Config na základě argumentů příkazového řádku
void initializeConfig(int argc, char* argv[], Config& config) {
    try {
        config = Config(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Chyba při inicializaci Config: " << e.what() << std::endl;
        std::exit(1);  // Ukončí program při chybě konfigurace
    }
}

// Funkce pro inicializaci objektu AuthFile s použitím konfiguračních údajů
void initializeAuth(const Config& config, AuthFile& authFile) {
    try {
        authFile = AuthFile(config.getAuthFile());
    } catch (const std::runtime_error& e) {
        std::cerr << "Chyba při načítání autentizačních údajů: " << e.what() << std::endl;
        std::exit(1);  // Ukončí program při chybě načítání autentizace
    }
}

// Hlavní funkce aplikace IMAP klienta
int main(int argc, char* argv[]) {
    Config config(argc, argv);  // Vytvoření a inicializace konfigurace
    initializeConfig(argc, argv, config);
    
    AuthFile authFile(config.getAuthFile());  // Vytvoření a inicializace autentizace
    initializeAuth(config, authFile);

    try {
        // Vytvoření instance IMAP klienta a spuštění hlavní logiky aplikace
        IMAPClient imapClient(config, authFile);
        imapClient.connect();  // Připojení k serveru
        imapClient.login();    // Přihlášení k serveru
        imapClient.fetchEmails();  // Stažení e-mailů
    } catch (const std::runtime_error& e) {
        std::cerr << "Chyba: " << e.what() << std::endl;
        return 1;  // Ukončení programu při chybě IMAP klienta
    } catch (...) {
        std::cerr << "Došlo k neznámé chybě." << std::endl;
        return 1;  // Ukončení programu při neznámé chybě
    }

    return 0;  // Úspěšné ukončení programu
}
