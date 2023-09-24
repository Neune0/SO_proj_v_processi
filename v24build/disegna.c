#include "disegna.h"
// processo che si occupa di disegnare lo schermo
void drawProcess(int* pipe_fd) {
	
	GameData *gameData = (GameData *)malloc(sizeof(GameData));
	// forse non serve, pipe in cui la rana legge e disegna scrive in teoria
	creaPipeNonBloccante(gameData->pipeRana_fd);
	
	// avvia i processi rana, tronchi e veicoli
	avviaProcessiBase(pipe_fd,&(gameData->pids),gameData->pipeRana_fd);
	
	inizializza(gameData); // inizializza i dati del gioco, qui si può leggere file di salvataggio ecc...
	int id_nemici[3]={-1,-1,-1};
	int id_rana_tronco=-1;
	int pos_x_rel=-1;
  while (gameData->contatori.manche!=3 && gameData->contatori.vite>0) {
  	read(pipe_fd[0], &(gameData->pipeData), sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe
  	
    aggiorna(gameData,pipe_fd,id_nemici,&id_rana_tronco,&pos_x_rel); // aggiorna stato del gioco
    
		stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
		
    refresh(); // Aggiorna la finestra
    
	}
	if(gameData->contatori.vite==0){
		// stampa game over
		stampaGameOver();
	}
	else{
		stampaWin();
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

void stampaWin(){
	clear();
	stampaBoxMenu();
	stampaLogoMenu(STARTROWLOGOMENU,STARTCOLLOGOMENU);
	mvprintw(15,80,"you win!!!!!!!!!!!!!");
	refresh();
	usleep(30000000);
	return;


}

void stampaGameOver(){
	clear();
	stampaBoxMenu();
	stampaLogoMenu(STARTROWLOGOMENU,STARTCOLLOGOMENU);
	mvprintw(15,80,"Game Over!");
	refresh();
	usleep(30000000);
	return;


}

