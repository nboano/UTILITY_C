/*
    ███    █▄      ███      ▄█   ▄█        ▄█      ███      ▄█     ▄████████    ▄████████ 
    ███    ███ ▀█████████▄ ███  ███       ███  ▀█████████▄ ███    ███    ███   ███    ███ 
    ███    ███    ▀███▀▀██ ███▌ ███       ███▌    ▀███▀▀██ ███▌   ███    █▀    ███    █▀  
    ███    ███     ███   ▀ ███▌ ███       ███▌     ███   ▀ ███▌  ▄███▄▄▄       ███        
    ███    ███     ███     ███▌ ███       ███▌     ███     ███▌ ▀▀███▀▀▀     ▀███████████ 
    ███    ███     ███     ███  ███       ███      ███     ███    ███    █▄           ███ 
    ███    ███     ███     ███  ███▌    ▄ ███      ███     ███    ███    ███    ▄█    ███ 
    ████████▀     ▄████▀   █▀   █████▄▄██ █▀      ▄████▀   █▀     ██████████  ▄████████▀  
                            ▀                                                         
    Niccolò Boano                                                               2020 - 2023
    ╔═════════════════════════════════════════════════════════════════════════════════════╗
    ║                                     SOCKET.C                                        ║
    ║   Gestione dei socket in ambiente Windows.                                          ║
    ║   Implementazione lettura e scrittura da socket e binding a porta.                  ║
    ║                                                                                     ║
    ║   NEW_SOCKET(mysocket, BUFFER_SIZE);        // (FUORI DAL MAIN)                     ║
    ╚═════════════════════════════════════════════════════════════════════════════════════╝
*/
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

//         WINDOWS SOCKET 2 LIBRARY
            #include <winsock2.h>
            #include <ws2tcpip.h>
        #pragma comment (lib, "Ws2_32.lib")
//

/// @brief Usa questa struct per gestire un socket in ambiente Windows.
typedef struct t_socket {
    /// @brief Puntatore al buffer in ricezione del socket, dove vengono scritti i dati ricevuti.
    char* Buffer;

    /// @brief Inizializza il socket.
    void (*Init)();

    /// @brief Collega il socket a una porta. Ogni richiesta entrante dalla porta sarà gestita dal socket.
    /// @param Port Il numero di porta.
    void (*Listen)(int Port);

    /// @brief Attende in maniera sincrona una connessione al socket da parte di un client.
    /// @return L'indirizzo IP del client che si è appena connesso
    char* (*WaitForConnection)();

    /// @brief Legge una stringa in input sul socket.
    /// @return La stringa letta, accessibile anche tramite .Buffer
    char* (*Read)();

    /// @brief Scrive una stringa in output sul socket.
    /// @param Text Il testo da scrivere.
    void (*Write)(const char* Text);

    /// @brief Chiude la connessione corrente.
    void (*Close)();

    SOCKET ListenSocket;
    SOCKET ClientSocket;
} Socket;

/// @brief Crea un nuovo socket.
/// @param this Nome della variabile che rappresenterà il nuovo socket.
/// @param bufferlen Lunghezza del buffer in ricezione.
#define NEW_SOCKET(this, bufferlen) \
Socket this;\
void INIT_##this() {\
    WSADATA wsaData;\
    WSAStartup(MAKEWORD(2,2), &wsaData);\
    this.Buffer = (char*)malloc(bufferlen);\
};\
void LISTEN_##this(int port_number) {\
    struct addrinfo* result = NULL;\
    struct addrinfo hints = {AI_PASSIVE, AF_INET, SOCK_STREAM, IPPROTO_TCP};\
    char portbf[6] = "";\
    sprintf(portbf, "%i", port_number);\
    getaddrinfo(NULL, portbf, &hints, &result);\
\
    this.ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);\
    bind(this.ListenSocket, result->ai_addr, (int)result->ai_addrlen);\
    freeaddrinfo(result);\
    listen(this.ListenSocket, SOMAXCONN);\
};\
char* WAITFORCONNECTION_##this() {\
    SOCKADDR_IN addr;   \
    int addrlen = sizeof(addr);\
    this.ClientSocket = accept(this.ListenSocket, (SOCKADDR*)&addr, &addrlen);\
    return inet_ntoa(addr.sin_addr);\
}\
char* READ_##this() {\
    int l = recv(this.ClientSocket, this.Buffer, bufferlen, 0);\
    this.Buffer[l] = 0;\
    if(l <= 0) return 0;\
    return this.Buffer;\
}\
void WRITE_##this(const char* str) {\
    send(this.ClientSocket, str, strlen(str), 0);\
}\
void CLOSE_##this() {\
    closesocket(this.ClientSocket);\
}\
Socket this = {\
    NULL,\
    INIT_##this,\
    LISTEN_##this,\
    WAITFORCONNECTION_##this,\
    READ_##this,\
    WRITE_##this,\
    CLOSE_##this,\
    INVALID_SOCKET,\
    INVALID_SOCKET,\
};\
