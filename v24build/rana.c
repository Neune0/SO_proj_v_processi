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
		pipeData.id=0;
		
		PipeData aus;
		 // Invia le coordinate iniziali attraverso la pipe
    write(pipe_fd[1], &pipeData, sizeof(PipeData));
    
		noecho();
		int ch='D';
    while (1) {
    		bool change=false;
    		ssize_t bytes_read = read(pipe_rana[0], &(aus), sizeof(PipeData));
    		if(bytes_read!=-1){
    			// pipeData = aus
    			pipeData.x=aus.x; // coordinate iniziali
					pipeData.y=aus.y;
					pipeData.type=aus.type;
					pipeData.id=aus.id;
					change=true;
    		} 
    		
    		pipeData.type='X'; // resetta il normale carattere della rana
       	
        ch = getch(); // Leggi il carattere dall'input
        
        // Muovi il personaggio in base all'input dell'utente
        switch (ch) {
        	case KEY_UP:
          	if(pipeData.y>minAreaGiocoY+3){
            	pipeData.y-=3;
            	change=true;
            }
            break;
          case KEY_DOWN:
          	if(pipeData.y<maxAreaGiocoY-3){
            	pipeData.y+=3;
            	change=true;
            }
            break;
          case KEY_LEFT:
          	if(pipeData.x>0){
            	pipeData.x--;
            	change=true;
            	
            }  
            break;
          case KEY_RIGHT:
          	if(pipeData.x<maxAreaGiocoX){
            	pipeData.x++;
            	change=true;
            }
            break;
          case 32: // KEY_SPACE 
          	pipeData.type='S'; //cambia carattere per dire a processoDisegna che  rana sta sparando
          	change=true;
            break;
          case 'p': // tasto p  PAUSA
          case 'P':
          	pipeData.type = 'Z';
          	change=true; 
            break;
          default:
          	break;
        }
				if(change){
					// Invia le coordinate attraverso la pipe
        	write(pipe_fd[1], &pipeData, sizeof(PipeData));
				}
        
        
			
        // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
        usleep(1000);
         
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





