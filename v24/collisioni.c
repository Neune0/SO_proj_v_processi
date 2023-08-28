#include "collisioni.h"



//----------------------------------------COLLISIONI----------------------------
/**	Prende due oggetti di gioco con rispettive sprite e verifica se gli oggetti si incontrano 
		Ritorna TRUE se oggetto_1 entra nel perimetro di oggetto_2
*/ 
bool checkCollisione(PipeData *object_1, PipeData *object_2, Sprite* sprite_1, Sprite* sprite_2)
{
	// stabilisce coordinate massime per entrambi gli oggetti 
	int obj1_maxX, obj1_maxY, obj2_maxX, obj2_maxY; 
	obj1_maxX = (object_1->x) + (sprite_1->max_col-1);
	obj1_maxY = (object_1->y) + (sprite_1->max_row-1);
	obj2_maxX = (object_2->x) + (sprite_2->max_col-1);
	obj2_maxY = (object_2->y) + (sprite_2->max_row-1);									//assumendo che tutti gli oggetti siano alti 2row
	
	bool checkUP, checkDOWN, checkLEFT,checkRIGHT;
	checkUP = checkDOWN = checkLEFT = checkRIGHT = false;
	// controlla se i lati di object_1 sono dentro object_2, controllo lato per lato
	checkUP = isBetween(object_1->y, object_2->y, obj2_maxY); 
	checkDOWN =  isBetween(obj1_maxY, object_2->y, obj2_maxY);
	checkLEFT = isBetween(object_1->x, object_2->x, obj2_maxX);
	checkRIGHT = isBetween(obj1_maxX, object_2->x, obj2_maxX);
	
	if((checkUP || checkDOWN)&&(checkLEFT || checkRIGHT))
	{
		return true;
	}
	
	return false;
}
//-----------------------------------------------------------------------------
// Controlla se la Rana collide con uno degli oggetti in gioco Tronco o veicolo (Rana == old_pos[0])
bool collisioneRana( PipeData *old_pos, Sprite *array_sprite)
{
	PipeData *rana = &old_pos[0];
	
  bool collision=false;
   
	for(int i=1; i<OLDPOSDIM; i++){ // per ogni oggetto di gioco
 		if(i<4) // check collisione con Tronchi
 		{ 
 			collision = checkCollisione(rana, &old_pos[i], &array_sprite[RANA_SPRITE], &array_sprite[TRONCO_SPRITE]);
	 	}
	 	if(i>=4){							// check collisione con auto/camion
	 		if(old_pos[i].type=='A')
	 		{
 				collision = checkCollisione(rana, &old_pos[i], &array_sprite[RANA_SPRITE], &array_sprite[AUTO_SPRITE]);
			}else if (old_pos[i].type=='C')
			{
				collision = checkCollisione(rana, &old_pos[i], &array_sprite[RANA_SPRITE], &array_sprite[CAMION_SPRITE]);
	 		}
	 	}
	 	if(collision) break; //se rileva collisione ferma il ciclo e ed esce
	}
 	return collision;
}
//----------------------------------------collisioni rana-auto/camion-----------------------
bool collisioneAuto( PipeData *old_pos, Sprite *array_sprite)
{
	PipeData *rana = &old_pos[0];
	
  bool collision=false;
   
	for(int i=4; i<OLDPOSDIM; i++){ // per ogni auto/camion in gioco (old_pos[4-11])
 		if(old_pos[i].type=='A')
 		{
			collision = checkCollisione(&old_pos[i], rana, &array_sprite[AUTO_SPRITE], &array_sprite[RANA_SPRITE]);
		}else if (old_pos[i].type=='C')
		{
			collision = checkCollisione(&old_pos[i], rana, &array_sprite[CAMION_SPRITE], &array_sprite[RANA_SPRITE]);
 		}
	 	if(collision) break; //se rileva collisione ferma il ciclo e ed esce
	}
 	return collision;
}
//------------------------------------collisioni proiettili nemici-------------------------------
// rileva collisione Rana-ProiettileNemico
int collisioneProiettiliNemici( PipeData *old_pos, PipeData *old_pos_proiettiliNemici ,Sprite *array_sprite)
{
	PipeData *rana = &old_pos[0];
	int bullet_id = -1; //valore non valido, usa bullet_id per sapere l'inidice del proiettile 
  bool collision=false;
   
	for(int i=0; i<MAXNPROIETTILINEMICI; i++){ 							// per ogni proiettile nemico di gioco
 		if(old_pos_proiettiliNemici[i].type != ' '){					// se il proiettile è attivo
 			collision = checkCollisione(&old_pos_proiettiliNemici[i], rana, 
																	&array_sprite[PROIETTILE_NEMICO_SPRITE], &array_sprite[RANA_SPRITE]);
 		}
	 	if(collision) {
	 		bullet_id = i;
	 		break;
	 	} //se rileva collisione ferma il ciclo e ed esce
	}
 	return bullet_id;
}
//----------------------------collisione TANE-------------------
bool collisioneTaneAperte( PipeData *old_pos, Tana *array_tane, Sprite *array_sprite, Sprite *arr_tana_sprite)
{
	bool collision=false;
	PipeData *rana = &old_pos[0];
	
	for(int i=0; i<5; i++){
		PipeData *tana = &array_tane[i].info; //punta alla struttura PipeData dentro la struct Tana
		if(array_tane[i].status == OPEN){
			collision = checkCollisione(rana, tana, &array_sprite[RANA_SPRITE], &arr_tana_sprite[OPEN]);
		}
		if(collision) {
			array_tane[i].status = CLOSED;
			break;
		}
	}
 	return collision;
}
//----------------------------------------
bool collisioneTaneChiuse( PipeData *old_pos, Tana *array_tane, Sprite *array_sprite, Sprite *arr_tana_sprite)
{
	bool collision=false;
	PipeData *rana = &old_pos[0];
	
	for(int i=0; i<5; i++){
		PipeData *tana = &array_tane[i].info; //punta alla struttura PipeData dentro la struct Tana
		if(array_tane[i].status == CLOSED){
			collision = checkCollisione(rana, tana, &array_sprite[RANA_SPRITE], &arr_tana_sprite[CLOSED]);
		}
		if(collision) {
			//array_tane[i].status = CLOSED;
			break;
		}
	}
 	return collision;
}
//-----------------------------------------collisione auto-proiettili------------
bool collisioneAutoProiettili( PipeData *old_pos, PipeData * array_proiettili, Sprite *array_sprite, TipoSprite sprite_proiettile)
{
	PipeData *rana = &old_pos[0];
	PipeData *veicolo;
	PipeData *proiettile;
	TipoSprite spriteVeicolo;
	char charBullet;	//char inviato dal proiettile sulla pipe
	switch(sprite_proiettile){					// in base al TIpoSprite, seleziona carattere usato dal proiettile,
		case PROIETTILE_NEMICO_SPRITE:
			charBullet = 'p';
			break;
		case PROIETTILE_SPRITE:
			charBullet = 'P';
			break;
		default:
			break;
	}
	
  bool collision=false;
  
	for(int i=0; i<MAXNPROIETTILI; i++){ 				// per ogni proiettile nemico di gioco
		proiettile = &array_proiettili[i];  			// prendi proiettile attuale
		
		if(proiettile->type == charBullet){							// se il proiettile è attivo...
			for(int j=4; j<OLDPOSDIM; j++){							// per ogni veicolo  in gioco
				veicolo =  &old_pos[j];										// prendi veicolo corrente
				
				switch(veicolo->type){							//controlla carattere del veicolo, scegli la sprite corretta
					case 'A':
						spriteVeicolo = AUTO_SPRITE;
						;
						break;
					case 'C':
						spriteVeicolo = CAMION_SPRITE;
						;
						break;
					default:
						spriteVeicolo = CAMION_SPRITE;
						;
						break;
				}
	 			collision = checkCollisione(proiettile, veicolo, 
																		&array_sprite[sprite_proiettile], &array_sprite[ spriteVeicolo ]);
 				if(collision) { 
 					//beep(); 
 					//break;
 					return collision;
				} //se rileva collisione ferma il ciclo e ed esce
 			}
		 //break;
		}//end if proiettile type
 		
	}
 	return collision;
}







//------------------------------------------------
//controlla se value è compreso tra valori min e max
bool isBetween (int value, int min_value, int max_value){
	
	if((value >= min_value) && (value <= max_value)){
		return true;
	}
	return false;
}

