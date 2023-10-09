#include "../inc/Execute.hpp"

void	Execute::execute(int fd, Server *server, std::string msg)
{
	std::string cmd = msg.substr(0, msg.find(' '));
	std::cout << "command : " + cmd << std::endl;
	if (cmd == "NICK" || cmd == "USER" || cmd == "PASS")
	{
		if (cmd == "NICK")
			Command::nick(fd, server, msg);
		if (cmd == "USER")
			Command::user(fd, server, msg);
		if (cmd == "PASS")
			Command::pass(fd, server, msg);
	}
	else {
		std::cout << "KRAL DAHA DIGER KOMUTLARI HANDLE ETMEDIK YA" << std::endl;
	}
	//User *user = server->findUser(fd);
}
