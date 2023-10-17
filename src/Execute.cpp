#include "../inc/Execute.hpp"
#include "../inc/Command.hpp"
#include "../inc/User.hpp"


bool	checkAuth(User *user, Server *server)
{
	if (!user->getAuths("PASS") || !user->getAuths("NICK") || !user->getAuths("USER")) {
		return (false);
	}
	user->setAuth();
	if (user->isAuth() == true)
	{
		std::string nickname = user->getNickname();
		std::string username = user->getUsername();
		std::string hostname = user->getHostname();
		std::string createtime = utils::getTime();
		numeric::sendNumeric(RPL_WELCOME(nickname, username, hostname), server, user);
		numeric::sendNumeric(RPL_YOURSERVICE(nickname, hostname), server, user);
		numeric::sendNumeric(RPL_CREATED(nickname,createtime), server, user);
	}
	return (true);
}

void joinControl(int &fd, Server *server, std::vector<std::string> split)
{

	if (split.size() == 2)
	{
		if (split[1][0] == '#' && split[1][1] != '0')
			Command::join(fd, server, split[1]);
		else
			return ;
	}
}

void partControl(int &fd, Server *server, std::vector<std::string> split)
{
	if (split.size() > 2)
	{
		if (split[1][0] == '#')
			Command::part(fd, server, split[1]);
		else
			return ;
	}
	else
		return ;
}

void privMsgControl(int &fd, Server *server, std::vector<std::string> split)
{
	User *user = server->findUser(fd);
	std::string nickName = user->getNickname();
	if (split.size() == 2)
	{
		if (split[1][0] == '#')
			Command::privMsg(fd, server, split);
		else
		{
			User *user1 = server->findUser(nickName);
			if (user == NULL)
				numeric::sendNumeric(ERR_NOSUCHNICK(split[1]), server, user1);
			else
			{
				int recvfd = user->getUserFd();
				server->sender(recvfd, utils::getPrefix(server->findUser(fd)) + " PRIVMSG " + split[1] + " :" + split[1]);    // check this line
			}
		}
	}
	else if (split.size() == 3)
	{
		if (split[1][0] == '#' && split[2] != "")
			Command::privMsg(fd, server, split);
	}
	else if (split.size() < 2)
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(split[0]), server, user);
}

void topicControl(int &fd, Server *server, std::vector<std::string> split)
{
	User *user = server->findUser(fd);

	if (split[1][0] == '#' && split[2] != "" && split[2][0] == ':') {
    int size = split.size();
	split[2].erase(0, 1);

    std::string message;
    for (int i = 2; i < size; i++) {
        message += split[i];
        if (i < size - 1) {
            message += " ";
        }
    }
	std::vector<std::string> newsplit;
	newsplit.push_back(split[0]);
	newsplit.push_back(split[1]);
	newsplit.push_back(message);
	Command::topic(fd, server, newsplit);
	}
	else if (split[1][0] == '#' && split[2] == "")
	{
		numeric::sendNumeric(RPL_NOTOPIC(user->getNickname(), split[1]), server, user);
	}
	else if (split[1][0] == '#' && split[2] != "" || split[2][0] != ':')
	{
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(split[0]), server, user);
	}
	else
		return ;
}

void quitControl(int &fd, Server *server, std::vector<std::string> split)
{
	if (split.size() == 2)
	{
		Command::quit(fd, server, split);
	}
	else
		return ; //add if commnad quit without message

}

void noticeControl(int &fd, Server *server, std::vector<std::string> split)
{
	if (split.size() == 3)
	{
		if (split[1][0] == '#')
			Command::notice(fd, server, split);
		else
			return ;
	}
	else
		return ;
}


void cmdall(int &fd, Server *server, std::string msg)
{
	std::vector<std::string> split = utils::split(msg, " ");
	for (size_t i = 0;  i < split.size(); i++)
	{
		std::cout << "--------------------------------" << std::endl;
		if (i == 0)
			std::cout << "User : " << server->findUser(fd)->getNickname() << std::endl;
		std::cout << "Message: " << split[i] << std::endl;
		std::cout << "Client fd -> " << fd << std::endl;
		std::cout << "--------------------------------" << std::endl;
	}
	if (split[0] == "JOIN")
		joinControl(fd, server, split);
	else if (split[0] == "PART")
		partControl(fd, server, split);
	else if (split[0] == "PRIVMSG")
		privMsgControl(fd, server, split);
	else if (split[0] == "TOPIC")
		topicControl(fd, server, split);
	else if (split[0] == "QUIT")
		quitControl(fd, server, split);
	else if (split[0] == "NOTICE")
		noticeControl(fd, server, split);
	else
		std::cout << "Command not found !" << std::endl;
}

void	Execute::execute(int fd, Server *server, std::string msg)
{
	User *user = server->findUser(fd);
	std::string cmd = msg.substr(0, msg.find(' '));
	if (!checkAuth(user, server) && (cmd != "NICK" && cmd != "USER" && cmd != "PASS"))
	{
		server->sender(fd, "Error !\nYou didn't verify your identity yet !");
	}
	else {
		if (cmd == "NICK")
			Command::nick(fd, server, msg);
		if (cmd == "USER")
			Command::user(fd, server, msg);
		if (cmd == "PASS")
			Command::pass(fd, server, msg);
		else
		{
			cmdall(fd, server, msg);
		}

	}
}
