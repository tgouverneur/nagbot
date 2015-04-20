#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

struct Host
{
	std::string	whole; // nick!user@host
	std::string	nick;
	std::string	user;
	std::string	host;
};

struct Message
{
	std::string	line;	// the whole line
	std::string	cmd;	// command token (PRIVMSG, MODE, ...)
	Host		from;
	Host		to;
	bool		srv;	// is a server message
	std::string	args;	// argument, after the token
};

#endif
