/******************************************************************************
 *                                                                            *
 *                             Název souboru: TLSHandler.cpp                  *
 *                             Předmět: ISA - IMAP Klient                     *
 *                             Autor: Vojtěch Teichmann (xteich02)            *
 *                                                                            *
 * Popis:                                                                     *
 *   Implementace třídy TLSHandler, která zajišťuje správu SSL/TLS kontextu   *
 *   pro zabezpečenou komunikaci klienta. Třída zajišťuje inicializaci        *
 *   SSL knihovny, vytvoření SSL kontextu a načítání certifikátů              *
 *   pro ověření serveru.                                                     *
 *                                                                            *
 ******************************************************************************/

#include "TLSHandler.h"
#include <stdexcept>

/**
 * @brief Konstruktor třídy TLSHandler
 *
 * Inicializuje SSL knihovnu a načítá chybové řetězce OpenSSL.
 */
TLSHandler::TLSHandler() : ssl_ctx(nullptr) {
    SSL_load_error_strings();         /**< Načítání chybových řetězců OpenSSL */
    OpenSSL_add_ssl_algorithms();     /**< Přidání všech SSL algoritmů */
}

/**
 * @brief Destruktor třídy TLSHandler
 *
 * Uvolňuje SSL kontext a provádí čistění SSL knihovny.
 */
TLSHandler::~TLSHandler() {
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);  /**< Uvolnění SSL kontextu */
    }
    EVP_cleanup();  /**< Čistění po OpenSSL */
}

/**
 * @brief Inicializace SSL kontextu
 *
 * @param certFile Cesta k souboru s certifikátem
 * @param certDir Cesta k adresáři s certifikáty
 *
 * Nastavuje SSL kontext pro použití TLS a načítá certifikát pro ověření serveru.
 * Vyhazuje výjimku std::runtime_error v případě chyby.
 */
void TLSHandler::initializeSSL(const std::string& certFile, const std::string& certDir) {
    SSL_load_error_strings();           /**< Načítání chybových řetězců */
    OpenSSL_add_all_algorithms();       /**< Přidání všech algoritmů */

    const SSL_METHOD *method = TLS_client_method();  /**< Použití metody TLS pro klienta */
    ssl_ctx = SSL_CTX_new(method);                   /**< Vytvoření SSL kontextu */

    if (!ssl_ctx) {
        throw std::runtime_error("Nelze vytvořit SSL kontext.");
    }

    // Načítání certifikátů pro ověření, pokud je specifikován certifikát
    if (!certFile.empty()) {
        if (!SSL_CTX_load_verify_locations(ssl_ctx, certFile.c_str(), certDir.c_str())) {
            SSL_CTX_free(ssl_ctx);  /**< Uvolnění kontextu v případě chyby */
            throw std::runtime_error("Chyba při načítání certifikátu.");
        }
    }
}

/**
 * @brief Získání SSL kontextu
 *
 * @return Ukazatel na SSL kontext
 */
SSL_CTX* TLSHandler::getContext() const {
    return ssl_ctx;
}
