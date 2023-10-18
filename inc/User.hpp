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
		Auth					auths[3];
		std::string				username;
		std::string				realname;
		std::string				nickname;
		std::string				hostname;
		int						mode;
		std::vector<Channel *>	channels;
		bool					cap;

	public :
		bool					auth; //public for now
		User(int fd);
		int			getUserFd() const;
		bool		isAuth() const;
		Auth		*getAuths();
		bool		getAuths(std::string auth);
		void		setAuths(std::string command, bool x);
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getHostname() const;
		std::string	getRealname() const;
		int			getMode() const;
		bool		getCap();
		std::vector<Channel *>	getChannels();
		void		setNickname(std::string nickname);
		void		setRealname(std::string realname);
		void		setUsername(std::string username);
		void		setHostname(std::string hostname);
		void		setAuth();
		void		setCap(bool x);
		void		setMode(int mode);
		void 		addChannel(Channel *channel); // add a user to the channel
		void		removeChannel(Channel *channel); // remove a user from the channel
		void		getChannelName(Channel *channel); // get a channel from the user
};

#endif
