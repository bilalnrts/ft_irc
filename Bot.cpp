#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int ac, char **av)
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char hello[1024] = "!BOT Hello from Bot!";
	char	buffer[1024] = {0};
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cout << "Socket creation failed !" << std::endl;
		return (-1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(av[1]));

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		std::cout << "Invalid address & Address not supported !" << std::endl;
		return (-1);
	}

	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
		std::cout << "Connection failed !" << std::endl;
		return (-1);
	}
	send(client_fd, hello, strlen(hello), 0);
	std::cout << "Hello msg sent" << std::endl;
	valread = read(client_fd, buffer, 1024);
	std::cout << valread << std::endl;

	close(client_fd);
	return (0);
}
