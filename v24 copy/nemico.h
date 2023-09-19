#ifndef NEMICO_H
#define NEMICO_H
#include "utilities.h"
pid_t avviaNemico(int* pipe_fd,PipeData* pipeData,int id);
void nemico(int *pipe_fd,PipeData* pipeData,int id);

#endif
