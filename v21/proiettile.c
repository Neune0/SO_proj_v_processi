#include "proiettile.h"

void avviaProiettile(int* pipe_fd, PipeData *shooter ,int *contatore_proiettili){
	pid_t move_pid = fork();
    
  if (move_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (move_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
      moveProiettile(pipe_fd, shooter);
      (*contatore_proiettili)--;
      exit(0);
  }
}
void moveProiettile(int *pipe_fd, PipeData *shooter) {
		int dirY = -1;
		PipeData proiettile;
		proiettile.x=shooter->x;	// le coordinate iniziali del proiettile sono quelle dell' oggetto che ha sparato
		proiettile.y=shooter->y;
		proiettile.type='P';
		proiettile.id=0;
		
    while (proiettile.y>1) {
    		proiettile.y = proiettile.y + dirY;
    		
        write(pipe_fd[1], &proiettile, sizeof(PipeData));
        
        usleep(100000);// Aspetta un po' prima di generare nuove coordinate
    }
}


