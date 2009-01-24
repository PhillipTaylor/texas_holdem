
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "util.h"

void send_str(int socket, char *message)
{
	send(socket, message, (strlen(message) + 1), 0);
}

char *recv_str(int socket)
{
	char buff[255];
	int buff_sz;

	char *ptr;

	buff_sz = recv(socket, &buff, 254, 0);
	buff[(buff_sz - 1)] = '\0';

	ptr = (char*) malloc (sizeof(char) * (buff_sz + 1));
	memcpy(ptr, buff, (buff_sz + 1));

	return ptr;
}

