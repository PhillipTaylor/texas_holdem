
PROGRAM_NAME = poker

SRC = linkedlist.c stack.c config.c logging.c card.c player.c main.c

$(PROGRAM_NAME): $(OBJ)
	gcc $(COMPILER) $(SRC) -o $(PROGRAM_NAME)

clean:
	rm -f $(PROGRAM_NAME)
