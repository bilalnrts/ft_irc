#include "../inc/Server.hpp"
#include "../inc/User.hpp"

Server::Server(int port, std::string password) : serverSocket(-1), PORT(port), password(password)
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
	if (!setHostname())
		return (false);
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
			if (fds[i].fd != -1 && (fds[i].revents == POLLIN))
				handleClient(fds, i);
			else if (fds[i].fd != -1 && (fds[i].revents == POLLHUP )) {
				//will create close process here
				close(fds[i].fd);
				fds[i].fd = -1;
			}
		}
	}
}

void	Server::acceptClient(struct pollfd fds[])
{
	struct sockaddr_in	clientAddress;

	int	newClient = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), reinterpret_cast<socklen_t*>(&clientAddress));
	if (newClient == -1) {
		perror("Error !\n Accept failed !\n");
		return ;
	}
	for (int i = 1; i < MAX_CLIENTS; i++) {
		if (fds[i].fd == -1) {
			if (fcntl(newClient, F_SETFL, O_NONBLOCK) == -1)
				perror("Error !\nFcntl didn't work as excepted!\n");
			fds[i].fd = newClient;
			fds[i].events = POLLIN;
			this->userList.push_back(new User(fds[i].fd));
			this->sender(fds[i].fd, "Bilo'nun IRC Serverina hosgeldin !");
			break ;
		}
	}
	std::cout << "New client connected !" << std::endl;
}

void	Server::handleClient(struct pollfd fds[], int index)
{
	int clientSocket = fds[index].fd;
	char buffer[MAX_BUFFER_SIZE];
	Execute exec;

	memset(buffer, 0, sizeof(buffer));
	int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesRead <= 0) {
		perror("Error !\nrecv didn't work as excepted!\n");
	}
	else {
		std::string msg = utils::trimBuffer(buffer);
		exec.execute(fds[index].fd, this, msg);
	}
}

void	Server::sender(int &fd, std::string message)
{
	std::string buffer = message + "\r\n";
	send(fd, buffer.c_str(), buffer.length(), 0);
}

int	Server::getPort() const
{
	return (this->PORT);
}

std::ostream &operator<<(std::ostream &o, const Server &s) {
	o << "Server port : " << s.getPort();
	return (o);
}

User	*Server::findUser(int fd)
{
	for (size_t i = 0; i < userList.size(); i++) {
		if (userList[i]->getUserFd() == fd)
			return (userList[i]);
	}
	return (NULL);
}

User	*Server::findUser(std::string nickname)
{
	for (size_t i = 0; i < this->userList.size(); i++)
	{
		if (this->userList[i]->getNickname() == nickname)
			return (this->userList[i]);
	}
	return (NULL);
}

std::string Server::getPassword() const
{
	return (this->password);
}

std::string Server::getHostname() const
{
	return (this->hostname);
}

bool	Server::setHostname()
{
	char hName[1024];
	if (gethostname(hName, 1024) == -1) {
		perror("Error !\nHostname couldn't be set !\n");
		return (false);
	}
	this->hostname = hName;
	return (true);
}

void Server::addChannelList(Channel *channel)
{
	this->channelList.push_back(channel);
}


bool Server::findChannel(std::string name)
{
	for (size_t i = 0; i < this->channelList.size(); i++)
	{
		if (this->channelList[i]->getName() == name)
			return (true);
	}
	return (false);
}

Channel	*Server::getChannel(std::string name)
{
	for (size_t i = 0; i < this->channelList.size(); i++)
	{
		if (this->channelList[i]->getName() == name)
			return (this->channelList[i]);
	}
	return (NULL);
}

void	Server::removeChannel(Channel *channel)
{
	for (size_t i = 0; i < this->channelList.size(); i++)
	{
		if (this->channelList[i] == channel)
		{
			this->channelList.erase(this->channelList.begin() + i);
			break ;
		}
	}
}

void	Server::removeUser(User *user)
{
	for (size_t i = 0; i < this->userList.size(); i++)
	{
		if (this->userList[i] == user)
		{
			this->userList.erase(this->userList.begin() + i);
			close(user->getUserFd());
			break ;
		}
	}
}

std::vector<Channel*>	Server::getChannelList()
{
	return (this->channelList);
}	