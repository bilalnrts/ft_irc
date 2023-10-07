#ifndef USER_HPP
#define USER_HPP

#include "Server.hpp"

typedef std::pair<std::string, bool> Auth;

class User
{
	private :
		int			fd;
		bool		auth;
		Auth		auths[3];
		std::string	username;
		std::string	nickname;
		std::string	hostname;

	public :
		User(int fd);
		int			getUserFd() const;
		bool		isAuth() const;
		Auth		*getAuths();
		void		setAuths(std::string command, bool x);
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getHostname() const;
};

#endif
