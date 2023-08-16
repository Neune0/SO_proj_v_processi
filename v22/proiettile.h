#ifndef PROIETTILE_H
#define PROIETTILE_H
#include "utilities.h"

pid_t avviaProiettile(int* pipe_fd, PipeData *shooter,int id);
void moveProiettile(int* pipe_fd, PipeData *shooter,int id);


//void proiettile(int* pipe_fd, PipeData* obj);
//pid_t creaProiettile(int* pipe_fd, PipeData* obj);

#endif
