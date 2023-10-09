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
		bool	rtn = true;

		if (size < 2) {
			numeric::sendNumeric(ERR_NONICKNAMEGIVEN, server, user);
			rtn = false;
		}
		if (server->findUser(nick) != NULL) {
			numeric::sendNumeric(ERR_NICKNAMEINUSE(nick), server, user);
			rtn = false;
		}
		if (isnumber(nick[0]) == 1 || nick.size() > 30 || nick.find_first_of(" \t\r\n\v\f") != std::string::npos
			|| nick.find_first_not_of(VALIDCHARS) != std::string::npos) {
			numeric::sendNumeric(ERR_ERRONEUSNICKNAME(nick), server, user);
			rtn = false;
		}
		return (rtn);
	}
}
