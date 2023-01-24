#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

//			 MESSAGEBOX NATIVE LIBRARY
		#pragma comment (lib, "User32.lib")
//
void CONSOLE_CLEAR() {
	system("cls");
}
void COLOR_RESET() {
	printf("\033[0m");
}
void CONSOLE_FLUSH() {
    fflush(stdin);
}
int CONSOLE_READ_INT() {
    int n;
    scanf("%i", &n);
    CONSOLE_FLUSH();
    return n;
}
int GET_COLS() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
}
int GET_ROWS() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int rows;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return rows;
}
void CONSOLE_SETCURSOR(int x, int y) {
     HANDLE hStdout;
	 CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	 hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
 if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
    {
		 MessageBox(NULL, "GetConsoleScreenBufferInfo", "Console Error", MB_OK);
    }
	 csbiInfo.dwCursorPosition.X = x;
	 csbiInfo.dwCursorPosition.Y = y;
 if (! SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition))
    {
		 MessageBox(NULL, "SetConsoleCursorPosition", "Console Error", MB_OK);
    }
}
int RANDOM_IN_RANGE(int min, int max) {
	return rand() % (max - min + 1) + min;
}
void COLOR_SET(char color[]) {
	printf(color);
}
char CONSOLE_READKEY () {
    return getch();
}
void CONSOLE_PRINT_CENTERED(char text[]) {
    int len = strlen(text);
    int consoleWidth = GET_COLS();
    for(int i = 0; i < (consoleWidth - len) / 2; i++)
        printf(" ");
    printf("%s", text);
}
void CHAR_REPEAT(int len, char ch) {
    for(int i = 0; i < len; i++)
        printf("%c", ch);
}
void CONSOLE_PRINT_INTERLINE() {
    CHAR_REPEAT(GET_COLS(), '=');
}
int MESSAGEBOX_NORMAL(char text[], char title[]) {
    MessageBox(NULL, text, title, 0);
}
int MESSAGEBOX_ERROR(char text[], char title[]) {
    MessageBox(NULL, text, title, MB_ICONERROR);
}
int MESSAGEBOX_INFO(char text[], char title[]) {
    MessageBox(NULL, text, title, MB_ICONINFORMATION);
}
int MESSAGEBOX_ALERT(char text[], char title[]) {
    MessageBox(NULL, text, title, MB_ICONEXCLAMATION);
}
int MESSAGEBOX_YESNO(char text[], char title[]) {
    int m = MessageBox(NULL, text, title, MB_ICONHAND | MB_YESNO | MB_DEFBUTTON1);
    switch(m) {
        case IDYES:
            m = 1;
            break;
        case IDNO:
            m = 0;
            break;
    }
    return m;
}
void SYS_SHUTDOWN() {
    system("shutdown /s");
}
void SYS_LOGOUT() {
    system("shutdown /l");
}
void SYS_SET_TITLE(char title[]) {
    char sas[255];
    sprintf(sas, "title %s", title);
    system(sas);
}
void FILE_READ_EACH_LINE(const char* fname, void(*cb)(const char*)) {
	FILE* fptr = fopen(fname, "r");
	if(!fptr) return;

	while(!feof(fptr)) {
		const char* buffer = (char*)malloc(MAX_STR);
		fgets(buffer, MAX_STR, fptr);
		cb(buffer);
		free(buffer);
	}

	fclose(fptr);
}

#pragma region METODI STRINGHE

int mia_strlen(char* str) {
	int c;
	for (c = 0; *str != 0; c++) str++;
	return c;
}
void mia_strcpy(char* dest, const char* src) {
	int l = mia_strlen(src);

	for (int i = 0; i < l + 1; i++)
		*(dest + i) = *(src + i);
}
void mia_gets(char* str) {
	int nch = 0;
	char* inserito = malloc(sizeof(char));
	if (!inserito) return;

	do
	{
		*inserito = getchar();
		*(str + nch) = *inserito;
		nch++;
	} while (*inserito != '\n');

	*(str + nch - 1) = 0; // Metto il tappo

	free(inserito);
}
void mia_strrev(char* str) {
	int l = mia_strlen(str);

	char* aus = malloc(sizeof(char));
	if (!aus) return;

	for (int i = 0; i < l / 2; i++)
	{
		*aus = *(str + i);
		*(str + i) = *(str + l - 1 - i);
		*(str + l - 1 - i) = *aus;
	}

	free(aus);
}
int mia_split(char* s, char separ, char** v) {
	int i = 0, j = 0, is = 0;
	do
	{
		if (s[i] == separ) {
			v[j][is] = 0;
			j++;
			is = 0;
		}
		else {
			v[j][is] = s[i];
			is++;
		}
		i++;
	} while (s[i] != 0 && j < MAX_SPLIT);
	return j;
}

#pragma endregion
