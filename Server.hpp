#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

class Server
{
	private :
		int					serverSocket;
		struct sockaddr_in	serverAddr;
		int					PORT;
		int					MAX_CLIENTS;
		int					MAX_BUFFER_SIZE;
		void				acceptClient(struct pollfd fds[]);
		void				handleClient(struct pollfd fds[], int index);

	public :
		Server(int port, int maxClient, int maxBufferSize);
		int		getPort() const;
		bool	start();
		void	run();
};

std::ostream &operator<<(std::ostream &o, const Server &s);

#endif
