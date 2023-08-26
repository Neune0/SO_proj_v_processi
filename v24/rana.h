#ifndef RANA_H
#define RANA_H
#include "utilities.h"
pid_t avviaRana(int* pipe_fd);
void moveProcess(int* pipe_fd);
#endif
