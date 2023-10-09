#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include "Server.hpp"
#include "User.hpp"

typedef std::pair<std::string, std::string> Numeric;
class Server;
class User;

// NUMERIC ERRORS
#define ERR_NICKNAMEINUSE(nick) numeric::makeNumeric("433", nick + " :Nickname is already in use")
#define ERR_NONICKNAMEGIVEN numeric::makeNumeric("431", ":No nickname given")
#define ERR_ERRONEUSNICKNAME(nick) numeric::makeNumeric("432", nick + " :Erroneous nickname")
namespace numeric
{
	Numeric makeNumeric(std::string code, std::string message);
	void	sendNumeric(Numeric reply, Server *server, User *user);
}

#endif
