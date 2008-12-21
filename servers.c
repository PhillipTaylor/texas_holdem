
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
	char tmp[255];
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
		p->name = buff;

		player_send(p, "Password: ");
		player_recv(p, &buff);

		if (strncmp(buff, p->password, strlen(p->password)) == 0)
			break;
		else
		{
			//nb. use inet_ntoa(their_addr.sin_addr) to print an IP
			logging_info("Failed login attempt");
			player_send(p, "Username or Password Incorrect. Please try again\n");
		}

	} while (--*retries);

	if (strncmp(buff, p->password, strlen(p->password)) == 0)
	{
		player_send(p, "Password Accepted\n");
		p->name = buff;

		sprintf(tmp, "Table count: %d\n", *table_count);
		player_send(p, tmp);

		player_send(p, "List of Tables:\n");
		for (i = 0; i < *table_count; i++)
		{
			sprintf(tmp, "Table name %d: %s\n", (i + 1), table_names[i]);
			player_send(p, tmp);
		}

		if (*table_count > 1)
		{
			sprintf(tmp, "Enter your choice (1-%d): ", *table_count);
			player_send(p, tmp);
			player_recv(p, &buff);

			i = atoi(buff);

			if (i < 1 || i > *table_count)
			{
				printf("The number you gave was outside the valid choices.");
				player_free(p);
				return;
			}

			//zero based index not exposed to user.
			i--;
		}
		else
			i = 0;


		sprintf(tmp, "You will be playing at %s\n", table_names[i]);
		player_send(p, tmp);

		logging_info("User %s logged in successfull and moved to table %s", p->name, table_names[i]);

		//now it is time to hand
		//the player over to the table
		//in question.

		return;
	}

	free(buff);
	close(client_fd);

}

