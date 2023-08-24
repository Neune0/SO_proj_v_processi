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
#include <signal.h>

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

#define OLDPOSDIM 12
#define MAXNPROIETTILI 3
#define MAXNNEMICI 3
#define MAXNPROIETTILINEMICI MAXNPROIETTILI

typedef enum 
{
	RANA_SPRITE, TRONCO_SPRITE, AUTO_SPRITE, CAMION_SPRITE, PROIETTILE_SPRITE, PROIETTILE_NEMICO_SPRITE, NEMICO_SPRITE
}TipoSprite;


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


typedef enum{OPEN, CLOSED } TanaStatus;

typedef struct {
	PipeData info;
	TanaStatus status;
}Tana;


typedef struct{
	int max_row;
	int max_col;
	char **sprite;
	int color;
}Sprite;




void inizializzaNcurses();
void creaPipe(int pipe_fd[2]);
void inizializzaColorazione();

#endif // UTILITIES_H

