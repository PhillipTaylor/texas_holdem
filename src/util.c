
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdarg.h>

#include "util.h"
#include "logging.h"

void send_str(int socket, char *message, ...)
{
	va_list ap;
	char arg_merged[200];

	va_start(ap, message);
	vsnprintf(arg_merged, 200, message, ap);
	va_end(ap);

	send(socket, arg_merged, (strlen(arg_merged) + 1), 0);
}

char *recv_str(int socket)
{
	char buff[255];
	int buff_sz;

	char *ptr;

	buff_sz = recv(socket, &buff, 254, 0);

	//TWO, BECAUSE "ROBERT" AS INPUT = R, O, B, E, R, T, (CHAR 13 - \r), (CHAR 10 \n), (CHAR NULL \0)

	buff[(buff_sz - 2)] = '\0';

	ptr = (char*) malloc (sizeof(char) * (buff_sz + 1));
	memcpy(ptr, buff, (buff_sz + 1));

	return ptr;
}

