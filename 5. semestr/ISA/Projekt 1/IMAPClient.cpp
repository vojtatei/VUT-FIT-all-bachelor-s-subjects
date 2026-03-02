
/******************************************************************************
 *                                                                            *
 *                             Název souboru: IMAPClient.cpp                  *
 *                             Předmět: ISA - IMAP Klient                     *
 *                             Autor: Vojtěch Teichmann (xteich02)            *
 *                                                                            *
 * Popis:                                                                     *
 *   Implementace třídy IMAPClient, která zajišťuje připojení k IMAP serveru, *
 *   autentizaci, výběr schránky a stahování e-mailů s možností použití TLS.  *
 *   Ukládá stažené zprávy do souborů a spravuje lokální kopii e-mailů.       *
 *                                                                            *
 ******************************************************************************/

#include "IMAPClient.h"
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <errno.h>
#include <sstream>
#include <vector>
#include <regex>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <dirent.h>
#include <sys/stat.h>
#include "TLSHandler.h"


IMAPClient::IMAPClient(const Config& config, const AuthFile& authFile)
    : config(config), authFile(authFile), tlsHandler(nullptr), sockfd(-1), ssl_ctx(nullptr), ssl(nullptr), commandTagCounter(1) {
    if (config.isTLSEnabled()) {
        tlsHandler = new TLSHandler();
        tlsHandler->initializeSSL(config.getCertFile(), config.getCertDir());
    }
}


IMAPClient::~IMAPClient() {
    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
    if (tlsHandler) {
        delete tlsHandler;  // Uvolnění třídy TLSHandler, která uvolní i SSL kontext
    }
    if (sockfd >= 0) {
        close(sockfd);
    }
}

void IMAPClient::connect() {
    struct hostent *host = gethostbyname(config.getServer().c_str());
    if (host == nullptr) {
        throw std::runtime_error("Nelze se připojit k serveru: Neznámý server nebo neplatná adresa (" + config.getServer() + ").");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(config.getPort());
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw std::runtime_error("Nelze vytvořit soket.");
    }

    if (::connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(sockfd);
        throw std::runtime_error("Nelze se připojit k serveru.");
    }

    if (config.isTLSEnabled()) {
        ssl = SSL_new(tlsHandler->getContext());
        SSL_set_fd(ssl, sockfd);
        if (SSL_connect(ssl) <= 0) {
            throw std::runtime_error("Chyba při připojování přes TLS.");
        }
    }
}

void IMAPClient::login() {
    std::string command = "LOGIN" + authFile.getUsername() + " " + authFile.getPassword();
    sendCommand(command, true);

    std::string response = receiveResponse();
    if (response.find("OK") == std::string::npos) {
        throw std::runtime_error("Přihlášení selhalo.");
    }
}

void IMAPClient::fetchEmails() {

    createDirectoryIfNotExists(config.getOutputDir());
    // Vybereme schránku
    selectMailbox(config.getMailbox());

    // Načtení UID zpráv ze serveru
    std::string searchCommand = config.downloadOnlyNew() ? "UID SEARCH UNSEEN" : "UID SEARCH ALL";
    sendCommand(searchCommand);
    std::string response = receiveResponse();

    // Extrakce UID zpráv ze serveru
    std::vector<std::string> serverUIDs;
    std::istringstream responseStream(response);
    std::string line;
    while (std::getline(responseStream, line)) {
        if (line.find("* SEARCH") != std::string::npos) {
            std::istringstream searchStream(line);
            std::string temp;
            searchStream >> temp;
            searchStream >> temp;
            while (searchStream >> temp) {
                serverUIDs.push_back(temp);
            }
        }
    }

    if (serverUIDs.empty()) {
        std::cout << "Na serveru nejsou žádné zprávy." << std::endl;
        return;
    }

    // Získání názvu schránky z konfigurace
    std::string mailboxName = config.getMailbox();

    // Kontrola, zda všechny zprávy jsou z jedné schránky
    if (!isSameMailboxStored(mailboxName)) {
        // Smazání všech souborů ve složce
        clearOutputDirectory();
    }

    int emailCount = 0;

    // Stažení nových zpráv
    for (const auto& uid : serverUIDs) {
        std::string filenamePrefix = "UID_" + uid + "_" + mailboxName;

        // Kontrola, zda už soubor s tímto UID existuje
        if (fileExistsWithPrefix(filenamePrefix)) {
            continue;
        }

        // Stažení hlaviček nebo celé zprávy
        std::string fetchCommand = config.downloadHeadersOnly()
            ? "UID FETCH " + uid + " (BODY.PEEK[HEADER])"
            : "UID FETCH " + uid + " (BODY.PEEK[])";
        sendCommand(fetchCommand);
        response = receiveResponse();

        // Extrakce těla zprávy nebo hlaviček
        size_t startPos = response.find("BODY[HEADER] {");
        if (startPos == std::string::npos) {
            startPos = response.find("BODY[] {");
        }

        if (startPos != std::string::npos) {
            startPos = response.find("{", startPos) + 1;
            size_t endPos = response.find("}", startPos);
            if (endPos != std::string::npos) {
                size_t bodySize = std::stoi(response.substr(startPos, endPos - startPos));
                std::string emailData = response.substr(endPos + 2, bodySize);

                // Uložení zprávy s formátem názvu UID_čísloUID_schránka
                std::string generatedFilename = config.getOutputDir() + "/UID_" + uid + "_" + mailboxName + ".eml";
                saveEmail(emailData, uid, generatedFilename);

                emailCount++;
            }
        }
    }

    std::cout << "Staženo " << emailCount << " nových zpráv." << std::endl;
}

void IMAPClient::sendCommand(const std::string& command, bool useTag) {
    std::string fullCommand = (useTag ? getNextCommandTag() + " " : "") + command + "\r\n";
    if (config.isTLSEnabled()) {
        SSL_write(ssl, fullCommand.c_str(), fullCommand.length());
    } else {
        send(sockfd, fullCommand.c_str(), fullCommand.length(), 0);
    }
}

void IMAPClient::selectMailbox(const std::string& mailboxName) {
    std::string command = "SELECT " + mailboxName;
    sendCommand(command, true); 

    std::string response = receiveResponse();
    if (response.find("OK") == std::string::npos) {
        throw std::runtime_error("Výběr schránky '" + mailboxName + "' selhal.");
    }
}

std::string IMAPClient::receiveResponse() {
    char buffer[4096];
    std::string response;
    int bytes = 0;

    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);


    while (true) {
        memset(buffer, 0, sizeof(buffer));
        if (config.isTLSEnabled()) {
            bytes = SSL_read(ssl, buffer, sizeof(buffer));
        } else {
            bytes = recv(sockfd, buffer, sizeof(buffer), 0);
        }

        if (bytes > 0) {
            response.append(buffer, bytes);
        } else if (bytes == 0) {
            break;
        } else if (bytes < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                break;
            } else {
                std::cerr << "Error receiving response: " << strerror(errno) << std::endl;
                break;
            }
        }

        // Kontrola konce odpovědi
        if (response.find(getCurrentCommandTag() + " OK") != std::string::npos ||
            response.find(getCurrentCommandTag() + " BAD") != std::string::npos ||
            response.find(getCurrentCommandTag() + " NO") != std::string::npos) {
            break;
        }
    }


    return response;
}

void IMAPClient::saveEmail(const std::string& emailData, const std::string& uid, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    createDirectoryIfNotExists(config.getOutputDir());
    if (!outFile.is_open()) {
        throw std::runtime_error("Nelze otevřít soubor pro zápis: " + filename);
    }

    std::istringstream emailStream(emailData);
    std::string line;
    bool headersComplete = false;

    // Přeskočení prvního řádku, pokud je prázdný nebo obsahuje jen bílé znaky
    if (std::getline(emailStream, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            std::getline(emailStream, line);
        }
    }

    // Zpracování hlaviček a těla zprávy
    while (std::getline(emailStream, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos && !headersComplete) {
            headersComplete = true;
            if (!config.downloadHeadersOnly()) {
                outFile << "\n";
            }
            continue;
        }

        if (!headersComplete) {
            outFile << line << "\n";
        } else {
            if (config.downloadHeadersOnly()) {
                break;
            }
            outFile << line << "\n";
        }
    }

    outFile.close();
}


std::string IMAPClient::getNextCommandTag() {
    char tag[5];
    snprintf(tag, sizeof(tag), "a%03d", commandTagCounter++);
    return std::string(tag);
}

std::string IMAPClient::getCurrentCommandTag() const {
    char tag[5];
    snprintf(tag, sizeof(tag), "a%03d", commandTagCounter - 1);
    return std::string(tag);
}

void IMAPClient::createDirectoryIfNotExists(const std::string& directory) {
    struct stat info;
    
    // Zkontroluje, zda složka již existuje
    if (stat(directory.c_str(), &info) != 0) {
        // Složka neexistuje, pokusí se ji vytvořit
        if (mkdir(directory.c_str(), 0777) != 0) {
            throw std::runtime_error("Nelze vytvořit složku: " + directory);
        }
    } else if (!(info.st_mode & S_IFDIR)) {
        // Cesta existuje, ale není to složka
        throw std::runtime_error(directory + " existuje, ale není to složka.");
    }
}

bool IMAPClient::fileExistsWithPrefix(const std::string& prefix) {
    DIR *dir;
    struct dirent *ent;
    
    

    if ((dir = opendir(config.getOutputDir().c_str())) != NULL) {


        while ((ent = readdir(dir)) != NULL) {
            std::string fileName = ent->d_name;

            if (fileName.rfind(prefix, 0) == 0) { 
                closedir(dir);
                return true;
            }
        }

        closedir(dir);
    } else {

    }

    return false;
}

bool IMAPClient::isSameMailboxStored(const std::string& mailboxName) {
    DIR *dir;
    struct dirent *ent;
    bool isSame = true;

    if ((dir = opendir(config.getOutputDir().c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = std::string(ent->d_name);
            if (filename.find("UID_") == 0) {
                size_t mailboxPos = filename.find_last_of('_');
                std::string storedMailbox = filename.substr(mailboxPos + 1, filename.length() - mailboxPos - 5);
                if (storedMailbox != mailboxName) {
                    isSame = false;
                    break;
                }
            }
        }
        closedir(dir);
    }

    return isSame;
}

void IMAPClient::clearOutputDirectory() {
    DIR *dir;
    struct dirent *ent;
    std::string outputDir = config.getOutputDir();

    if ((dir = opendir(outputDir.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = std::string(ent->d_name);
            if (filename.find("UID_") == 0) {
                std::string filepath = outputDir + "/" + filename;
                std::remove(filepath.c_str());
            }
        }
        closedir(dir);
    }
}
