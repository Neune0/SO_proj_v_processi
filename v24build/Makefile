# Nome dell'eseguibile
EXECUTABLE := frog

# Elenco dei file oggetto
OBJECTS := main.o utilities.o disegna.o auto.o tronco.o rana.o logo.o menuIniziale.o inizializza.o proiettile.o nemico.o collisioni.o pausa.o proiettileNemico.o debug.o hud.o schermo.o

# Compilatore C
CC := gcc

# Opzioni di compilazione
CFLAGS := -Wall -Wextra

# Librerie necessarie
LDLIBS := -lncurses


# Regola di compilazione dell'eseguibile
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDLIBS)

# Regole di compilazione dei file oggetto
main.o: main.c disegna.h
	$(CC) $(CFLAGS) -c main.c

disegna.o: disegna.c auto.h tronco.h rana.h logo.h menuIniziale.h inizializza.h proiettile.h nemico.h proiettileNemico.h debug.h hud.h schermo.h
	$(CC) $(CFLAGS) -c disegna.c
	
auto.o: auto.c utilities.h
	$(CC) $(CFLAGS) -c auto.c

tronco.o: tronco.c utilities.h
	$(CC) $(CFLAGS) -c tronco.c
	
rana.o: rana.c pausa.h
	$(CC) $(CFLAGS) -c rana.c

proiettile.o: proiettile.c utilities.h
	$(CC) $(CFLAGS) -c proiettile.c


logo.o: logo.c utilities.h
	$(CC) $(CFLAGS) -c logo.c
	
menuIniziale.o: menuIniziale.c utilities.h
	$(CC) $(CFLAGS) -c menuIniziale.c
	
inizializza.o: inizializza.c utilities.h
	$(CC) $(CFLAGS) -c inizializza.c

nemico.o: nemico.c utilities.h
	$(CC) $(CFLAGS) -c nemico.c
	
proiettileNemico.o: proiettileNemico.c utilities.h
	$(CC) $(CFLAGS) -c proiettileNemico.c
	
collisioni.o: collisioni.c utilities.h
	$(CC) $(CFLAGS) -c collisioni.c 

pausa.o: pausa.c utilities.h
	$(CC) $(CFLAGS) -c pausa.c
	
debug.o: debug.c utilities.h
	$(CC) $(CFLAGS) -c debug.c

hud.o: hud.c utilities.h
	$(CC) $(CFLAGS) -c hud.c
	
schermo.o: schermo.c utilities.h
	$(CC) $(CFLAGS) -c schermo.c

utilities.o: utilities.c utilities.h
	$(CC) $(CFLAGS) -c utilities.c

# Pulizia dei file oggetto e dell'eseguibile
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

openAll:
	gedit *.c *.h TODO.txt MakeFile&




