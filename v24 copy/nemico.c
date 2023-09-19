#include "nemico.h"
pid_t avviaNemico(int* pipe_fd,PipeData* pipeData,int id){
	pid_t nemico_pid = fork();
	if (nemico_pid < 0) {
		perror("Fork failed");
		exit(1);
	}
	else{
		if (nemico_pid == 0) {
			close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
			nemico(pipe_fd, pipeData,id);
			exit(0);
		}
		else{
			return nemico_pid;
		}
	}
	return nemico_pid;
}

void nemico(int *pipe_fd,PipeData* pipeData,int id) {
		
		PipeData nemico;
		nemico.x=pipeData->x;
		nemico.y=pipeData->y;
		nemico.type='s';
		nemico.id=id;
		int contatore=0;
    while (1) {
    		if(contatore%100==0){
    			write(pipe_fd[1], &nemico, sizeof(PipeData));
    		}
    		contatore=(contatore+1)%101;
        usleep(100000);// Aspetta un po' prima di generare nuove coordinate
    }
}
