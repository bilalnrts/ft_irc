#include "Channel.hpp"

Channel::Channel(): name("Default"), topic("Default") {}

Channel::Channel(std::string name): name(name), topic(" ") {}

Channel::~Channel() {}

void	Channel::addUser(User *user) {this->userList.push_back(user);}

void	Channel::removeUser(User *user)
{
	std::vector<User *>::iterator it = this->userList.begin();
	while (it != this->userList.end())
	{
		if (*it == user) {
			this->userList.erase(it);
			break ;
		}
		it++;
	}
}

void	Channel::addEditor(User *user) {this->editorList.push_back(user);}

void	Channel::removeEditor(User *user)
{
	std::vector<User *>::iterator it = this->editorList.begin();
	while (it != this->editorList.end())
	{
		if (*it == user) {
			this->editorList.erase(it);
			break ;
		}
		it++;
	}
}

void	Channel::setOwner(User *user) {this->owner = user;}

void	Channel::setName(std::string name) {this->name = name;}

void	Channel::setTopic(std::string topic) {this->topic = topic;}

std::string	Channel::getTopic() const {return(this->topic);}

std::string Channel::getName() const {return (this->name);}

std::vector<User *> Channel::getUserList() const {return (this->userList);}

std::vector<User *> Channel::getEditorList() const {return (this->editorList);}

User	*Channel::getOwner() const {return (this->owner);}

User	*Channel::getUser(User *user)
{
	std::vector<User *>::iterator it = this->userList.begin();
	while (it != this->userList.end())
	{
		if (*it == user) {return (*it);}
		it++;
	}
	return (0);
}

User	*Channel::getEditor(User *user)
{
	std::vector<User *>::iterator it = this->editorList.begin();
	while (it != this->editorList.end())
	{
		if (*it == user) {return (*it);}
		it++;
	}
	return (0);
}

void Channel::changeOwner(Server *server)
{
	if (this->userList.size() == 1) {this->owner = 0;}
	else
	{
		User *newOwner;
		srand(time(NULL));
		while (true)
		{
			int randomIndex = rand() % this->userList.size();
			newOwner = this->userList[randomIndex];
			if (newOwner != this->owner) {break;}
		}
		this->owner = newOwner;
		this->addEditor(this->owner);
		for (User *user : this->userList) {
			int toSend = user->getUserFd();
			server->sender(toSend, utils::getPrefix(user) + " MODE " + this->name + " +o " + this->owner->getNickname());
		} // set the user getters
	}
}

