/******************************************************************************
 *                                                                            *
 *                             Název souboru: AuthFile.h                      *
 *                             Předmět: ISA - IMAP Klient                     *
 *                             Autor: Vojtěch Teichmann (xteich02)            *
 *                                                                            *
 * Popis:                                                                     *
 *   Hlavičkový soubor třídy AuthFile, která slouží k načtení a správě        *
 *   autentizačních údajů (uživatelského jména a hesla) z externího souboru.  *
 *   Tyto údaje jsou vyžadovány pro autentizaci při připojení IMAP klienta    *
 *   k serveru.                                                               *
 *                                                                            *
 ******************************************************************************/


#ifndef AUTHFILE_H
#define AUTHFILE_H

#include <string>


/**
 * @class AuthFile
 * @brief Třída pro načítání a správu autentizačních údajů ze souboru.
 */
class AuthFile {
public:

    /**
     * @brief Konstruktor, který inicializuje autentizační údaje načtením ze zadaného souboru.
     * @param filename Cesta k souboru obsahujícímu autentizační údaje.
     */
    AuthFile(const std::string& filename);


    /**
     * @brief Získá uživatelské jméno.
     * @return Uživatelské jméno jako std::string.
     */
    std::string getUsername() const;


    /**
     * @brief Získá heslo.
     * @return Heslo jako std::string.
     */
    std::string getPassword() const;
    
private:

    /**
     * @brief Načte autentizační údaje ze zadaného souboru.
     * @param filename Cesta k souboru obsahujícímu autentizační údaje.
     */
    void loadCredentials(const std::string& filename);

    
    std::string username; ///< Uživatelské jméno načtené ze souboru.
    std::string password; ///< Heslo načtené ze souboru.
};

#endif // AUTHFILE_H
