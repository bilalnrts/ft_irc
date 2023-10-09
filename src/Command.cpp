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

	/*
		Command: PASS
		Parameters: <password>
		NUMERIC REPLIES :
		-ERR_NEEDMOREPARAMS		->	"<command> :Not enough parameters"				->	Returned by the server by numerous commands to
																						indicate to the client that it didn't supply enough parameters.
		-ERR_ALREADYREGISTRED	->	":Unauthorized command (already registered)"	->	Returned by the server to any link which tries to
																						change part of the registered details (such as
																						password or user details from second USER message).
		EXAMPLE :
			PASS secretpasswordhere
	*/
	void	pass(int fd, Server *server, std::string msg) //TOPLAM 2 HATA HANDLE EDİLECEK
	{
		User *user = server->findUser(fd);
		std::vector<std::string> spl = utils::split(msg, " ");
		std::string pass;

		pass = spl[1];
		if (!pass.empty() && pass[pass.length() - 1] == '\n') {         pass.erase(pass.length() - 1);     }
		std::cout << "msg geldi : " + msg << " pass : " + pass << std::endl;
		if (spl.size() < 2)
		{
			//NUMERIC REPLY -> ERR_NEEDMOREPARAMS
			std::cout << "Password yok" << std::endl;
		}
		std::cout << "ser pass : " + server->getPassword() << std::endl;
		std::cout << "ser type : " << typeid(server->getPassword()).name() << std::endl;
		std::cout << "pass type : " << typeid(pass).name() << std::endl;
		if (pass == server->getPassword()) {
			user->setAuths("PASS", true);
			std::cout << "True burasi" << std::endl;
		}
	}

	/*
		Command: USER
		Parameters: <user> <mode> <unused> <realname>
		NUMERIC REPLIES :
		-ERR_NEEDMOREPARAMS		->	"<command> :Not enough parameters"				->	Returned by the server by numerous commands to
																						indicate to the client that it didn't supply enough parameters.
		-ERR_ALREADYREGISTRED	->	":Unauthorized command (already registered)"	->	Returned by the server to any link which tries to
																						change part of the registered details (such as
																						password or user details from second USER message).
		EXAMPLE :
			USER guest 0 * :Ronnie Reagan
	*/
	void	user(int fd, Server *server, std::string msg) // YORULDUM SALIYORUM VALLA
	{
		(void)fd;
		(void)server;
		(void)msg;
		std::cout << "User komutu calisti" << std::endl;
	}
}
