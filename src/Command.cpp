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
		std::cout << "Nick : " + user->getNickname() << std::endl;
		std::cout << "Real : " + user->getRealname() << std::endl;
		std::cout << "Host : " + user->getHostname() << std::endl;
		std::cout << "Mode : " << user->getMode() << std::endl;
	}
}
