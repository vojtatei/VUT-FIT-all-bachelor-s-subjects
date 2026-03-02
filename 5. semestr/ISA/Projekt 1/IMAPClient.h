/******************************************************************************
 *                                                                            *
 *                             Název souboru: IMAPClient.h                    *
 *                             Předmět: ISA - IMAP Klient                     *
 *                             Autor: Vojtěch Teichmann (xteich02)            *
 *                                                                            *
 * Popis:                                                                     *
 *   Hlavičkový soubor pro třídu IMAPClient, která implementuje               *
 *   klienta pro práci s protokolem IMAP. Třída umožňuje připojení            *
 *   k IMAP serveru, autentizaci, výběr schránky a stahování e-mailů.         *
 *   Podporuje také šifrované připojení pomocí TLS a správu stažených         *
 *   zpráv včetně ukládání do souborů.                                        *
 *                                                                            *
 ******************************************************************************/


#ifndef IMAPCLIENT_H
#define IMAPCLIENT_H

#include <string>
#include <vector>
#include "Config.h"
#include "AuthFile.h"
#include "TLSHandler.h"
#include <openssl/ssl.h>
#include <sys/stat.h>

/**
 * @class IMAPClient
 * @brief Třída pro správu IMAP připojení, autentizaci a stahování e-mailů z IMAP serveru.
 * 
 * Třída IMAPClient implementuje funkce potřebné pro připojení k IMAP serveru,
 * autentizaci, výběr schránky a stahování e-mailů s možností šifrovaného připojení pomocí TLS.
 */
class IMAPClient {
public:
    /**
     * @brief Konstruktor třídy IMAPClient.
     * @param config Konfigurace IMAP klienta.
     * @param authFile Objekt obsahující autentizační údaje.
     */
    IMAPClient(const Config& config, const AuthFile& authFile);

    /**
     * @brief Destruktor třídy IMAPClient.
     */
    ~IMAPClient();

    /**
     * @brief Připojení k IMAP serveru.
     */
    void connect();

    /**
     * @brief Autentizace uživatele na IMAP serveru.
     */
    void login();

    /**
     * @brief Stažení e-mailů z aktuálně vybrané schránky.
     */
    void fetchEmails();

    /**
     * @brief Výběr schránky na IMAP serveru.
     * @param mailboxName Název schránky.
     */
    void selectMailbox(const std::string& mailboxName);

private:
    const Config& config; ///< Konfigurace IMAP klienta.
    const AuthFile& authFile; ///< Autentizační údaje.
    TLSHandler *tlsHandler; ///< Správce TLS připojení.
    int sockfd; ///< Soketové připojení k serveru.
    SSL_CTX *ssl_ctx; ///< SSL kontext.
    SSL *ssl; ///< SSL spojení.

    int commandTagCounter; ///< Počítadlo příkazových tagů.

    /**
     * @brief Odeslání příkazu na IMAP server.
     * @param command Příkaz, který se má odeslat.
     * @param useTag Určuje, zda se má použít příkazový tag.
     */
    void sendCommand(const std::string& command, bool useTag = true);

    /**
     * @brief Příjem odpovědi ze serveru.
     * @return Řetězec obsahující odpověď serveru.
     */
    std::string receiveResponse();

    /**
     * @brief Uložení e-mailu do souboru.
     * @param emailData Obsah e-mailu.
     * @param uid UID zprávy.
     * @param defaultFilename Výchozí název souboru.
     */
    void saveEmail(const std::string& emailData, const std::string& uid, const std::string& defaultFilename);

    /**
     * @brief Vytvoří adresář, pokud neexistuje.
     * @param directory Cesta k adresáři.
     */
    void createDirectoryIfNotExists(const std::string& directory);

    /**
     * @brief Získá další příkazový tag.
     * @return Řetězec příkazového tagu.
     */
    std::string getNextCommandTag();

    /**
     * @brief Získá aktuální příkazový tag.
     * @return Řetězec aktuálního příkazového tagu.
     */
    std::string getCurrentCommandTag() const;

    /**
     * @brief Kontroluje, zda existuje soubor s předponou.
     * @param prefix Předpona pro hledání souboru.
     * @return True, pokud soubor s předponou existuje, jinak false.
     */
    bool fileExistsWithPrefix(const std::string& prefix);

    /**
     * @brief Kontroluje, zda všechny zprávy jsou z jedné schránky.
     * @param mailboxName Název schránky.
     * @return True, pokud jsou všechny zprávy z jedné schránky, jinak false.
     */
    bool isSameMailboxStored(const std::string& mailboxName);

    /**
     * @brief Vyčistí výstupní adresář od všech e-mailových souborů.
     */
    void clearOutputDirectory();
};

#endif // IMAPCLIENT_H
