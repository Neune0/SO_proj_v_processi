#include "proiettile.h"

void proiettile(int* pipe_fd, PipeData* obj){
	
	PipeData proiettile; // Inizializza proiettile
	proiettile.x = obj->x;
	proiettile.y = obj->y;
	proiettile.type = 'S';
	proiettile.id = 8;
	int dir_y = -1;   //direzione del proiettile
	while(1){
		// scrivi posizione su pipe
		close(pipe_fd[0]);
		write(pipe_fd[1],&proiettile, sizeof(PipeData));
		
		// aggiorna posizione
		proiettile.y+=dir_y;
		usleep(100000);
		
		if(proiettile.y<=0){ exit(0); } // raggiunta la fine del campo, ha finito, or va in busy-waiting
	}
	
	
}//end proiettile


pid_t creaProiettile(int* pipe_fd,  PipeData* obj){
	pid_t pidProiettile;
	pidProiettile = fork();
	switch(pidProiettile){
		case -1:
			perror("proietile ERROR");
			return 1;
			break;
			
		case 0: 		// figlio proiettile
			proiettile(pipe_fd, obj);
			_exit(0);
			break;
			
		default:   
			break;
	}
	
	return pidProiettile;	// ritorna il pid del figlio proiettile
}
