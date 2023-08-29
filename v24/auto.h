#ifndef AUTO_H
#define AUTO_H
#include "utilities.h"
#define NUMAUTO 8
void camion(int* pipe_fd, int y, int direzione_x, int id);
void macchina(int* pipe_fd, int y, int direzione_x, int id);
pid_t avviaMacchina(int* pipe_fd,int row_y,int *dir_auto,int id);
void gestoreMacchine(int* pipe_fd, pid_t* pid_veicoli);
#endif
