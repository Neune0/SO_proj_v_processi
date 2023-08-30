#include "auto.h"

void gestoreMacchine(int* pipe_fd, pid_t* pid_veicoli){
	// determina direzione delle corsie 
	int dir_auto[ NUMAUTO ];
	dir_auto[0] = (rand() % 2 == 1) ? 1 : -1;
	
	for (int i = 1; i < NUMAUTO; i++) {
  	dir_auto[i] = -1 * dir_auto[i - 1];
	}
				
	pid_veicoli[0]= avviaMacchina(pipe_fd,22,dir_auto,4); // macchina 1
	pid_veicoli[1]= avviaMacchina(pipe_fd,24,dir_auto,5); // macchina 2
	pid_veicoli[2]= avviaMacchina(pipe_fd,27,dir_auto,6); // macchina 3
	pid_veicoli[3]= avviaMacchina(pipe_fd,29,dir_auto,7); // macchina 4
	sleep(2);
	pid_veicoli[4]= avviaMacchina(pipe_fd,22,dir_auto,8); // macchina 1
	pid_veicoli[5]= avviaMacchina(pipe_fd,24,dir_auto,9); // macchina 2
	pid_veicoli[6]= avviaMacchina(pipe_fd,27,dir_auto,10); // macchina 3
	pid_veicoli[7]= avviaMacchina(pipe_fd,29,dir_auto,11); // macchina 4
	return;
}

pid_t avviaMacchina(int* pipe_fd,int row_y,int *dir_auto,int id){
	pid_t macchina_pid = fork();		// crea processo per macchina1
	if (macchina_pid < 0) {
		perror("Fork failed");
		exit(1);
	}
	else{
		if (macchina_pid == 0) {
			// figlio-macchina 1
			int direzione_x = dir_auto[id-4];
			int _rand;
			// "brucia" un numero di tiri pari all'id in modo che ogni figlo abbia un random diverso
			for(int i=0; i<id; i++){ 
				_rand = rand()%2;
			}
			
			// controllo se la corsia è libera
			
			
			
			// avvia l'auto solo se la corsia è libera 
			close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
			if(_rand){																		//sceglie random se è auto o camion
				macchina(pipe_fd, row_y, direzione_x, id);
			}else{
				camion(pipe_fd, row_y, direzione_x, id);
			}
			exit(0);
		}
	}
	return macchina_pid;
}


void macchina(int* pipe_fd, int y, int direzione_x, int id){
	  int lunghezza_auto= 4;
	  // determina random il tempo di respawn quando esce dall'area di gioco 
   	int respawn_time;
   	respawn_time = rand() % 5; //0-4 secondi
  	
		PipeData pipeData;
		
		if(direzione_x == 1){  // se va a destra
			pipeData.x=0;				// compari a sinistra
		}else{
			pipeData.x=WIDTH - lunghezza_auto; //se va a sinistra, compare a destra
		}
		pipeData.y=y;
		pipeData.type='A';
		pipeData.id=id;
		
		int direzione = direzione_x;
		
		sleep(respawn_time); // attesa random prima di comparire
    while (1) {
    	
    	if(direzione==1){
    		if(pipeData.x + lunghezza_auto + 1 > WIDTH){ // se l'auto esce dallo schermo a destra, ricompare a sinistra
    			pipeData.x= 0;
      	}
      	
    	}else{   // con direzione -1
    		if(pipeData.x - 1 < 0){ // esce a sinistra, ricompare a destra
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



void camion(int* pipe_fd, int y, int direzione_x, int id){
   	int lunghezza_camion= 7;
   	int respawn_time;
   	respawn_time = rand() % 5; //0-4 secondi
   	bool respawn = false;
   	
   	
		PipeData pipeData;
		
		if(direzione_x == 1){  // se va a destra
			pipeData.x=0;				// compari a sinistra
		}else{
			pipeData.x=WIDTH - lunghezza_camion; //se va a sinistra, compare a destra
		}
		pipeData.y=y;
		pipeData.type='C';
		pipeData.id=id;
		
		int direzione = direzione_x;
		
		sleep(respawn_time);
    while (1) {
    	
    	if(direzione==1){
    		if(pipeData.x + lunghezza_camion + 1 > WIDTH){ // se l'auto esce dallo schermo a destra, ricompare a sinistra
    			pipeData.x= 0;
    			respawn = true;
    			pipeData.type='c';
      	}
      	
    	}else{   // con direzione -1
    		if(pipeData.x - 1 < 0){ // esce a sinistra, ricompare a destra
    			pipeData.x= WIDTH - lunghezza_camion;
    			respawn = true;
    			pipeData.type='c';
    		}
    	}
    	pipeData.x+=direzione;
      // Invia le coordinate attraverso la pipe
      write(pipe_fd[1], &pipeData, sizeof(PipeData));
			if(respawn){
				usleep(300000);
				respawn = false;
				pipeData.type='C';
			}
			
      // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
      usleep(100000);
    }
}//end camion

