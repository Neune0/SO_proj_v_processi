#include "debug.h"


void printDebug(pid_t pidRana,
								pid_t* array_pid_proiettili,
								pid_t* array_pid_proiettili_nemici,
								pid_t* array_pid_nemici,
								pid_t* pid_veicoli,
								pid_t* pid_tronchi,
								int contatore_proiettili_in_gioco,
								int contatore_nemici_in_gioco,
								int contatore_proiettili_nemici_in_gioco,
								PipeData pipeData,
								PipeData* old_pos){


    mvprintw(37,2,"                                                       ");
    mvprintw(37,2,"pid rana: %d",pidRana);
    mvprintw(36,2,"                                                                                                                           ");
    mvprintw(36,2,"pid proiettili: ");
    for(int i=0;i<MAXNPROIETTILI;i++){
    	mvprintw(36,18+(i*11),"pid%d: %d",i,array_pid_proiettili[i]);
    }
    mvprintw(35,2,"                                                                                                                           ");
    mvprintw(35,2,"pid proiettili nemici: ");
    for(int i=0;i<MAXNPROIETTILINEMICI;i++){
    	mvprintw(35,26+(i*11),"pid%d: %d",i,array_pid_proiettili_nemici[i]);
    }
    mvprintw(34,2,"                                                                                                                           ");
    mvprintw(34,2,"pid nemici: ");
    for(int i=0;i<MAXNNEMICI;i++){
    	mvprintw(34,15+(i*11),"pid%d: %d",i,array_pid_nemici[i]);
    }
    mvprintw(33,2,"                                                                                                                           ");
    mvprintw(33,2,"pid veicoli: ");
    for(int i=0;i<8;i++){
    	mvprintw(33,15+(i*11),"pid%d: %d",i,pid_veicoli[i]);
    }
    
    mvprintw(38,2,"                                                                                                                           ");
    mvprintw(38,2,"pid tronchi: ");
    for(int i=0;i<3;i++){
    	mvprintw(38,15+(i*11),"pid%d: %d",i,pid_tronchi[i]);
    }
    
    mvprintw(37,110,"                        ");
    mvprintw(37,110,"proiettili in gioco: %d",contatore_proiettili_in_gioco);
    mvprintw(38,110,"                               ");
    mvprintw(38,110,"nemici in gioco: %d",contatore_nemici_in_gioco);
    mvprintw(39,110,"                                  ");
    mvprintw(39,110,"proiettili nemici in gioco: %d",contatore_proiettili_nemici_in_gioco);
    
  switch(pipeData.type){
    	case 'X':
    		mvprintw(0,110,"                                    ");
    		mvprintw(0,110,"RANA tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type, old_pos[0].x,old_pos[0].y, pipeData.id);
        break;
			case 'T':
        mvprintw(pipeData.id,110,"                                    ");
    		mvprintw(pipeData.id,110,"TRONCO tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
      case 'A':
      	mvprintw(pipeData.id,110,"                                    ");
    		mvprintw(pipeData.id,110,"AUTO tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
     case 'C': 
      	mvprintw(pipeData.id,110,"                                    ");
    		mvprintw(pipeData.id,110,"CAMION tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
      case 'P':
      	mvprintw(13+pipeData.id,110,"                                       ");
    		mvprintw(13+pipeData.id,110,"PROIETTILE tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
      	if(pipeData.y<0){
      		mvprintw(13+pipeData.id,110,"                                    ");
    			mvprintw(13+pipeData.id,110,"id: %d uccisione proiettile con pid: %d",pipeData.id,array_pid_proiettili[pipeData.id]);   		
      	}
      	break;
      case 'p':
      	mvprintw(25+pipeData.id,110,"                                       ");
    		mvprintw(25+pipeData.id,110,"PROIETTILEN tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
      	if(pipeData.y>30){
      		mvprintw(25+pipeData.id,110,"                                    ");
    			mvprintw(25+pipeData.id,110,"id: %d uccisione proiettile con pid: %d",pipeData.id,array_pid_proiettili[pipeData.id]);
    			}
      	break;
      default:
        break;
    }
    return;
}
