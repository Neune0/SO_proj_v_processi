#ifndef AUTO_H
#define AUTO_H
#include "utilities.h"
#define NUMAUTO 4
void camion(int* pipe_fd, int y, int direzione_x, int id);
void macchina(int* pipe_fd, int y, int direzione_x, int id);
void avviaMacchina(int* pipe_fd,int row_y,int *dir_auto,int id);
void avviaGestoreMacchine(int* pipe_fd);
void gestoreMacchine(int* pipe_fd);
#endif
