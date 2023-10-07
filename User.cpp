#include "User.hpp"

User::User(int fd): fd(fd), auth(false)
{
	this->auths[0] = Auth("NICK", false);
	this->auths[1] = Auth("PASS", false);
	this->auths[2] = Auth("USER", false);
}

int User::getUserFd() const {return (this->fd);}

bool	User::isAuth() const {return (this->auth);}

Auth	*User::getAuths() {return (this->auths);}

void	User::setAuths(std::string command, bool x)
{
	for (int i = 0; i < 3; i++) {
		if (this->auths[i].first == command)
			this->auths[i].second = x;
	}
}

std::string User::getNickname() const {return (this->nickname);}
std::string User::getUsername() const {return (this->username);}
std::string User::getHostname() const {return (this->hostname);}
