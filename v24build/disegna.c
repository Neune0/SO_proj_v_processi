#include "disegna.h"
// processo che si occupa di disegnare lo schermo
void drawProcess(int* pipe_fd) {
	
	GameData *gameData = (GameData *)malloc(sizeof(GameData));
	// forse non serve, pipe in cui la rana legge e disegna scrive in teoria
	creaPipe(gameData->pipeRana_fd);
	
	// avvia i processi rana, tronchi e veicoli
	avviaProcessiBase(pipe_fd,&(gameData->pids),gameData->pipeRana_fd);
	
	inizializza(gameData); // inizializza i dati del gioco, qui si può leggere file di salvataggio ecc...
	
  while (1) {
  	read(pipe_fd[0], &(gameData->pipeData), sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe
  	
    aggiorna(gameData,pipe_fd); // aggiorna stato del gioco
    
		stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
		
    refresh(); // Aggiorna la finestra
    
    // debug zone
    /**/
    mvprintw(37,0,"                                        ");
    mvprintw(37,0,"disegna.c oldpos x Rana: %d",gameData->oldPos.general[0].x);
    
    mvprintw(38,0,"                                                               ");
    mvprintw(38,0,"disegna.c pipeData| tipo: %c, x: %d, y: %d, id: %d",gameData->pipeData.type, gameData->pipeData.x, gameData->pipeData.y, gameData->pipeData.id);
    /**/
	}
	
	free(gameData);
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
        drawProcess(pipe_fd);
        exit(0); // TODO: è davvero utile?
    }
}
