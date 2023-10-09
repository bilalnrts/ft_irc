#include "Utils.hpp"

namespace utils
{
	std::string getPrefix(User *user)
	{
		return (":" + user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname());
	}
	std::vector<std::string>	split(std::string msg, std::string chars)
	{
		std::vector<std::string>	spl;
		int							index;
		std::string					little;

		while ((index = msg.find(chars)) != std::string::npos)
		{
			little = msg.substr(0, index);
			spl.push_back(little);
			msg.erase(0, index + chars.length());
		}
		spl.push_back(msg);
		return (spl);
	}
}
