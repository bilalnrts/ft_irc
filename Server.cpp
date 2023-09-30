#include "Server.hpp"

Server::Server(int port, int maxClient, int maxBufferSize) : serverSocket(-1), PORT(port), MAX_CLIENTS(maxClient), MAX_BUFFER_SIZE(maxBufferSize)
{
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
}

bool	Server::start()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		perror("Error !\nSocket initialization failed !\n");
		return (false);
	}
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		perror("Error !\nBind failed !\n");
		return (false);
	}
	if (listen(serverSocket, MAX_CLIENTS) == -1) {
		perror("Error !\nListen failed !\n");
		return (false);
	}
	std::cout << *this << std::endl;
	return (true);
}

void	Server::run()
{
	struct pollfd fds[MAX_CLIENTS];
	fds[0].fd = serverSocket;
	fds[0].events = POLLIN;

	for (int i = 1; i < MAX_CLIENTS; ++i) {
		fds[i].fd = -1;
	}
	while (true)
	{
		int	numReady = poll(fds, MAX_CLIENTS, -1);
		if (numReady == -1) {
			perror("Error !\n");
		}
		if (fds[0].revents & POLLIN) {
			acceptClient(fds);
		}
		for (int i = 1; i < MAX_CLIENTS; i++) {
			if (fds[i].fd != -1 && (fds[i].revents & (POLLIN | POLLHUP)))
			{
				handleClient(fds, i);
			}
		}
	}
}

void	Server::acceptClient(struct pollfd fds[])
{
	int	newClient = accept(serverSocket, 0, 0);
	if (newClient == -1) {
		perror("Error !\n Accept failed !\n");
		return ;
	}
	for (int i = 1; i < MAX_CLIENTS; i++) {
		if (fds[i].fd == -1) {
			fds[i].fd = newClient;
			fds[i].events = POLLIN;
			break ;
		}
	}
	std::cout << "New client connected !" << std::endl;
}

void	Server::handleClient(struct pollfd fds[], int index)
{
	int clientSocket = fds[index].fd;
	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));

	int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesRead <= 0) {
		close(clientSocket);
		fds[index].fd = -1;
	}
	else {
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (fds[i].fd != -1 && (fds[i].fd != clientSocket)) {
				send(fds[i].fd, buffer, strlen(buffer), 0);
			}
		}
	}
}

int	Server::getPort() const
{
	return (this->PORT);
}

std::ostream &operator<<(std::ostream &o, const Server &s) {
	o << "Server port : " << s.getPort();
	return (o);
}
