#include "auto.h"
void avviaGestoreMacchine(int* pipe_fd){
	pid_t gestore_auto_pid = fork();
  if (gestore_auto_pid < 0) {
		perror("Fork failed");
		exit(1);
	} 
	else{
		if (gestore_auto_pid == 0) { 
			gestoreMacchine(pipe_fd);
		}
	}
	return;
}

void gestoreMacchine(int* pipe_fd){
	// determina direzione delle corsie 
	int dir_auto[ NUMAUTO ];
	dir_auto[0] = (rand() % 2 == 1) ? 1 : -1;
				
	for (int i = 1; i < NUMAUTO; i++) {
  	dir_auto[i] = -1 * dir_auto[i - 1];
	}
				
	avviaMacchina(pipe_fd,22,dir_auto,4); // macchina 1
	avviaMacchina(pipe_fd,24,dir_auto,5); // macchina 2
	avviaMacchina(pipe_fd,27,dir_auto,6); // macchina 3
	avviaMacchina(pipe_fd,29,dir_auto,7); // macchina 4
				
	wait(NULL); //aspetta fine di macchina1
	wait(NULL); //aspetta fine di macchina2
	wait(NULL); //aspetta fine di macchina3
	wait(NULL); //aspetta fine di macchina4
	exit(0);
}

void avviaMacchina(int* pipe_fd,int row_y,int *dir_auto,int id){
	pid_t macchina_pid = fork();		// crea processo per macchina1
	if (macchina_pid < 0) {
		perror("Fork failed");
		exit(1);
	}
	else{
		if (macchina_pid == 0) {
			// figlio-macchina 1
			int direzione_x = dir_auto[id-4];
			close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
			macchina(pipe_fd, row_y, direzione_x, id);
			exit(0);
		}
	}
	return;
}
void macchina(int* pipe_fd, int y, int direzione_x, int id){
	   	// determina random la coordinata_X  di spawn della macchina 
   	int i=0;
   	int spawn_rand;
   	do{
   		 spawn_rand = rand() % 100;
   		 i++;
   	}while(i!=id);
   	
		PipeData pipeData;
		//pipeData.x=5;
		
		pipeData.x=spawn_rand;
		pipeData.y=y;
		pipeData.type='A';
		pipeData.id=id;
				
		int lunghezza_auto= 4;
		
		int direzione = direzione_x;
		
    while (1) {
    	
    	if(direzione==1){
    		if(pipeData.x + lunghezza_auto + 1 < WIDTH){
      		//pipeData.x++;
      	}
      	else{	// se l'auto esce dallo schermo a destra, ricompare a sinistra 
      		//direzione*=-1;
      		//pipeData.x= lunghezza_auto;
      		pipeData.x= 1;
      	}
    	}else{
    		if(pipeData.x - 1 > 0){
    			//pipeData.x--;
    		}
    		else{
    			//direzione*=-1;
    			pipeData.x= WIDTH - lunghezza_auto;
    		}
    	}
    	pipeData.x+=direzione;
      // Invia le coordinate attraverso la pipe
      write(pipe_fd[1], &pipeData, sizeof(PipeData));

      // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
      usleep(100000);
    }
}//end macchina
