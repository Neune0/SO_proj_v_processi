#include "disegna.h"

/**
		Versione del main con registrazione modifiche frame per frame
		tramite variabile di flag nelle celle delle matrici
		
*/
int main() {
		srand(time(NULL));
		
		initscr(); // Inizializza ncurses
    curs_set(FALSE); // Nasconde il cursore
    nodelay(stdscr, TRUE); // Abilita l'input non bloccante
    keypad(stdscr, TRUE); // Abilita il keypad mode
		
    // Crea la pipe
    int pipe_fd[2]; // Pipe file descriptors
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    // Crea il processo per il disegno
    pid_t draw_pid = fork();
    if (draw_pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (draw_pid == 0) {
        // Processo "disegna"
        close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe
        drawProcess(pipe_fd);
        exit(0);
    }

    // Crea il processo per il movimento
    pid_t move_pid = fork();
    if (move_pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (move_pid == 0) {
        // Processo "muovi"
        close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
        moveProcess(pipe_fd);
        exit(0);
    }
    
    pid_t gestore_figli_pid = fork();
    if(gestore_figli_pid < 0){
    	perror("Fork failed");
      return 1;
    }else if(gestore_figli_pid == 0){  // gestore tronchi
    		int num_tronchi = 3;
    		int dir_tronco[3];
    		int tmp_r = rand()%2; // imposta direzione random del primo tronco
    		if(tmp_r == 1){
    			dir_tronco[0]= 1;
    		}else{
    			dir_tronco[0]= -1;
    		}
    		
    		// imposta la direzione del tronco opposta alla direzione del tronco precedente
    		int i = 1;
    		while(i<num_tronchi){
    			dir_tronco[i]= -1* dir_tronco[i-1];
    			i++;
    		}
    		//dir_tronco[1]= -1* dir_tronco[0]; 
    		//dir_tronco[2]= -1* dir_tronco[1];
    		
    		
    		
    	// Crea il processo per il tronco 1
				pid_t tronco_pid = fork();
				if (tronco_pid < 0) {
				    perror("Fork failed");
				    return 1;
				} else if (tronco_pid == 0) {
				    // Processo tronco 1
				    //int col_tronco = (int) rand()%(WIDTH-10)
				    close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
				    tronco(pipe_fd,10, dir_tronco[0] ,1);
				    exit(0);
				}
				
				// Crea il processo per il tronco 2
				pid_t tronco_pid2 = fork();
				if (tronco_pid2 < 0) {
				    perror("Fork failed");
				    return 1;
				} else if (tronco_pid2 == 0) {
				    // Processo tronco 2
				    close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
				    tronco(pipe_fd,13, dir_tronco[1],2);
				    exit(0);
				}
				
				// Crea il processo per il tronco 3
				pid_t tronco_pid3 = fork();
				if (tronco_pid3 < 0) {
				    perror("Fork failed");
				    return 1;
				} else if (tronco_pid3 == 0) {
				    // Processo tronco 3
				    close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
				    tronco(pipe_fd,16, dir_tronco[2],3);
				    exit(0);
				}
				//gestore tronchi
				//aspetta termine dei figli-tronchi
				wait(NULL);
				wait(NULL);
				wait(NULL);
				exit(0);
    
    } //fine gestore tronchi, continua il padre...
    
    pid_t gestore_auto_pid = fork();
    /*----------------Gestore figli macchine/caminon ---------*/
    if (gestore_auto_pid < 0) {
		    perror("Fork failed");
		    return 1;
		} else if (gestore_auto_pid == 0) { 
				// -----------------------------gestore macchine----------------
				// determina direzione delle corsie 
				int index_auto = 1;
				int num_auto = 4;
				int dir_auto[ num_auto ];
				int tmp_r=rand()%2;
				if(tmp_r == 1){
					dir_auto[0]=1;
				}else{
					dir_auto[0]=-1;
				}
				while(index_auto < num_auto){
					dir_auto[index_auto]= -1* dir_auto[index_auto-1];
					index_auto++;
				}
				
				pid_t macchina_pid1 = fork();		// crea processo per macchina1
				if (macchina_pid1 < 0) {
		    	perror("Fork failed");
		    	return 1;
				} else if (macchina_pid1 == 0) {
					// figlio-macchina 1
					
					int row_y = 22;
					int direzione_x = dir_auto[0];
					int id = 4;
					close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
					macchina(pipe_fd, row_y, direzione_x, id);
					
					exit(0);
				}// continua gestore macchine...
				
				pid_t macchina_pid2 = fork();		// crea processo per macchina2
				if (macchina_pid2 < 0) {
		    	perror("Fork failed");
		    	return 1;
				} else if (macchina_pid2 == 0) {
					// figlio-macchina 2
					int row_y = 24;
					int direzione_x = dir_auto[1];
					int id = 5;
					close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
					macchina(pipe_fd, row_y, direzione_x, id);
					exit(0);
				}// continua gestore macchine...
				
				pid_t macchina_pid3 = fork();		// crea processo per macchina3
				if (macchina_pid3 < 0) {
		    	perror("Fork failed");
		    	return 1;
				} else if (macchina_pid3 == 0) {
					// figlio-macchina 3
					int row_y = 27;
					int direzione_x = dir_auto[2];
					int id = 6;
					close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
					macchina(pipe_fd, row_y, direzione_x, id);
					exit(0);
				
				}// continua gestore macchine...
				
				pid_t macchina_pid4 = fork();		// crea processo per macchina4
				if (macchina_pid4 < 0) {
		    	perror("Fork failed");
		    	return 1;
				} else if (macchina_pid4 == 0) {
					// figlio-macchina 4
					int row_y = 29;
					int direzione_x = dir_auto[3];
					int id = 7;
					
					close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
					macchina(pipe_fd, row_y, direzione_x, id);
					
					exit(0);
				
				}// continua gestore macchine...
				
				
				wait(NULL); //aspetta fine di macchina1
				wait(NULL); //aspetta fine di macchina2
				wait(NULL); //aspetta fine di macchina3
				wait(NULL); //aspetta fine di macchina4
				exit(0);
				
				
		}//fine gestore macchine, continua il padre...
				
    // Chiudi le estremità della pipe nel processo padre
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // Aspetta che i processi figlio terminino
    //moveProcess, drawProcess, gestore-tronchi, gestore-macchine
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    endwin(); // Termina ncurses
    return 0;   
}
