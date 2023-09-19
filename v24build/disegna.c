#include "disegna.h"
// processo che si occupa di disegnare lo schermo
void drawProcess(int* pipe_fd) {
	GameData gameData;
	
	creaPipe(gameData.pipeRana_fd);
	
	avviaProcessiBase(pipe_fd,&(gameData.pids),gameData.pipeRana_fd);
	
	inizializza(&gameData);
	
  while (1) {
  	read(pipe_fd[0], &(gameData.pipeData), sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe
  	
    aggiorna(&gameData,pipefd); // aggiorna stato del gioco
    
		stampaMatrice(screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
		
    refresh(); // Aggiorna la finestra
	}
  return;  
}
// per far partire il processo che si occupa di disegnare
void avviaDrawProcess(int pipe_fd[2]) {
    pid_t draw_pid = fork();
    if (draw_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (draw_pid == 0) {
        // Processo "disegna"
        //close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe, TODO: è davvero utile?
        drawProcess(pipe_fd);
        exit(0); // TODO: è davvero utile?
    }
}
