#include "Utils.hpp"

namespace utils
{
	std::string getPrefix(User *user)
	{
		return (":" + user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname());
	}
}
