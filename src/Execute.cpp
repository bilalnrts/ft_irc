#include "../inc/Execute.hpp"
#include "../inc/Command.hpp"
#include "../inc/User.hpp"

void joinControl(int &fd, Server *server, std::vector<std::string> split)
{

	if (split.size() == 2)
	{
		if (split[1][0] == '#' && split[1][1] != '0')
			Command::join(fd, server, split[1]);
		else if (split[1][0] == '#' && split[1][1] == '0') //leave all channel
		{
			User *user = server->findUser(fd);
			Channel *channel;
			std::vector<Channel *> channels = user->getChannels();

			for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) {
				channel = *it;
				Command::part(fd, server, channel->getName());
			}
		}
		else
		{
			numeric::sendNumeric(ERR_NEEDMOREPARAMS(split[0]), server, server->findUser(fd));
		}

	}
	else
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(split[0]), server, server->findUser(fd));
}

void partControl(int &fd, Server *server, std::vector<std::string> split)
{
	if (split.size() == 2)
	{
		if (split[1][0] == '#')
			Command::part(fd, server, split[1]);
		else
			numeric::sendNumeric(ERR_NEEDMOREPARAMS(split[0]), server, server->findUser(fd));
	}
	else
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(split[0]), server, server->findUser(fd));
}

std::vector<std::string>	splitHelper(std::vector<std::string> split)
{
		split[2].erase(0, 1);
		std::string message;
		for (size_t i = 2; i < split.size(); i++) {
			message += split[i];
			if (i < split.size() - 1) {
				message += " ";
			}
		}
		std::vector<std::string> newsplit;
		newsplit.push_back(split[0]);
		newsplit.push_back(split[1]);
		newsplit.push_back(message);

		return newsplit;
}

void privMsgControl(int &fd, Server *server, std::vector<std::string> split)
{
	User *user = server->findUser(fd);
	std::string nickName = user->getNickname();

	if (split.size() < 2 || split[2][0] != ':') {
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(split[0]), server, user);
		return ;
	}
	if (split[1][0] == '#') {
		Channel *channel = server->getChannel(split[1]);
		if (channel == NULL)
		{
			numeric::sendNumeric(ERR_NOSUCHNICK(split[1]), server, user);
			return ;
		}
		std::vector<std::string> newsplit = splitHelper(split);
		Command::privMsg(fd, server, newsplit);
	}
	else if (split[1][0] != '#')
	{
		User *user2 = server->findUser(split[1]);
		if (user2 == NULL)
		{
			numeric::sendNumeric(ERR_NOSUCHNICK(split[1]), server, user);
			return ;
		}
		std::vector<std::string> newsplit = splitHelper(split);
		Command::privMsg(fd, server, newsplit);
	}
}
//topic #deneme  sadasa
void topicControl(int &fd, Server *server, std::vector<std::string> split)
{
	User *user = server->findUser(fd);


	if (split.size() < 2 || split[1][0] != '#' || (split.size() > 2 && split[2][0] != ':')) {
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(split[0]), server, user);
	}
	Channel *channel = server->getChannel(split[1]);
	if (!channel->isEditorInChannel(user)) {
		numeric::sendNumeric(ERR_CHANOPRIVSNEEDED(channel->getName()), server, user);
	}
	if (split.size() > 2)
		split = splitHelper(split);
	Command::topic(fd, server, split);
}

void quitControl(int &fd, Server *server, std::vector<std::string> split)
{
	if (split.size() == 2 && split[1][0] == ':')
	{
		Command::quit(fd, server, split);
	}
	else
	{
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(split[0]), server, server->findUser(fd));
	}
}

void noticeControl(int &fd, Server *server, std::vector<std::string> split)
{
	if (split.size() < 3 || split[1][0] == '#' || split[2][0] != ':')
		return ;
	bool x = server->findUser(split[1]) == NULL ? false : true;
	if (x) {
		std::vector<std::string> newsplit = splitHelper(split);
		Command::notice(fd, server, newsplit);
	}
}

void kickcontrol(int &fd, Server *server, std::vector<std::string> split)
{
	// command = > KICK #channel user
	if (split[1][0] == '#' && split[2] != "") {
		int size = split.size();
		if (size == 3)
		{
			Command::kick(fd, server, split);
		}
		else if (size > 3)
		{ //Command = > KICK #channel user :message
			if (split[2][0] == ':')
			{
				std::vector<std::string> newsplit = splitHelper(split);
				Command::kick(fd, server, newsplit);
			}
			else
				return ;
		}
	}
	else
	{
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(split[0]), server, server->findUser(fd));
	}
}


void cmdall(int &fd, Server *server, std::string msg)
{
	std::vector<std::string> split = utils::split(msg, " ");
	// for (size_t i = 0;  i < split.size(); i++)
	// {
	// 	std::cout << "--------------------------------" << std::endl;
	// 	if (i == 0)
	// 		std::cout << "User : " << server->findUser(fd)->getNickname() << std::endl;
	// 	std::cout << "Message: " << split[i] << std::endl;
	// 	std::cout << "Client fd -> " << fd << std::endl;
	// 	std::cout << "--------------------------------" << std::endl;
	// }
	std::cout << msg << std::endl;
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
	else if (split[0] == "KICK")
		kickcontrol(fd, server, split);
	else
		numeric::sendNumeric(ERR_UNKNOWNCOMMAND(split[0]), server, server->findUser(fd));
}

bool	checkEnter(std::string msg)
{
	for (size_t i = 0; i < msg.length(); i++) {
		if (msg[i] == '\n')
			return (true);
	}
	return (false);
}

void	Execute::execute(int fd, Server *server, std::string msg)
{
	if (checkEnter(msg)) {
		std::vector<std::string> splCmd = utils::split(msg, "\n");
		for (size_t i = 0; i < splCmd.size(); i++) {
			Execute::execute(fd, server, utils::trimBuffer(splCmd[i]));
		}
	}
	User *user = server->findUser(fd);
	std::string cmd = msg.substr(0, msg.find(' '));

	if (user->isAuth() != true && (cmd != "NICK" && cmd != "USER" && cmd != "PASS" && cmd != "CAP"))
	{
		server->sender(fd, "Error !\nYou didn't verify your identity yet !");
		std::cout << msg << std::endl;
	}
	else {
		if (cmd == "NICK")
			Command::nick(fd, server, msg);
		else if (cmd == "USER")
			Command::user(fd, server, msg);
		else if (cmd == "PASS")
			Command::pass(fd, server, msg);
		else if (cmd == "CAP")
			Command::cap(fd, server, msg);
		else
		{
			cmdall(fd, server, msg);
		}

	}
}
