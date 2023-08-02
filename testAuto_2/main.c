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
    
    		int dir_tronco[3];
    		int tmp_r = rand()%2; // imposta direzione random del primo tronco
    		if(tmp_r == 1){
    			dir_tronco[0]= 1;
    		}else{
    			dir_tronco[0]= -1;
    		}
    		dir_tronco[1]= -1* dir_tronco[0]; // imposta la direzione del tronco opposta al tronco precedente
    		dir_tronco[2]= -1* dir_tronco[1];
    		
    		
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
				// gestore macchine
				
				pid_t macchina_pid1 = fork();		// crea processo per macchina1
				if (macchina_pid1 < 0) {
		    	perror("Fork failed");
		    	return 1;
				} else if (macchina_pid1 == 0) {
					// figlio-macchina 1
					int row_y = 24;
					int direzione_x = 1;
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
					int row_y = 27;
					int direzione_x = 1;
					int id = 5;
					close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
					macchina(pipe_fd, row_y, direzione_x, id);
					exit(0);
				
				}// continua gestore macchine...
				
				wait(NULL); //aspetta fine di macchina1
				wait(NULL); //aspetta fine di macchina2
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
    return 0;
}
