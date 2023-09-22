#ifndef DEBUG_H
#define DEBUG_H
#include "utilities.h"
void printDebug(pid_t pidRana,
								pid_t* array_pid_proiettili,
								pid_t* array_pid_proiettili_nemici,
								pid_t* array_pid_nemici,
								pid_t* pid_veicoli,
								pid_t* pid_tronchi,
								int contatore_proiettili_in_gioco,
								int contatore_nemici_in_gioco,
								int contatore_proiettili_nemici_in_gioco,
								PipeData pipeData,
								PipeData* old_pos);
#endif
