#ifndef CHECK_HPP
#define CHECK_HPP

#include "User.hpp"
#include "Server.hpp"
#include <sstream>

class Server;
class User;

#define VALIDCHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-"

namespace check
{
	bool	nick(std::string nick, int size, User *user, Server *server);
	bool	pass(std::string cmd, std::string pass, int size, User *user, Server *server);
	bool	user(std::vector<std::string> splWithColon, std::vector<std::string> splWithSpace, User *user, Server *server, int fd);
}

#endif
