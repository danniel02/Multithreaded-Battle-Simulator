EXE = main
OBJ = deque.o fighter.o specs.o main.o
CC = gcc -Wall

all:	$(EXE)

main:	$(OBJ)
	$(CC) -o main $(OBJ) -lpthread -lrt

main.o:		main.c defs.h
	${CC} -c main.c 

deque.o:	deque.c defs.h
	$(CC) -c deque.c

fighter.o:	fighter.c defs.h
	$(CC) -c fighter.c

specs.o:	specs.c defs.h
	$(CC) -c specs.c

clean:
	rm -f $(EXE) $(OBJ)
