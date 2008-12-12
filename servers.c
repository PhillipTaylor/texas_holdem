
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <signal.h>

#define MYPORT 3665
#define BACKLOG 10

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

void wait_for_players(void)
{
	int master_socket, child_socket, t, len;
	struct sockaddr_in local, remote;
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;

	char buff[100];

	printf("Main has forked off to wait for players.\n");

	if ((master_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		logging_critical("unable to open master socket.\n");
		exit(2);
	}

	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		logging_critical("setsockopt");
		exit(1);
	}
	
	local.sin_family = AF_INET;		// host byte order
	local.sin_port = htons(MYPORT);		// short, network byte order
	local.sin_addr.s_addr = INADDR_ANY;    	// automatically fill with my IP
	memset(local.sin_zero, '\0', sizeof local.sin_zero);

	if (bind(master_socket, (struct sockaddr *)&local, sizeof local) == -1) {
		logging_critical("bind");
		exit(1);
	}

	if (listen(master_socket, BACKLOG) == -1) {
		logging_critical("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		logging_critical("sigaction");
		exit(1);
	}

	while(1) {  // main accept() loop
		sin_size = sizeof remote;
		if ((child_socket = accept(master_socket, (struct sockaddr *)&remote, &sin_size)) == -1) {
			logging_critical("accept");
			continue;
		}
		printf("server: got connection from %s\n",inet_ntoa(remote.sin_addr));
		if (!fork()) { // this is the child process
			close(master_socket); // child doesn't need the listener
			if (send(child_socket, "Hello, world!\n", 14, 0) == -1)
				logging_critical("send");
			close(child_socket);
			exit(0);
		}
		close(child_socket);  // parent doesn't need this
	}

	logging_debug_low("waiting for players officially.");
	
}
