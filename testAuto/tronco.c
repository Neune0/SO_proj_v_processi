#include "tronco.h"
void tronco(int* pipe_fd, int y, int direzione_x, int id) {
   	
   	// determina random la coordinata_X  di spawn del tronco 
   	int i=0;
   	int spawn_rand;
   	do{
   		 spawn_rand = rand() % 100;
   		 i++;
   	}while(i!=id);
   	
		struct PipeData pipeData;
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
    	//pipeData.x+=direzione;
      // Invia le coordinate attraverso la pipe
      write(pipe_fd[1], &pipeData, sizeof(struct PipeData));

      // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
      usleep(100000);
    }
}//end tronco


