#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "User.hpp"
#include "Utils.hpp"
#include "Numeric.hpp"
#include "Check.hpp"

class Server;

namespace Command
{
	void	nick(int fd, Server *server, std::string msg);
	void	pass(int fd, Server *server, std::string msg);
	void	user(int fd, Server *server, std::string msg);
}

#endif
