#include "proiettile.h"

pid_t avviaProiettile(int* pipe_fd, PipeData *shooter,int id){
	pid_t move_pid = fork();
    
  if (move_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (move_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
      moveProiettile(pipe_fd, shooter,id);
      exit(0);
  }
  else{
  	// sono nel padre
  	return move_pid;
  }
}
void moveProiettile(int *pipe_fd, PipeData *shooter,int id) {
		int dirY = -1;
		PipeData proiettile;
		proiettile.x=shooter->x;	// le coordinate iniziali del proiettile sono quelle dell' oggetto che ha sparato
		proiettile.y=shooter->y;
		proiettile.type='P';
		proiettile.id=id;
		
    while (1) {
    		proiettile.y = proiettile.y + dirY;
        write(pipe_fd[1], &proiettile, sizeof(PipeData));
        usleep(100000);// Aspetta un po' prima di generare nuove coordinate
    }
}

// uccide il processo proiettile corrispondente all' id passato
void uccidiProiettile( pid_t *array_pid_proiettili, int id_proiettile){
	if((id_proiettile != -1) && (array_pid_proiettili[id_proiettile] != 0))
	{ 
		kill(array_pid_proiettili[id_proiettile], SIGKILL);
		waitpid(array_pid_proiettili[id_proiettile],NULL,0);
		array_pid_proiettili[id_proiettile]=0;
	}
}




