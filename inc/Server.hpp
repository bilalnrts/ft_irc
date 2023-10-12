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
		void	addChannelList(Channel *channel); // add a channel to the channel list
		User				*findUser(int fd);
		User				*findUser(std::string nickname);
		std::string			getPassword() const;
		std::string			getHostname() const;
		Channel				*getChannel(std::string name);
		bool				setHostname();
		bool				findChannel(std::string name); //returns a vector of channels with the same name
		void				removeChannel(Channel *channel); // remove a channel from the channel list
		void				removeUser(User *user); // remove a user from the user list -- closed user fd
		std::vector<Channel*>	getChannelList();
};

std::ostream &operator<<(std::ostream &o, const Server &s);

#endif
