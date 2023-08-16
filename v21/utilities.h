#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define WIDTH 104
#define HEIGHT 37
#define RANA 1
#define TRONCHI 2
#define SFONDO 3
#define FIUME 4
#define TANE 5
#define PRATO 6
#define STRADA 7
#define STRISCIA 8
#define MARCIAPIEDE 9
#define AUTO 10
#define MENUBOX 11
#define PROIETTILE 12

// Definizione della struttura dati per le coordinate (x, y) e per il tipo
typedef struct{
    int x;
    int y;
    char type;
    int id;
}PipeData;

typedef struct{
	char ch;
	int color;
	bool is_changed;
}ScreenCell;

void inizializzaNcurses();
void creaPipe(int pipe_fd[2]);
void inizializzaColorazione();

#endif // UTILITIES_H

