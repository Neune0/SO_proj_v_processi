#include "proiettile.h"

void avviaProiettile(int* pipe_fd){
	pid_t move_pid = fork();
    
  if (move_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (move_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
      moveProiettile(pipe_fd);
      exit(0);
  }
}
void moveProiettile(int* pipe_fd) {
		int dirY = -1;
		PipeData pipeData;
		pipeData.x=30;
		pipeData.y=30;
		pipeData.type='P';
		pipeData.id=0;
		
    while (1) {
    		//pipeData.type='X'; // resetta il normale carattere della rana
    		pipeData.y= pipeData.y + dirY;
        write(pipe_fd[1], &pipeData, sizeof(PipeData));

        // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
        usleep(100000);
        
        // beep di debug
        if(pipeData.y < -1){ 
        	beep();
        	break;
        }
    }
    return;
}


