#include "../inc/User.hpp"

User::User(int fd): fd(fd), auth(true) //auth will be false later
{
	this->auths[0] = Auth("NICK", false);
	this->auths[1] = Auth("PASS", false);
	this->auths[2] = Auth("USER", false);
	this->username = "";
	this->nickname = "";
	this->realname = "";
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

void	User::setNickname(std::string nickname) {this->nickname = nickname;}

void	User::setUsername(std::string username) {this->username = username;}

void	User::setRealname(std::string realname) {this->realname = realname;}

std::vector<Channel *>	User::getChannels() {return (this->channels);}

bool	User::getAuths(std::string auth)
{
	for (int i = 0; i < 3; i++) {
		if (auth == this->auths[i].first)
			return (this->auths[i].second);
	}
	return (false);
}
