/******************************************************************************
 *                                                                            *
 *                             Název souboru: Config.cpp                      *
 *                             Předmět: ISA - IMAP Klient                     *
 *                             Autor: Vojtěch Teichmann (xteich02)            *
 *                                                                            *
 * Popis:                                                                     *
 *   Implementace třídy Config, která zpracovává parametry příkazového        *
 *   řádku a ukládá konfiguraci pro IMAP klienta. Třída umožňuje nastavit     *
 *   různé možnosti, jako je adresa serveru, port, TLS, certifikáty,          *
 *   přihlašovací údaje a výstupní adresář pro ukládání stažených zpráv.      *
 *                                                                            *
 ******************************************************************************/

#include "Config.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

/**
 * @brief Konstruktor třídy Config. Načítá parametry příkazového řádku a
 * inicializuje konfiguraci.
 * @param argc Počet argumentů příkazového řádku.
 * @param argv Pole argumentů příkazového řádku.
 */
Config::Config(int argc, char* argv[])
    : port(143), tlsEnabled(false), certDir("/etc/ssl/certs"), mailbox("INBOX"), 
      newMessagesOnly(false), headersOnly(false) {
    parseArgs(argc, argv);
}

/**
 * @brief Zpracování argumentů příkazového řádku a nastavení konfigurace.
 * Očekává argumenty pro server, port, TLS, certifikáty, přihlašovací údaje,
 * název schránky a výstupní adresář.
 * @param argc Počet argumentů příkazového řádku.
 * @param argv Pole argumentů příkazového řádku.
 */
void Config::parseArgs(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Použití: imapcl server [-p port] [-T [-c certfile] [-C certaddr]] "
                  << "[-n] [-h] -a auth_file [-b MAILBOX] -o out_dir" << std::endl;
        std::exit(1);
    }

    // První parametr musí být název serveru
    server = argv[1];
    bool portExplicitlySet = false;

    for (int i = 2; i < argc; ++i) {
        if (std::strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            port = std::atoi(argv[++i]);
            portExplicitlySet = true;
        } else if (std::strcmp(argv[i], "-T") == 0) {
            tlsEnabled = true;
            if (!portExplicitlySet) {
                port = 993;
            }
        } else if (std::strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            certFile = argv[++i];
        } else if (std::strcmp(argv[i], "-C") == 0 && i + 1 < argc) {
            certDir = argv[++i];
        } else if (std::strcmp(argv[i], "-a") == 0 && i + 1 < argc) {        
            authFile = argv[++i];
        } else if (std::strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            mailbox = argv[++i];
        } else if (std::strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            outputDir = argv[++i];
        } else if (std::strcmp(argv[i], "-n") == 0) {
            newMessagesOnly = true;
        } else if (std::strcmp(argv[i], "-h") == 0) {
            headersOnly = true;
        } else {
            std::cerr << "Neznámý parametr: " << argv[i] << std::endl;
            std::exit(1);
        }
    }

    // Kontrola povinných parametrů
    if (authFile.empty()) {
        std::cerr << "Chybí povinný parametr: auth_file (-a)." << std::endl;
        std::exit(1);
    }

    if (outputDir.empty()) {
        std::cerr << "Chybí povinný parametr: output_dir (-o)." << std::endl;
        std::exit(1);
    }
}

// Gettery pro získání konfiguračních hodnot

/// @brief Vrací název serveru pro připojení.
std::string Config::getServer() const {
    return server;
}

/// @brief Vrací port, na kterém bude klient komunikovat se serverem.
int Config::getPort() const {
    return port;
}

/// @brief Vrací hodnotu, zda je povolené TLS.
bool Config::isTLSEnabled() const {
    return tlsEnabled;
}

/// @brief Vrací cestu k souboru s certifikátem pro TLS ověření.
std::string Config::getCertFile() const {
    return certFile;
}

/// @brief Vrací adresář s certifikáty pro TLS ověření.
std::string Config::getCertDir() const {
    return certDir;
}

/// @brief Vrací cestu k souboru s autentizačními údaji.
std::string Config::getAuthFile() const {
    return authFile;
}

/// @brief Vrací název schránky pro stažení e-mailů.
std::string Config::getMailbox() const {
    return mailbox;
}

/// @brief Vrací adresář, kam se uloží stažené zprávy.
std::string Config::getOutputDir() const {
    return outputDir;
}

/// @brief Vrací hodnotu, zda se mají stahovat pouze nové nepřečtené zprávy.
bool Config::downloadOnlyNew() const {
    return newMessagesOnly;
}

/// @brief Vrací hodnotu, zda se mají stahovat pouze hlavičky e-mailů.
bool Config::downloadHeadersOnly() const {
    return headersOnly;
}
