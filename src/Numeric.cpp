#include "../inc/Numeric.hpp"

namespace numeric
{
	Numeric makeNumeric(std::string code, std::string message)
	{
		Numeric numeric;

		numeric.first = code;
		numeric.second = message;
		return (numeric);
	}

	void sendNumeric(Numeric reply, Server *server, User *user)
	{
		int fd = user->getUserFd();
		server->sender(fd, ":" + server->getHostname() + " " + reply.first + " " + user->getNickname() + " " + reply.second); // added : + 
	}
}
