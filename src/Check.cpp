#include "../inc/Check.hpp"

namespace check
{
	/*
		Command: NICK
		Parameters: <nickname>
		NUMERIC REPLIES :
		+ERR_NONICKNAMEGIVEN	->	":No nickname given"										->	Returned when a nickname parameter expected for a command and isn't found.
		+ERR_NICKNAMEINUSE		->	"<nick> :Nickname is already in use"						->	Returned when a NICK message is processed that results in
																									an attempt to change to a currently existing nickname.
		-ERR_UNAVAILRESOURCE	->	"<nick/channel> :Nick/channel is temporarily unavailable"	->	Returned by a server to a user trying to join a channel
																									currently blocked by the channel delay mechanism.
																									Returned by a server to a user trying to change nickname
																									when the desired nickname is blocked by the nick delay mechanism.
		-ERR_ERRONEUSNICKNAME	->	"<nick> :Erroneous nickname"								->	Returned after receiving a NICK message which contains
																									characters which do not fall in the defined set. See
																									section 2.3.1 for details on valid nicknames.
		-ERR_NICKCOLLISION		->	"<nick> :Nickname collision KILL from <user>@<host>"		->	Returned by a server to a client when it detects a
																									nickname collision (registered of a NICK that
																									already exists by another server).
		-ERR_RESTRICTED			->	":Your connection is restricted!"							->	Sent by the server to a user upon connection to indicate
																									the restricted nature of the connection (user mode "+r").
		EXAMPLE :
			NICK Wiz
	*/
	bool	nick(std::string nick, int size, User *user, Server *server)
	{
		if (size < 2) {
			numeric::sendNumeric(ERR_NONICKNAMEGIVEN, server, user);
			return (false);
		}
		if (server->findUser(nick) != NULL) {
			numeric::sendNumeric(ERR_NICKNAMEINUSE(nick), server, user);
			return(false);
		}
		if (isnumber(nick[0]) == 1 || nick.size() > 30 || nick.find_first_of(" \t\r\n\v\f") != std::string::npos
			|| nick.find_first_not_of(VALIDCHARS) != std::string::npos) {
			numeric::sendNumeric(ERR_ERRONEUSNICKNAME(nick), server, user);
			return(false);
		}
		return (true);
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
	bool	pass(std::string cmd, std::string pass, int size, User *user, Server *server)
	{
		if (size < 2) {
			numeric::sendNumeric(ERR_NEEDMOREPARAMS(cmd), server, user);
			return(false);
		}
		if (user->getAuths(cmd)) {
			numeric::sendNumeric(ERR_ALREADYREGISTRED, server, user);
			return(false);
		}
		if (server->getPassword() != pass)
			return (false);
		return(true);
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
	bool	user(std::vector<std::string> splWithColon, std::vector<std::string> splWithSpace, User *user, Server *server, int fd)
	{
		int					mode;
		std::stringstream	ss;
		if (user->getAuths("USER")) {
			numeric::sendNumeric(ERR_ALREADYREGISTRED, server, user);
			return (false);
		}
		if (splWithColon.size() < 2 || splWithSpace.size() < 5) {
			numeric::sendNumeric(ERR_NEEDMOREPARAMS(splWithSpace[0]), server, user);
			return (false);
		}
		ss << splWithSpace[2]; // mode param
		ss >> mode;
		if (ss.fail()) {
			server->sender(fd, "Error :Invalid mode");
			return (false);
		}
		return (true);
	}
}
