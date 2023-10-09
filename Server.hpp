#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include "User.hpp"
#include "Channel.hpp"

class User;
class Channel;

#define MAX_CLIENTS 100
#define MAX_BUFFER_SIZE 1024

class Server
{
	private :
		int						serverSocket;							//Server socket fd && Values are assigned in the start() function
		struct sockaddr_in		serverAddr;								//Keeps port and protocol information && Values are assigned during creation (constructor)
		int						PORT;									//Listened port number && Values are assigned during creation (constructor)
		std::vector<User*>		userList;								//List of connected users
		std::vector<Channel*>	channelList;							//List of created channels
		std::string				password;								//Server password && has get methods && Values are assigned during creation (constructor)
		std::string				hostname;								//Hostname (will create later) has get set methods
		std::string				createdTime;							//Server creation time
		void				acceptClient(struct pollfd fds[]);
		void				handleClient(struct pollfd fds[], int index);

	public :
		Server(int port, std::string password);
		int		getPort() const;
		bool	start();
		void	run();
		void	sender(int &fd, std::string message);
		User				*findUser(int fd);
		std::string			getPassword() const;
		std::string			getHostname() const;
		bool				setHostname();
};

std::ostream &operator<<(std::ostream &o, const Server &s);

#endif
