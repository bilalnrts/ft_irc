#include "../inc/User.hpp"

User::User(int fd): fd(fd), auth(false)
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

std::string User::getRealname() const {return (this->realname);}

int User::getMode() const {return (this->mode);}

void	User::setNickname(std::string nickname) {this->nickname = nickname;}

void	User::setUsername(std::string username) {this->username = username;}

void	User::setRealname(std::string realname) {this->realname = realname;}

void	User::setHostname(std::string hostname) {this->hostname = hostname;}

void	User::setAuth() {this->auth = true;}

void	User::setMode(int mode) {this->mode = mode;}

std::vector<Channel *>	User::getChannels() {return (this->channels);}

bool	User::getAuths(std::string auth)
{
	for (int i = 0; i < 3; i++) {
		if (auth == this->auths[i].first)
			return (this->auths[i].second);
	}
	return (false);
}

void	User::addChannel(Channel *channel)
{
	this->channels.push_back(channel);
}

void User::removeChannel(Channel *channel)
{
	for (size_t i = 0; i < this->channels.size(); i++) {
		if (this->channels[i] == channel)
			this->channels.erase(this->channels.begin() + i);
	}
}

