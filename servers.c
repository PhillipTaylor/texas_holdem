
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#include "globals.h"
#include "servers.h"
#include "config.h"
#include "player.h"

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

int wait_for_players(void)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr, their_addr;	// my address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	int *port;
	int *listen_backlog;

	port = malloc(sizeof(int));
	listen_backlog = malloc(sizeof(int));

	config_get_int("port_number", &port);
	config_get_int("listen_backlog", &listen_backlog);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		logging_critical("call to socket() failed with -1 in wait_for_players()");
		_exit(1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		logging_critical("call to setsockopt() failed with -1 in wait_for_players()");
		_exit(1);
	}
	
	my_addr.sin_family = AF_INET;		 // host byte order
	my_addr.sin_port = htons(*port);	 // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
		logging_critical("call to bind() failed with -1 in wait_for_players()\nTry killing ALL instances of this program and start it again.");
		_exit(1);
	}

	if (listen(sockfd, *listen_backlog) == -1) {
		logging_critical("call to listen() failed with -1 in wait_for_players");
		_exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
			perror("accept");
			continue;
		}

		logging_debug("Connection recieved from %s", inet_ntoa(their_addr.sin_addr));
		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			login_handshake(new_fd);
			_exit(0);
		}
	}

	return 0;
}

void login_handshake(int client_fd)
{
	player *p;
	int i;
	char *table;
	int msg_num;

	//we can use player_send() and player_recv()
	//with a valid connection set.
	//so lets take advantage of this.
	p = player_new();
	p->connection = client_fd;

	player_send(p, "Username: ");
	player_recv(p, &p->name);

	player_send(p, "Password: ");
	player_recv(p, &p->password);

	//send list of tables
	player_send(p, "Select a table:\n");
	for (i = 0; i < *player_count; i++)
		player_send(p, "%i: %s\n", (i + 1), table_names[i]);

	player_send(p, "Enter a number: ");
	player_recv(p, &table);

	//msg num is used to send the message
	//to the correct thread.	
	msg_num = atoi(table);

	p->mtype = (long) (MSG_QUEUE_OFFSET - 1) + ((long)msg_num);

	i =  msgsnd(msg_queue, p, sizeof(player), 0);

	if (i == -1)
		logging_critical("msgsnd failed");
	else
		logging_info("msgsnd returned %i", i);

	logging_info("message sent\n");
}

