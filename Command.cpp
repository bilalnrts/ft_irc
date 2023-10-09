#include "Command.hpp"
#include <typeinfo>

namespace Command
{
	/*
		Command: NICK
		Parameters: <nickname>
		NUMERIC REPLIES :
		-ERR_NONICKNAMEGIVEN	->	":No nickname given"										->	Returned when a nickname parameter expected for a command and isn't found.
		-ERR_NICKNAMEINUSE		->	"<nick> :Nickname is already in use"						->	Returned when a NICK message is processed that results in
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
	void	nick(int fd, Server *server, std::string msg) //TOPLAM 6 HATA HANDLE EDİLMESİ LAZIM BURADA, DAHA SONRA HALLEDERİZ
	{
		User *user = server->findUser(fd);
		std::vector<std::string> spl = utils::split(msg, " ");
		std::string nick;

		if (spl.size() < 2)
		{
			//NUMERIC REPLY -> ERR_NONICKNAMEGIVEN -> :No nickname given
			std::cout << "Nickname yok" << std::endl;
		}
		for (int i = 1; i < spl.size(); i++) {
			nick.append(spl[i]);
		}
		if (user->getNickname() != "") {
			if (user->getNickname() == nick) {
				//NUMERIC REPLY -> ERR_NICKNAMEINUSE -> <nick> :Nickname is already in use
				std::cout << "Ayni nick" << std::endl;
			}
			//BURADA NICK DEĞİŞTİRDİĞİNİ DİĞER CLİENTLERE SÖYLEMEMİZ LAZIM
			std::cout << nick << std::endl;
		}
		user->setNickname(nick);
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
	void	pass(int fd, Server *server, std::string msg) //TOPLAM 2 HATA HANDLE EDİLECEK
	{
		User *user = server->findUser(fd);
		std::vector<std::string> spl = utils::split(msg, " ");
		std::string pass;

		pass = spl[1];
		if (!pass.empty() && pass[pass.length() - 1] == '\n') {         pass.erase(pass.length() - 1);     }
		std::cout << "msg geldi : " + msg << " pass : " + pass << std::endl;
		if (spl.size() < 2)
		{
			//NUMERIC REPLY -> ERR_NEEDMOREPARAMS
			std::cout << "Password yok" << std::endl;
		}
		std::cout << "ser pass : " + server->getPassword() << std::endl;
		std::cout << "ser type : " << typeid(server->getPassword()).name() << std::endl;
		std::cout << "pass type : " << typeid(pass).name() << std::endl;
		if (pass == server->getPassword()) {
			user->setAuths("PASS", true);
			std::cout << "True burasi" << std::endl;
		}
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
	void	user(int fd, Server *server, std::string msg) // YORULDUM SALIYORUM VALLA
	{
		std::cout << "User komutu calisti" << std::endl;
	}
}
