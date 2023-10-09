#include "Command.hpp"
#include <typeinfo>

namespace Command
{
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

	void	user(int fd, Server *server, std::string msg) // YORULDUM SALIYORUM VALLA
	{
		std::cout << "User komutu calisti" << std::endl;
	}
}
