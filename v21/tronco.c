#include "tronco.h"

void avviaGestoreTronchi(int* pipe_fd){
	pid_t gestore_figli_pid = fork();
  if(gestore_figli_pid < 0){
  	perror("Fork failed");
    exit(1);
  }else{
  	if(gestore_figli_pid == 0){
    	gestoreTronchi(pipe_fd); // avvio gestore tronchi
    }
  }
  return;
}

void gestoreTronchi(int* pipe_fd){
	int num_tronchi = NTRONCHI;
  int dir_tronco[NTRONCHI];
  dir_tronco[0] = (rand() % 2 == 1) ? 1 : -1;
    		
  for (int i = 1; i < num_tronchi; i++) {
  	dir_tronco[i] = -1 * dir_tronco[i - 1];
	}
					
  avviaTronco(pipe_fd,YTRONCOUNO,dir_tronco,1);
  avviaTronco(pipe_fd,YTRONCODUE,dir_tronco,2);
  avviaTronco(pipe_fd,YTRONCOTRE,dir_tronco,3);
				
	//aspetta termine dei figli-tronchi
	wait(NULL);
	wait(NULL);
	wait(NULL);
	exit(0);
}

void avviaTronco(int* pipe_fd,int x_spawn,int *dir_tronco,int id){
	pid_t tronco_pid = fork();
	if (tronco_pid < 0) {
		perror("Fork failed");
		exit(1);
	}
	else{
		if (tronco_pid == 0) {
			close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
			tronco(pipe_fd,x_spawn, dir_tronco[id-1] ,id);
			exit(0);
		}
	}
	return;
}

void tronco(int* pipe_fd, int y, int direzione_x, int id) {
   	
   	// determina random la coordinata_X  di spawn del tronco 
   	int i=0;
   	int spawn_rand;
   	do{
   		 spawn_rand = rand() % 100;
   		 i++;
   	}while(i!=id);
   	
		PipeData pipeData;
		pipeData.x=spawn_rand;
		pipeData.y=y;
		pipeData.type='T';
		pipeData.id=id;
				
		int lunghezza_tronco= 9;
		
		int direzione = direzione_x;
		
    while (1) {
    	
    	if(direzione==1){
    		if(pipeData.x + lunghezza_tronco + 1 < WIDTH){
      		pipeData.x++;
      	}
      	else{
      		direzione*=-1;
      	}
    	}else{
    		if(pipeData.x - 1 > 0){
    			pipeData.x--;
    		}
    		else{
    			direzione*=-1;
    		}
    	}
      // Invia le coordinate attraverso la pipe
      write(pipe_fd[1], &pipeData, sizeof(PipeData));

      // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
      usleep(100000);
    }
}
