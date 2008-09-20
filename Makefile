
SRC = card.h card.c configuration.h configuration.c hand.c hand.c handdetection.h handdetection.c linkedlist.h linkedlist.c main.c player.h player.c simplecardalgorithms.c simplecardalgorithms.h stack.h stack.c table.h table.c

OBJ = card.o configuration.o hand.o handdetection.o linkedlist.o main.o player.o simplecardalgorithms.o stack.o table.o

PROG = poker

$(PROG): $(OBJ)
	gcc $(OBJ) -o $(PROG)
	
$(OBJ): $(SRC)
