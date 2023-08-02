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
// Definizione della struttura dati per le coordinate (x, y) e per il tipo
struct PipeData {
    int x;
    int y;
    char type;
    int id;
};



// Il tuo codice dell'header qui

#endif // UTILITIES_H

