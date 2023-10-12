#include "../inc/Utils.hpp"

namespace utils
{
	std::string getPrefix(User *user)
	{
		return (":" + user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname());
	}

	std::vector<std::string>	split(std::string msg, std::string chars)
	{
		std::vector<std::string>	spl;
		size_t						index;
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

	std::string trimBuffer(std::string buffer)
	{
		std::string trimmed = buffer;
		trimmed.erase(trimmed.find_last_not_of("\r\n") + 1);
		return trimmed;
	}

	std::string getTime()
	{
		time_t now = time(0);
		tm *ltm = localtime(&now);
		std::string time = std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);
		return (time);
	}
}
