#include "disegna.h"
int main() {
		int pipe_fd[2]; // Pipe file descriptors
		inizializzaNcurses(); // inizializzazione della libreria ncurses + inizializzazione seme per random
		
    creaPipe(pipe_fd);  // Crea la pipe
    
    //avviaLogo();   // visualizza il logo ed aspetta che l'utente prema un tasto qualsiasi
    
    //avviaMenuIniziale(); // fa partire il processo per il menu iniziale, aspetta che termini e poi prosegue
		
    avviaDrawProcess(pipe_fd); // avvia il processo che gestisce la stampa a schermo
		
    avviaRana(pipe_fd); // avvia il processo che gestisce il movimento della rana
    
    avviaGestoreTronchi(pipe_fd); // avvia il processo che gestisce i tronchi
    
    avviaGestoreMacchine(pipe_fd); // avvia il processo che gestisce le macchine
    
    // Chiudi le estremità della pipe nel processo padre
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // Aspetta che i processi figlio terminino
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
 
    endwin(); // Termina ncurses
    return 0;
}
