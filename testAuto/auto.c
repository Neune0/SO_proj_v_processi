#include "auto.h"
void macchina(int* pipe_fd, int y, int direzione_x, int id){
	   	// determina random la coordinata_X  di spawn della macchina 
   	int i=0;
   	int spawn_rand;
   	do{
   		 spawn_rand = rand() % 100;
   		 i++;
   	}while(i!=id);
   	
		struct PipeData pipeData;
		pipeData.x=spawn_rand;
		pipeData.y=y;
		pipeData.type='A';
		pipeData.id=id;
				
		int lunghezza_auto= 4;
		
		int direzione = direzione_x;
		
    while (1) {
    	
    	if(direzione==1){
    		if(pipeData.x + lunghezza_auto + 1 < WIDTH){
      		pipeData.x++;
      	}
      	else{	// se l'auto esce dallo schermo a destra, ricompare a sinistra 
      		//direzione*=-1;
      		pipeData.x= lunghezza_auto;
      	}
    	}else{
    		if(pipeData.x - 1 > 0){
    			pipeData.x--;
    		}
    		else{
    			//direzione*=-1;
    			pipeData.x= WIDTH - lunghezza_auto;
    		}
    	}
    	//pipeData.x+=direzione;
      // Invia le coordinate attraverso la pipe
      write(pipe_fd[1], &pipeData, sizeof(struct PipeData));

      // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
      usleep(100000);
    }
}//end macchina
