#include "Server.hpp"

const int PORT = 6667;
const int MAX_CLIENTS = 10;
const int MAX_BUFFER_SIZE = 1024;

int main()
{
	Server server(PORT, MAX_CLIENTS, MAX_BUFFER_SIZE);
	if (server.start())
	{
		server.run();
	}
	return 0;
}
