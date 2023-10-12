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
	void	join(int fd, Server *server, std::string msg);
	void	part(int fd, Server *server, std::string msg);
	void	privMsg(int fd, Server *server, std::vector<std::string> split);
	void	topic(int fd, Server *server, std::vector<std::string> split);
	void	quit(int fd, Server *server, std::vector<std::string> split);
	void 	notice(int fd , Server *server, std::vector<std::string> split);

}

#endif
