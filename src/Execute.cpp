#include "../inc/Execute.hpp"

bool	checkAuth(User *user)
{
	if (!user->getAuths("PASS") || !user->getAuths("NICK") || !user->getAuths("USER")) {
		return (false);
	}
	user->setAuth();
	return (true);
}

void	Execute::execute(int fd, Server *server, std::string msg)
{
	User *user = server->findUser(fd);
	std::string cmd = msg.substr(0, msg.find(' '));
	if (!checkAuth(user) && (cmd != "NICK" && cmd != "USER" && cmd != "PASS"))
	{
		server->sender(fd, "Error !\nYou didn't verify your identity yet !");
	}
	else {
		if (cmd == "NICK")
			Command::nick(fd, server, msg);
		if (cmd == "USER")
			Command::user(fd, server, msg);
		if (cmd == "PASS")
			Command::pass(fd, server, msg);
	}
}
