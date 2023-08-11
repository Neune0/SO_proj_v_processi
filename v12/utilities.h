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
// TODO: da rendere tipo, si risparmiano caratteri nella dichiarazione delle funzioni, cosi non si deve andare a capo se molti parametri
// Definizione della struttura dati per le coordinate (x, y) e per il tipo
struct PipeData {
    int x;
    int y;
    char type;
    int id;
};

void inizializzaNcurses();
void creaPipe(int pipe_fd[2]);
void inizializzaColorazione();

#endif // UTILITIES_H

