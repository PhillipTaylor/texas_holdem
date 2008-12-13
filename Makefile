
SRC = linkedlist.c stack.c config.c logging.c card.c player.c servers.c main.c
OBJ = linkedlist.o stack.o config.o logging.o card.o player.o servers.o main.o
PROG = poker

$(PROG): $(OBJ)
	gcc $(OBJ) -o $(PROG)

$(OBJ): $(SRC)

