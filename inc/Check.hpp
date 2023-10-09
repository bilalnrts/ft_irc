#ifndef CHECK_HPP
#define CHECK_HPP

#include "User.hpp"
#include "Server.hpp"

class Server;
class User;

#define VALIDCHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-"

namespace check
{
	bool	nick(std::string nick, int size, User *user, Server *server);
	bool	pass(std::string cmd, std::string pass, int size, User *user, Server *server);
}

#endif
