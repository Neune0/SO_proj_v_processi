#include "utilities.h"

void inizializzaNcurses() {
    srand(time(NULL));
    initscr(); // Inizializza ncurses
    curs_set(FALSE); // Nasconde il cursore
    nodelay(stdscr, TRUE); // Abilita l'input non bloccante
    keypad(stdscr, TRUE); // Abilita il keypad mode
    inizializzaColorazione();
    
}

void inizializzaColorazione(){
		start_color();
    init_pair(RANA, COLOR_GREEN, COLOR_BLACK); // Crea una combinazione di colori (colore verde su sfondo nero)
    init_pair(TRONCHI,COLOR_YELLOW,COLOR_RED); // per la colorazione dei tronchi
    init_pair(SFONDO,COLOR_WHITE,COLOR_BLACK); // colore sfondo
    init_pair(FIUME,COLOR_WHITE,COLOR_BLUE); // colore fiume 
    init_pair(TANE,COLOR_WHITE,COLOR_YELLOW); // colore tane
    init_pair(PRATO,COLOR_WHITE,COLOR_GREEN); // colore prato
    init_pair(STRADA,COLOR_WHITE,COLOR_BLACK); // colore strada
    init_pair(STRISCIA,COLOR_WHITE,COLOR_WHITE); // colore striscia 
    init_pair(MARCIAPIEDE,COLOR_WHITE,COLOR_WHITE); // colore marciapiede
    init_pair(AUTO,COLOR_WHITE,COLOR_YELLOW); // colore auto
    init_pair(MENUBOX,COLOR_GREEN,COLOR_BLACK);
    init_pair(PROIETTILE,COLOR_WHITE,COLOR_BLACK); // colore proiettile
}

void creaPipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
}


// uccide un dato processo  dell'array di pid_t
void uccidiProcesso( pid_t *array_pid, int id_processo){
	if((id_processo != -1) && (array_pid[id_processo] != 0))
	{ 
		kill(array_pid[id_processo], SIGKILL);
		waitpid(array_pid[id_processo],NULL,0);
		array_pid[id_processo]=0;
	}
}

//-----------------------------------------------------------
int id_disponibile(pid_t *array_pid, int lunghezza){
	for(int i=0;i<lunghezza;i++){
		if(array_pid[i]==0)return i;
	}
	return -1;
}
void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData){
    old_pos->x=pipeData->x;
    old_pos->y=pipeData->y;
    old_pos->type=pipeData->type;
    return;
}
void aggiornaDirezioneTronchi(PipeData *pipeData, PipeData *old_pos, int *arr_dir_tronchi ){
		int tronco_id = pipeData->id;													//individua che tronco hai letto
		arr_dir_tronchi[tronco_id] = pipeData->x - old_pos->x	;//controlla la direzione in base alla posizione precedente
}
