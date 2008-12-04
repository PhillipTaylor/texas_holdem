
SRC = linkedlist.c stack.c config.c logging.c card.c server.c main.c
OBJ = linkedlist.o stack.o config.o logging.o card.o server.o main.o
PROG = poker

$(PROG): $(OBJ)
	gcc $(OBJ) -o $(PROG)

$(OBJ): $(SRC)

