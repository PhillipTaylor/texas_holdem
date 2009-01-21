
PROGRAM_NAME = poker

SRC = src/linkedlist.c src/stack.c src/config.c src/logging.c src/card.c src/player.c src/table.c src/main.c

$(PROGRAM_NAME): $(OBJ)
	gcc $(COMPILER) $(SRC) -o $(PROGRAM_NAME)

clean:
	rm -f $(PROGRAM_NAME)
	rm -f *.o
