/******************************************************************************
 *                                                                            *
 *                             Název souboru: TLSHandler.h                    *
 *                             Předmět: ISA - IMAP Klient                     *
 *                             Autor: Vojtěch Teichmann (xteich02)            *
 *                                                                            *
 * Popis:                                                                     *
 *   Hlavičkový soubor pro třídu TLSHandler, která spravuje SSL/TLS kontext   *
 *   pro zabezpečenou komunikaci klienta s IMAP serverem.                     *
 *   Zajišťuje inicializaci SSL knihovny a načítání certifikátů pro           *
 *   ověření serveru.                                                         *
 *                                                                            *
 ******************************************************************************/


#ifndef TLSHANDLER_H
#define TLSHANDLER_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>


/**
 * @brief Třída TLSHandler
 *
 * Třída spravuje SSL/TLS kontext pro zabezpečenou komunikaci klienta s IMAP serverem.
 */
class TLSHandler {
public:

    /**
     * @brief Konstruktor třídy TLSHandler
     *
     * Inicializuje SSL knihovnu.
     */
    TLSHandler();

    /**
     * @brief Destruktor třídy TLSHandler
     *
     * Uvolňuje SSL kontext a čistí SSL knihovnu.
     */
    ~TLSHandler();


    /**
     * @brief Inicializace SSL kontextu
     *
     * @param certFile Cesta k souboru s certifikátem
     * @param certDir Cesta k adresáři s certifikáty
     *
     * Nastavuje SSL kontext pro použití TLS a načítá certifikát pro ověření serveru.
     */
    void initializeSSL(const std::string& certFile, const std::string& certDir);



    /**
     * @brief Získání SSL kontextu
     *
     * @return Ukazatel na SSL kontext
     */
    SSL_CTX* getContext() const;

private:
    SSL_CTX *ssl_ctx; /**< SSL kontext pro zabezpečenou komunikaci */
};

#endif // TLSHANDLER_H
