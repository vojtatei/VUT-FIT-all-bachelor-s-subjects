/******************************************************************************
 *                                                                            *
 *                             Název souboru: AuthFile.cpp                    *
 *                             Předmět: ISA - IMAP Klient                     *
 *                             Autor: Vojtěch Teichmann (xteich02)            *
 *                                                                            *
 * Popis:                                                                     *
 *   Třída AuthFile slouží k načtení a správě autentizačních údajů            *
 *   (uživatelského jména a hesla) z externího souboru. Tento soubor          *
 *   je vyžadován pro autentizaci při připojení IMAP klienta k serveru.       *
 *                                                                            *
 ******************************************************************************/


#include "AuthFile.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <cstring>
#include <cstdlib>
#ifndef PATH_MAX
#define PATH_MAX 4096 // Maximální délka cesty k souboru
#endif

// Konstruktor třídy AuthFile, který inicializuje autentizační údaje ze zadaného souboru
AuthFile::AuthFile(const std::string& filename) {
    loadCredentials(filename);
}

// Metoda pro načtení autentizačních údajů (uživatelského jména a hesla) ze souboru
void AuthFile::loadCredentials(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nelze otevřít soubor s autentizačními údaji: " << filename << std::endl;
        std::exit(1); 
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("username") != std::string::npos) {
            username = line.substr(line.find("=") + 1);
        } else if (line.find("password") != std::string::npos) {
            password = line.substr(line.find("=") + 1);
        }
    }

    file.close();

    if (username.empty() || password.empty()) {
        std::cerr << "Soubor s autentizačními údaji je neúplný: " << filename << std::endl;
        std::exit(1);
    }
}

// Getter pro získání uživatelského jména
std::string AuthFile::getUsername() const {
    return username;
}

// Getter pro získání hesla
std::string AuthFile::getPassword() const {
    return password;
}
