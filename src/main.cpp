#include "../inc/Server.hpp"
#include <sstream>

int	checkPort(char *p)
{
	int					port;
	std::stringstream	ss;

	ss << p;
	ss >> port;
	if (!ss.fail()) {
		if (port > 0 && port < 65535) {
			return (port);
		} else {
			perror("Error !\nPort number is not in range !\n <0> - <65535>");
		}
	} else {
		perror("Error !\nPort is not int!\n");
	}
	return (0);
}

int main(int ac, char **av)
{
	if (ac == 3) {
		int port = checkPort(av[1]);
		if (port) {
			Server server(port, av[2]);
			if (server.start())
			{
				server.run();
			}
		}
	}
	else {
		std::cout << "Error !\nWrong number of parameters! <./ircserver> <port> <password>" << std::endl;
	}
	return 0;
}
