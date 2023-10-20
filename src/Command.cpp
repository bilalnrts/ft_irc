#include "../inc/Command.hpp"

namespace Command
{
	void	nick(int fd, Server *server, std::string msg)
	{
		User *user = server->findUser(fd);
		std::vector<std::string> spl = utils::split(msg, " ");
		std::string nick;

		for (size_t i = 1; i < spl.size(); i++) {
			nick.append(spl[i]);
		}
		if (!check::nick(nick, spl.size(), user, server)) {
			return ;
		}
		user->setNickname(nick);
		user->setAuths("NICK", true);
		std::vector<Channel *> channels = user->getChannels();
		for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) {
			std::vector<User *> users = (*it)->getUserList();
			for (std::vector<User *>::iterator it2 = users.begin(); it2 != users.end(); it2++) {
				int toSend = (*it2)->getUserFd();
				if (fd != toSend)
					server->sender(toSend, utils::getPrefix(user) + " NICK " + nick);
			}
		}
		if (utils::checkAuth(user))
			utils::welcome(user, server);
	}

	void	pass(int fd, Server *server, std::string msg)
	{
		User *user = server->findUser(fd);
		std::vector<std::string> spl = utils::split(msg, ":");

		if (check::pass(spl[0], spl[1], spl.size(), user, server)) {
			user->setAuths("PASS", true);
		}
		if (utils::checkAuth(user))
			utils::welcome(user, server);
	}


	void	user(int fd, Server *server, std::string msg)
	{
		User *user = server->findUser(fd);
		std::vector<std::string> splWithColon = utils::split(msg, ":");
		std::vector<std::string> splWithSpace = utils::split(msg, " ");

		if (check::user(splWithColon, splWithSpace, user, server, fd)) {
			user->setUsername(splWithSpace[1]);
			user->setRealname(splWithColon[1]);
			user->setMode(std::stoi(splWithSpace[2], 0, 10));
			user->setHostname(splWithSpace[3]);
			user->setAuths("USER", true);
		}
		if (utils::checkAuth(user))
			utils::welcome(user, server);
	}

	void	cap(int fd, Server *server, std::string msg)
	{
		User *user = server->findUser(fd);
		(void)msg;
		if (user->getCap() == false)
		{
			server->sender(fd, "CAP * LS :multi-prefix sasl");
			user->setCap(true);
		}
		else
		{
			server->sender(fd, "CAP * ACK multi-prefix");
			user->setCap(false);
		}
	}

	void	join(int fd, Server *server, std::string msg)
	{
		Channel *channel = server->getChannel(msg);
		User	*user = server->findUser(fd);

		if (channel == NULL)
		{
			Channel *channel = new Channel(msg);
				channel->addUser(user);
				server->addChannelList(channel);
				channel->setOwner(user);
				channel->addEditor(user);
				server->sender(fd , utils::getPrefix(user) + " JOIN " + msg);
				server->sender(fd, utils::getPrefix(user) + " MODE " + msg + " +o " + user->getNickname());
				user->addChannel(channel);
				numeric::sendNumeric(RPL_NOTOPIC(user->getNickname(), msg), server, user);
				return ;
		}
		if (channel->getUser(user) == NULL)
		{
			server->sender(fd , utils::getPrefix(user) + " JOIN " + msg);
			channel->addUser(user);
			user->addChannel(channel);
			if (channel->getTopic() != "")
				numeric::sendNumeric(RPL_TOPIC(user->getNickname(), msg, channel->getTopic()), server, user);
			else
				numeric::sendNumeric(RPL_NOTOPIC(user->getNickname(), msg), server, user);
		}
		std::string channelName = channel->getName();
		std::vector<User *> users = channel->getUserList();
		std::string allnickname = "";
		for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)
		{
			int toSend = (*it)->getUserFd();
			if (fd != toSend)
				server->sender(toSend, utils::getPrefix(user) + " JOIN " + msg);
			if (it != users.begin())
				allnickname += (*it)->getNickname() + " ";
			if (channel->getOwner() == user)
				allnickname += "@";
		}
		numeric::sendNumeric(RPL_NAMEREPLY(nickname, channelName, allnickname), server, user);
		numeric::sendNumeric(RPL_ENDOFNAMES(nickname, channelName), server, user);
	}

	void part(int fd, Server *server, std::string msg)
	{
		User *user = server->findUser(fd);
		Channel *channel = server->getChannel(msg);

		if (user == channel->getOwner()) {
			channel->changeOwner(server);
		}
		channel->removeUser(user);
		user->removeChannel(channel);
		channel->removeEditor(user);
		std::vector<User*> users = channel->getUserList();
		for (std::vector<User*>::iterator it = users.begin(); it != users.end(); it++)
		{
			int tosend = (*it)->getUserFd();
			server->sender(tosend, utils::getPrefix(user) + " PART " + channel->getName());
		}
		if (channel->getUserList().size() == 0)
		{
			server->removeChannel(channel);
			delete channel;
			return ;
		}
	}
// notice
// quit
// part
	void privMsg(int fd, Server *server, std::vector<std::string> split)
	{
		if (split[1][0] == '#') {
			//channel
			Channel *channel = server->getChannel(split[1]);
			std::vector<User *> users = channel->getUserList();
			for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++) {
				User *user = server->findUser((*it)->getUserFd());
				int userFd = user->getUserFd();
				if (userFd != fd)
					server->sender(userFd, utils::getPrefix(user) + " PRIVMSG " + split[1] + " : " + split[2]);
			}
		} else
		{
			User *user = server->findUser(split[1]);
			int userFd = user->getUserFd();
			if (userFd != fd)
				server->sender(userFd, utils::getPrefix(server->findUser(fd)) + " PRIVMSG " + split[1] + " : " + split[2]);
		}
	}

	void topic(int fd, Server *server, std::vector<std::string> split)
	{
		User *user = server->findUser(fd);
		if (split.size() == 3)
		{
			std::string channelName = split[1];
			Channel *channel = server->getChannel(channelName);
			std::string topic = split[2];
			server->sender(fd, utils::getPrefix(user) + " TOPIC " + channelName + " :" + topic);
			 std::vector<User *> userList = channel->getUserList();
			for (std::vector<User*>::iterator it = userList.begin(); it != userList.end(); it++)
			{
				int toSend = (*it)->getUserFd();
				if (toSend != fd)
					server->sender(toSend, utils::getPrefix(user) + " TOPIC " + channelName + " :" + topic);
			}
			channel->setTopic(topic);
		}
		if (split.size() == 2)
		{
			std::string channelName = split[1];
			Channel *channel = server->getChannel(channelName);
			if (channel->getTopic() != "")
				numeric::sendNumeric(RPL_TOPIC(user->getNickname(), channelName, channel->getTopic()), server, user);
			else
				numeric::sendNumeric(RPL_NOTOPIC(user->getNickname(), channelName), server, user);
		}
	}

	void quit (int fd, Server *server, std::vector<std::string> split)
	{
		User *user = server->findUser(fd);
		if (split[1][0] == ':')
			split[1].erase(0, 1);
		std::string message = split[1];
		std::vector<Channel *> channels = user->getChannels();
		for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			std::vector<User *> users = (*it)->getUserList();
			for (std::vector<User *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
			{
				int toSend = (*it2)->getUserFd();
				if (fd != toSend)
					server->sender(toSend, utils::getPrefix(user) + " QUIT :" + message);
			}
		}
		server->removeUser(user);
		delete user;
	}

	void notice (int fd , Server *server, std::vector<std::string> split)
	{
		std::vector<Channel *> channelList = server->getChannelList();
		int	targetFd = server->findUser(split[1])->getUserFd();

		for (std::vector<Channel *>::iterator it = channelList.begin(); it != channelList.end(); it++) {
			std::vector<User *> userList = (*it)->getUserList();
			for (std::vector<User *>::iterator it2 = userList.begin(); it2 != userList.end(); it2++) {
				if ((*it2)->getUserFd() == fd) {
					server->sender(targetFd, utils::getPrefix(server->findUser(fd)) + " NOTICE " + (*it)->getName() + " :" + split[2]);
				}
			}
		}
	}

	void kick (int fd , Server *server, std::vector<std::string> split)
	{
		int size = split.size();
		std::string channelName = split[1];
		std::string nickName = split[2];
		Channel *channel = server->getChannel(channelName);
		User *user = server->findUser(fd);
		User *user2 = server->findUser(nickName);
		if (channel->getOwner() != user)
		{
			numeric::sendNumeric(ERR_CHANOPRIVSNEEDED(channelName), server, user);
			return ;
		}
		std::vector<User *> users = channel->getUserList();
		for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)
		{
			int toSend = (*it)->getUserFd();
			if (fd != toSend)
				server->sender(toSend, utils::getPrefix(user) + " KICK " + channelName + " " + nickName);
			else if (fd != toSend && size == 4)
			{
				std::string message = split[3];
				server->sender(toSend, utils::getPrefix(user) + " KICK " + channelName + " " + nickName + " :" + message);
			}
		}
		channel->removeUser(user2);
		user2->removeChannel(channel);
		if (channel->getUserList().size() == 0)
		{
			server->removeChannel(channel);
			delete channel;
			return ;
		}
	}
}
