
#ifndef SERVERS_H
#define SERVERS_H

int wait_for_players(void);
void sigchld_handler(int s);
void login_handshake(int client_fd);

#endif
