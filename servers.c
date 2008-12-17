
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
		perror("socket");
		exit(1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	
	my_addr.sin_family = AF_INET;		 // host byte order
	my_addr.sin_port = htons(*port);	 // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, *listen_backlog) == -1) {
		perror("listen");
		exit(1);
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

		logging_debug_high("Connection recieved\n");
		printf("server: got connection from %s\n",inet_ntoa(their_addr.sin_addr));
		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			//if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
			//	perror("send");
			//close(new_fd);
			login_handshake(new_fd);
			_exit(0);
		}
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}

void login_handshake(int client_fd)
{

	player *p;
	char *buff;
	int *retries;
	int i;

	config_get_int("password_retries", &retries);

	p = player_new();
	p->name = NULL;
	p->password = "password";
	p->connection = client_fd;

	do
	{

		player_send(p, "Username: ");
		player_recv(p, &buff);

		logging_debug_high(buff);

		player_send(p, "Password: ");
		player_recv(p, &buff);

		player_send(p, "List of Tables:");

		for (i = 0; i < *table_count; i++)
			player_send(p, *(table_array[i]));

		player_send(p, "Enter your choice: ");
		player_recv(p, &buff);		

		if (strncmp(buff, p->password, strlen(p->password)) == 0)
			break;
		else
			player_send(p, "Username or Password Incorrect. Please try again\nPassword: ");

	} while (--*retries);

	if (strncmp(buff, p->password, strlen(p->password)) == 0)
	{
		player_send(p, "Password Accepted\n");
		p->name = buff;


		return;
	}

	free(buff);
	close(client_fd);

}

