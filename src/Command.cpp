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
	}

	void	pass(int fd, Server *server, std::string msg)
	{
		User *user = server->findUser(fd);
		std::vector<std::string> spl = utils::split(msg, " ");

		if (check::pass(spl[0], spl[1], spl.size(), user, server)) {
			user->setAuths("PASS", true);
		}
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
		std::string nickname = user->getNickname();
		std::string username = user->getUsername();
		std::string hostname = user->getHostname();
		std::cout << "Nick : " + nickname << std::endl;
		std::cout << "Real : " + user->getRealname() << std::endl;
		std::cout << "Host : " + user->getHostname() << std::endl;
		std::cout << "Mode : " << user->getMode() << std::endl;
	}

	void	join(int fd, Server *server, std::string msg)
	{
		Channel *channel = server->getChannel(msg);
		User	*user = server->findUser(fd);

		/* if (check::checkJoin(msg, user, server))
			return ; */
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
			allnickname += (*it)->getNickname();
		}
		numeric::sendNumeric(RPL_NAMEREPLY(nickname, channelName, allnickname), server, user);
		numeric::sendNumeric(RPL_ENDOFNAMES(nickname, channelName), server, user);
	}

	void part(int fd, Server *server, std::string msg)
	{
		User *user = server->findUser(fd);
		Channel *channel = server->getChannel(msg);

		if (user == channel->getOwner())
			// must be change owner
		channel->removeUser(user);
		user->removeChannel(channel);
		// must be a remove operator 
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

	void privMsg(int fd, Server *server, std::vector<std::string> split)
	{
			User *user = server->findUser(fd);
			Channel *channel = server->getChannel(split[1]);
			std::string msg = "";
			std::string channelName = channel->getName();

			if (split.size() == 2)
				msg = split[1];
			else
				msg = split[2];
			if (channel == NULL)
			{
				numeric::sendNumeric(ERR_NOSUCHNICK(channelName), server, user);
				return ;
			}
			std::vector<User *> users = channel->getUserList();
			for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)
			{
				int fd2 = (*it)->getUserFd();
				if (fd != fd2)
					server->sender(fd2, utils::getPrefix(user) + " PRIVMSG " + channelName + " :" + msg);
			}
	}

	void topic(int fd, Server *server, std::vector<std::string> split)
	{
		User *user = server->findUser(fd);
		if (split.size() == 3)
		{
			std::string channelName = split[1];
			Channel *channel = server->getChannel(channelName);
			if (split[2][0] == ':')
				split[2].erase(0, 1);
			std::string topic = split[2];
			server->sender(fd, utils::getPrefix(user) + " TOPIC " + channelName + " :" + topic);
			 std::vector<User *> userList = channel->getUserList();
			for (std::vector<User*>::iterator it = userList.begin(); it != userList.end(); it++)
			{
				int toSend = (*it)->getUserFd();
				if (toSend != fd)
					server->sender(toSend, utils::getPrefix(user) + " TOPIC " + channelName + " :" + topic);
			}
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
		if (!&topic)
			// add numeric
			return ;
		else
			// add numeric
			return ;
	}

	void quit (int fd, Server *server, std::vector<std::string> split)
	{
		User *user = server->findUser(fd);
		if (split[1][0] == ':')
			split[1].erase(0, 1);
		std::string channelName = split[1];
		std::vector<Channel *> channels = user->getChannels();
		for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			std::vector<User *> users = (*it)->getUserList();
			for (std::vector<User *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
			{
				int toSend = (*it2)->getUserFd();
				if (fd != toSend)
					server->sender(toSend, utils::getPrefix(user) + " QUIT :" + channelName);
			}
		}
		server->removeUser(user);
		delete user;
	}

	void notice (int fd , Server *server, std::vector<std::string> split)
	{
		std::string channelName = split[1];
		std::string message = split[2];

		// fd ile alakalı bir şey yapılacak mı? Kontrol

		Channel *channel = server->getChannel(channelName);
		std::vector<Channel *> channelList = server->getChannelList();
		std::vector<User *> users = channel->getUserList();
		for (std::vector<Channel *>::iterator it2 = channelList.begin(); it2 != channelList.end(); it2++)
		{
			std::vector<User *>::iterator it = users.begin();
			for (; it != users.end(); it++)
			{
				if ((*it)->getUserFd() != fd)
				{
					std::string nickName = (*it)->getNickname();
					server->sender(fd, utils::getPrefix(server->findUser(fd)) + " NOTICE " + channelName + " :" + message);
				}
			}
		}
	}
}
