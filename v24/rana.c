#include "rana.h"
pid_t avviaRana(int* pipe_fd){
	pid_t move_pid = fork();
    
  if (move_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (move_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
      moveProcess(pipe_fd);
      exit(0);
  }
	return move_pid;
}
void moveProcess(int* pipe_fd) {
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
    signal(SIGSTOP, handle_stop);
    // Installa il gestore di segnale per SIGCONT
    signal(SIGCONT, handle_continue);
		
		 // Invia le coordinate iniziali attraverso la pipe
    write(pipe_fd[1], &pipeData, sizeof(PipeData));
		
		noecho();
    while (1) {
    		pipeData.type='X'; // resetta il normale carattere della rana
        
        // Leggi il carattere dall'input
        int ch = getch();
        if (ch != ERR) {
            
            // Muovi il personaggio in base all'input dell'utente
            switch (ch) {
                case KEY_UP:
                	if(pipeData.y>minAreaGiocoY){pipeData.y--;}
                  break;
                case KEY_DOWN:
                	if(pipeData.y<maxAreaGiocoY){pipeData.y++;}
                  break;
                case KEY_LEFT:
                	if(pipeData.x>0){pipeData.x--;}  
                  break;
                case KEY_RIGHT:
                	if(pipeData.x<maxAreaGiocoX){pipeData.x++;}
                  break;
                case 32: // KEY_SPACE 
                	pipeData.type='S'; //cambia carattere per dire a processoDisegna che  rana sta sparando
                  break;
                case 'p': // tasto z  PAUSA
                case 'P':
                	pipeData.type = 'Z'; 
                	break;
            }

            // Invia le coordinate attraverso la pipe
            write(pipe_fd[1], &pipeData, sizeof(PipeData));
        }

        // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
        usleep(100000);
    }
    return;
}
