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
    ║                                     UTILITIES.H                                     ║
    ║   Libreria utilities.                                                               ║
    ║   Gestione colori console, MessageBox (Windows), Liste, Socket.                     ║
    ║                                                                                     ║
    ╚═════════════════════════════════════════════════════════════════════════════════════╝
*/
#ifndef UBOANO_H_INCLUDED
#define UBOANO_H_INCLUDED

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <malloc.h>


#include "llist.c"
#include "socket.c"

#pragma warning(disable:4996) // Per il compilatore Microsoft

int GET_COLS();
int GET_ROWS();

void COLOR_SET(char color[]);
void COLOR_RESET();
int RANDOM_IN_RANGE(int min, int max);

void CONSOLE_CLEAR();
void CONSOLE_FLUSH();
char CONSOLE_READKEY();
int CONSOLE_READ_INT();
void CONSOLE_SETCURSOR(int x, int y);
void CONSOLE_PRINT_CENTERED(char text[]);
void CONSOLE_PRINT_INTERLINE();

void CHAR_REPEAT(int len, char ch);

int MESSAGEBOX_NORMAL(char text[], char title[]);
int MESSAGEBOX_ERROR(char text[], char title[]);
int MESSAGEBOX_INFO(char text[], char title[]);
int MESSAGEBOX_ALERT(char text[], char title[]);
int MESSAGEBOX_YESNO(char text[], char title[]);

void SYS_SHUTDOWN();
void SYS_LOGOUT();
void SYS_SET_TITLE(char title[]);

void FILE_READ_EACH_LINE(const char* fname, void(*cb)(const char*));

int mia_strlen(char* str);
void mia_strcpy(char* dest, const char* src);
void mia_gets(char* str);
void mia_strrev(char* str);

#define MAX_STR 128
#define MAX_SPLIT 16
int mia_split(char* s, char separ, char** v);

struct t_console {
	/// @brief Pulisce la console.
	void (*Clear)();

	/// @brief Legge un carattere in console.
    /// @return Il tasto premuto.
	char (*ReadKey)();

	/// @brief Esegue un flush del buffer della console.
	void (*Flush)();

	/// @brief Legge un numero in input.
    /// @return Il numero letto.
	int (*ReadInt)();

	/// @brief Sposta il cursore in una certa posizione.
    /// @param x Posizione orizzontale.
    /// @param y Posizione verticale.
	void (*SetCursorPosition)(int x, int y);

    /// @brief Stampa un testo al centro della console.
    /// @param text Il testo.
    void (*PrintCentered)(char text[]);

    /// @brief Ottiene l'altezza della console.
    /// @return L'altezza della console (in caratteri).
    int (*GetHeight)();

    /// @brief Ottiene la larghezza della console.
    /// @return La larghezza della console (in caratteri).
    int (*GetWidth)();

    /// @brief Stampa un'interlinea in console. (=======)
    void (*PrintInterline)();
};
struct t_random {

    /// @brief Genera un numero casuale in un dato intervallo.
    /// @param min Il numero minimo.
    /// @param max Il numero massimo (escluso).
    /// @return Un numero casuale compreso tra min e max.
    int (*InRange)(int min, int max);
};
struct t_str {
    void (*Concat)(char* str1, const char* str2);
    void (*Copy)(char* destination, const char* source);
    void (*Get)(char* str);
    void (*Reverse)(char* str);
    int (*GetLength)(char* str);
    int (*Split)(char* str, char separatore, char** matrice);
};
struct t_ch {
    void (*Repeat)(int len, char ch);
};
struct t_msgbox {
    int (*Show)(char text[], char title[]);
    int (*Error)(char text[], char title[]);
    int (*Info)(char text[], char title[]);
    int (*Alert)(char text[], char title[]);
    int (*YesNo)(char text[], char title[]);
};
struct t_colors {
    char Black[8];
    char Blue[8];
    char Green[8];
    char Aqua[8];
    char Red[8];
    char Purple[8];
    char Yellow[8];
    char White[8];
    void (*Set)(char color[]);
    void (*Reset)();
};
struct t_sys {
    void (*Beep)(int freq, int duration);
    void (*Shutdown)();
    void (*LogOut)();
    void (*SetWindowTitle)(char title[]);
};
struct t_file {
    void (*ReadEachLine)(const char* fname, void(*cb)(const char*));
};

/// @brief Gestione dei colori in console.
struct t_colors Color = {"\033[0;30m", "\033[0;34m", "\033[0;32m", "\033[0;36m", "\033[0;31m", "\033[0;35m", "\033[0;33m", "\033[0;37m", &COLOR_SET, &COLOR_RESET};

/// @brief Generazione di numeri casuali.
struct t_random Random = { &RANDOM_IN_RANGE };

/// @brief Gestione della console.
struct t_console Console = { &CONSOLE_CLEAR, &CONSOLE_READKEY, &CONSOLE_FLUSH, &CONSOLE_READ_INT, &CONSOLE_SETCURSOR, &CONSOLE_PRINT_CENTERED, &GET_ROWS, &GET_COLS, &CONSOLE_PRINT_INTERLINE };

/// @brief Gestione di stringhe e array di caratteri.
struct t_str String = { &strcat, &strcpy, &mia_gets, &mia_strrev, &mia_strlen, &mia_split };

struct t_ch Char = { &CHAR_REPEAT };

/// @brief Gestione delle finestre di dialogo native Win32.
struct t_msgbox MsgBox = { &MESSAGEBOX_NORMAL, &MESSAGEBOX_ERROR, &MESSAGEBOX_INFO, &MESSAGEBOX_ALERT, &MESSAGEBOX_YESNO };

/// @brief Gestione delle funzioni di sistema.
struct t_sys System = { &Beep, &SYS_SHUTDOWN, &SYS_LOGOUT, &SYS_SET_TITLE };

/// @brief Gestione dei file.
struct t_file File = { &FILE_READ_EACH_LINE };

#include "utilities.c"

#endif // UBOANO_H_INCLUDED
