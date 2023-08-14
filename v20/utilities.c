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
}

void creaPipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
}
