#ifndef RANA_H
#define RANA_H
//#include "pausa.h"
#include "utilities.h"
pid_t avviaRana(int* pipe_fd, int* pipe_rana);
void moveProcess(int* pipe_fd, int* pipe_rana);
pid_t resetRana( int* pipe_fd, int* pipe_rana, pid_t pid_processo_rana);
#endif

