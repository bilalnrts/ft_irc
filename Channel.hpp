#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <iostream>
#include <vector>

class User;
class Server;

class Channel
{
	private :
		std::vector<User *>	userList;
		std::vector<User *>	editorList;
		User				*owner;
		std::string			name;
		std::string			topic;
	public :
		Channel();
		Channel(std::string name);
		~Channel();
		void	addUser(User *user);
		void	removeUser(User *user);
		void	addEditor(User *user);
		void	removeEditor(User *user);
		void	setOwner(User *user);
		void	setName(std::string name);
		void	setTopic(std::string topic);
		void	changeOwner(Server *server);
		std::string	getTopic() const;
		std::string	getName() const;
		std::vector<User *> getUserList() const;
		std::vector<User *> getEditorList() const;
		User	*getOwner() const;
		User	*getUser(User *user);
		User	*getEditor(User *user);
};

#endif
