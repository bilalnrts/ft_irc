#ifndef EXECUTE_HPP
#define EXECUTE_HPP

#include "Server.hpp"

class Server;

class Execute
{
	public :
		void	execute(int fd, Server *server, std::string msg);
};

#endif
