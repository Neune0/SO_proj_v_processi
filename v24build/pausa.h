#ifndef PAUSA_H
#define PAUSA_H
#include <sys/wait.h>
#include <signal.h>
#include "utilities.h"
extern volatile sig_atomic_t child_running;

void handle_stop(int signum);

void handle_continue(int signum);

int pausa();

#endif
