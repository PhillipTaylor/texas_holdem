
#ifndef TEXAS_HOLDEM_UTIL_H
#define TEXAS_HOLDEM_UTIL_H

void send_str(int socket, char *message, ...);
char *recv_str(int socket);

#endif
