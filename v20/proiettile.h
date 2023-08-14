#ifndef PROIETTILE_H
#define PROIETTILE_H
#include "utilities.h"

void avviaProiettile(int* pipe_fd, PipeData *shooter);
void moveProiettile(int* pipe_fd, PipeData *shooter);


//void proiettile(int* pipe_fd, PipeData* obj);
//pid_t creaProiettile(int* pipe_fd, PipeData* obj);

#endif
