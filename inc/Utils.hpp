#ifndef UTILS_HPP
#define UTILS_HPP

#include "User.hpp"

class User;

namespace utils
{
	std::string getPrefix(User *user);
	std::vector<std::string>	split(std::string msg, std::string chars);
	std::string trimBuffer(std::string buffer);
	std::string getTime();
	bool		checkAuth(User *user);
	void		welcome(User *user, Server *server);
}

#endif
