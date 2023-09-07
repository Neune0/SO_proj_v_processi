#include "proiettileNemico.h"

pid_t avviaProiettileNemico(int* pipe_fd, PipeData *shooter,int id){
	pid_t proiettile_nemico_pid = fork();
    
  if (proiettile_nemico_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (proiettile_nemico_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
     	proiettileNemico(pipe_fd, shooter,id);
      exit(0);
  }
  else{
  	// sono nel padre
  	return proiettile_nemico_pid;
  }
}
void proiettileNemico(int *pipe_fd, PipeData *shooter,int id) {
		int dirY = 1;
		PipeData proiettileNemico;
		proiettileNemico.x=shooter->x;	// le coordinate iniziali del proiettile sono quelle dell' oggetto che ha sparato
		proiettileNemico.y=shooter->y;
		proiettileNemico.type='p';
		proiettileNemico.id=id;
		
    while (1) {
    		proiettileNemico.y = proiettileNemico.y + dirY;
        write(pipe_fd[1], &proiettileNemico, sizeof(PipeData));
        usleep(100000);// Aspetta un po' prima di generare nuove coordinate
    }
}


void uccidiProiettileNemico( pid_t *array_pid_proiettili, int id_proiettile){
	if((id_proiettile != -1) && (array_pid_proiettili[id_proiettile] != 0))
	{
		kill(array_pid_proiettili[id_proiettile], SIGKILL);
		waitpid(array_pid_proiettili[id_proiettile],NULL,0);
		array_pid_proiettili[id_proiettile]=0;
	}
}











