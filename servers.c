
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "prt_poker_clients"

void wait_for_players(void)
{

	printf("Main has forked off to wait for players.\n");

	//Every loop we test a semaphore that safely checks if we
	//should continue listening or if we are done.
	
	
	

}
