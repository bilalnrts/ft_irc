#ifndef USER_HPP
#define USER_HPP

#include "Server.hpp"
#include "Execute.hpp"
#include "Channel.hpp"

typedef std::pair<std::string, bool> Auth;

class User
{
	private :
		int						fd;
		bool					auth;
		Auth					auths[3];
		std::string				username;
		std::string				realname;
		std::string				nickname;
		std::string				hostname;
		std::vector<Channel *>	channels;

	public :
		User(int fd);
		int			getUserFd() const;
		bool		isAuth() const;
		Auth		*getAuths();
		bool		getAuths(std::string auth);
		void		setAuths(std::string command, bool x);
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getHostname() const;
		std::vector<Channel *>	getChannels();
		void		setNickname(std::string nickname);
		void		setRealname(std::string realname);
		void		setUsername(std::string username);
};

#endif
