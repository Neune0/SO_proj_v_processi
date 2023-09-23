#include "nemico.h"
pid_t avviaNemico(int* pipe_fd,int id){
	pid_t nemico_pid = fork();
	if (nemico_pid < 0) {
		perror("Fork failed");
		exit(1);
	}
	else{
		if (nemico_pid == 0) {
			close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
			nemico(pipe_fd,id);
			exit(0);
		}
		else{
			return nemico_pid;
		}
	}
	return nemico_pid;
}

void nemico(int *pipe_fd,int id) { // l'id fa riferimento all'id del tronco su cui il processo nemico sta
		
		PipeData nemico;
		nemico.x=0;
		nemico.y=0;
		nemico.type='s';
		nemico.id=id;
		int contatore=0;
    while (1) {
    		if(contatore%100==0){
    			write(pipe_fd[1], &nemico, sizeof(PipeData));
    		}
    		contatore=(contatore+1)%50;
        usleep(100000);// Aspetta un po' prima di generare nuove coordinate
    }
}


void killNemico(pid_t pid_nemico){
		kill(pid_nemico, SIGKILL);
		waitpid(pid_nemico, NULL,0);
		return;
}
