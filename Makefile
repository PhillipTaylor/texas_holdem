
SRC = linkedlist.c stack.c config.c logging.c card.c servers.c main.c
OBJ = linkedlist.o stack.o config.o logging.o card.o servers.o main.o
PROG = poker

$(PROG): $(OBJ)
	gcc $(OBJ) -o $(PROG)

$(OBJ): $(SRC)

