/******************************************************************************
 *                                                                            *
 *                             Název souboru: main.h                          *
 *                             Předmět: ISA - IMAP Klient                     *
 *                             Autor: Vojtěch Teichmann (xteich02)            *
 *                                                                            *
 * Popis:                                                                     *
 *   Hlavičkový soubor pro hlavní funkce aplikace IMAP klienta. Obsahuje      *
 *   deklarace funkcí pro inicializaci konfigurace a autentizace.             *
 *                                                                            *
 ******************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include "Config.h"
#include "AuthFile.h"
#include "TLSHandler.h"


/**
 * Inicializuje konfiguraci aplikace na základě parametrů příkazového řádku.
 *
 * @param argc Počet argumentů.
 * @param argv Pole argumentů příkazového řádku.
 * @param config Odkaz na objekt konfigurace, který bude inicializován.
 */
void initializeConfig(int argc, char* argv[], Config& config);


/**
 * Inicializuje autentizační údaje na základě zadané konfigurace.
 *
 * @param config Konfigurace aplikace, která obsahuje cestu k souboru s autentizací.
 * @param authFile Odkaz na objekt pro autentizaci, který bude inicializován.
 */
void initializeAuth(const Config& config, AuthFile& authFile);

#endif // MAIN_H
