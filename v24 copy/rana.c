#include "rana.h"
pid_t avviaRana(int* pipe_fd, int* pipe_rana){
	pid_t move_pid = fork(); // pid che contiente il pid della rana
    
  if (move_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (move_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
      moveProcess(pipe_fd, pipe_rana);
      exit(0);
  }
	return move_pid;
}
void moveProcess(int* pipe_fd, int* pipe_rana) {
		// limiti dell' area di gioco per la rana
		int minAreaGiocoX = 0;
		int minAreaGiocoY = 4;
		int maxAreaGiocoX = WIDTH -3;
		int maxAreaGiocoY = HEIGHT-6;
		
		PipeData pipeData;
		pipeData.x=WIDTH/2; // coordinate iniziali
		pipeData.y=HEIGHT-6;
		pipeData.type='X';
		
		// Installa il gestore di segnale per SIGSTOP
    //signal(SIGSTOP, handle_stop);
    // Installa il gestore di segnale per SIGCONT
    //signal(SIGCONT, handle_continue);
		
		 // Invia le coordinate iniziali attraverso la pipe
    write(pipe_fd[1], &pipeData, sizeof(PipeData));
		noecho();
		close(pipe_rana[1]);
    while (1) {
    			
    		 // legge le coordinate della rana inviate da disegna, eventualmente modificate.
        //read(pipe_rana[0], &pipeData, sizeof(PipeData)); 
       
        //pipeData.x = 0;
  			//pipeData.y =  0;
    		pipeData.type='X'; // resetta il normale carattere della rana
       	
        // Leggi il carattere dall'input
        int ch = getch();
        fflush(stdin);
       
        if (ch != ERR) {
            
            // Muovi il personaggio in base all'input dell'utente
            switch (ch) {
                case KEY_UP:
                	if(pipeData.y>minAreaGiocoY){
                		pipeData.y--;
                		//pipeData.y = -1;
              		}
                  break;
                case KEY_DOWN:
                	if(pipeData.y<maxAreaGiocoY){
                		pipeData.y++;
                		//pipeData.y = +1;
              		}
                  break;
                case KEY_LEFT:
                	if(pipeData.x>0){
                		pipeData.x--;
                		//pipeData.x = -1;
                	}  
                  break;
                case KEY_RIGHT:
                	if(pipeData.x<maxAreaGiocoX){
                		pipeData.x++;
                		//pipeData.x = +1;
                	}
                  break;
                case 32: // KEY_SPACE 
                	pipeData.type='S'; //cambia carattere per dire a processoDisegna che  rana sta sparando
                  break;
                case 'p': // tasto p  PAUSA
                case 'P':
                	pipeData.type = 'Z'; 
                	break;
            }

            // Invia le coordinate attraverso la pipe
            write(pipe_fd[1], &pipeData, sizeof(PipeData));
        }
        // vuota buffer input
      	do{
      		getch();
      	}while(getch()!= ERR);

        // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
        usleep(100000);
         
    }//end while
    return;
}//end moveProcess 


pid_t resetRana(int* pipe_fd, int* pipe_rana, pid_t pid_processo_rana){
		pid_t newPid;
		kill(pid_processo_rana, SIGKILL);
		waitpid(pid_processo_rana, NULL,0);
		newPid = avviaRana(pipe_fd, pipe_rana);
		return newPid;
}





