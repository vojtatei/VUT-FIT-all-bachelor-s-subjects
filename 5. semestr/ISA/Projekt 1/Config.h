/******************************************************************************
 *                                                                            *
 *                             Název souboru: Config.h                        *
 *                             Předmět: ISA - IMAP Klient                     *
 *                             Autor: Vojtěch Teichmann (xteich02)            *
 *                                                                            *
 * Popis:                                                                     *
 *   Hlavičkový soubor pro třídu Config, která slouží k načítání              *
 *   a správě konfigurace aplikace z příkazového řádku.                       *
 *   Třída poskytuje nastavení pro server, TLS, autentizaci,                  *
 *   výběr schránky a výstupního adresáře.                                    *
 *                                                                            *
 ******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

/**
 * @brief Třída Config zajišťuje načítání a správu konfiguračních parametrů
 * z příkazového řádku pro IMAP klienta.
 */
class Config {
public:
    /**
     * @brief Konstruktor, který načte parametry z příkazového řádku.
     * @param argc Počet argumentů.
     * @param argv Pole argumentů.
     */
    Config(int argc, char* argv[]);

    /// @brief Vrací název serveru, na který se klient připojí.
    std::string getServer() const;

    /// @brief Vrací port pro připojení k IMAP serveru.
    int getPort() const;

    /// @brief Vrací true, pokud je aktivováno TLS.
    bool isTLSEnabled() const;

    /// @brief Vrací cestu k souboru s certifikátem pro TLS.
    std::string getCertFile() const;

    /// @brief Vrací cestu k adresáři s certifikáty pro TLS.
    std::string getCertDir() const;

    /// @brief Vrací cestu k souboru s autentizačními údaji.
    std::string getAuthFile() const;

    /// @brief Vrací název schránky, ze které se budou stahovat e-maily.
    std::string getMailbox() const;

    /// @brief Vrací cestu k výstupnímu adresáři pro ukládání stažených zpráv.
    std::string getOutputDir() const;

    /// @brief Vrací true, pokud mají být staženy pouze nové nepřečtené zprávy.
    bool downloadOnlyNew() const;

    /// @brief Vrací true, pokud mají být staženy pouze hlavičky zpráv.
    bool downloadHeadersOnly() const;

private:
    /**
     * @brief Metoda pro zpracování argumentů příkazového řádku a jejich
     * ukládání do atributů konfigurace.
     * @param argc Počet argumentů.
     * @param argv Pole argumentů.
     */
    void parseArgs(int argc, char* argv[]);

    std::string server;           ///< Název serveru.
    int port;                     ///< Port pro připojení.
    bool tlsEnabled;              ///< Povolení TLS.
    std::string certFile;         ///< Cesta k certifikátu pro TLS.
    std::string certDir;          ///< Cesta k adresáři s certifikáty.
    std::string authFile;         ///< Cesta k souboru s autentizačními údaji.
    std::string mailbox;          ///< Název schránky (např. INBOX).
    std::string outputDir;        ///< Cesta k výstupnímu adresáři pro zprávy.
    std::vector<std::string> ignoredUIDs; ///< Seznam ignorovaných UID zpráv.
    bool newMessagesOnly;         ///< Stažení pouze nových zpráv.
    bool headersOnly;             ///< Stažení pouze hlaviček zpráv.
};

#endif // CONFIG_H
