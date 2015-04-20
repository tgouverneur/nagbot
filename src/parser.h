#ifndef PARSER_H
#define PARSER_H

#include <queue>
#include <string>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/tokenizer.hpp>

#include "message.h"
#include "socket.h"
#include "cmd.h"

typedef std::basic_string<char>     string_t;
typedef string_t::value_type            char_t;

typedef boost::tokenizer <
        boost::char_separator<char_t>,
        string_t::const_iterator,
        string_t
> Tokenizer;

const boost::char_separator<char_t> sep_space(" ");
const boost::char_separator<char_t> sep_excl("!");
const boost::char_separator<char_t> sep_colon(":");

class Nagbot;
class Irc;
class Parser;

class Parser
{
        boost::mutex            mtx_queue;

	Irc			*_irc;
	Nagbot			*_nagbot;
	Socket			*_socket;

	std::queue<std::string>	m_queue;
	volatile bool		_detach;
	volatile bool		_started;

	std::vector<Cmd<Parser> >	_cmds;

	std::string	readQueue(void);
	bool		isData(void);
	void		loop(void);
	void		sleep(int)const;
	bool		parse(std::string);
	bool 		splitHost(Host &);

	boost::thread           _thread;

	/* all irc related handlers */

	bool		handlePing(Message&);
	bool		handleWelcome(Message&);
	bool		handlePrivmsg(Message&);


	public:

	Parser(Irc*, Nagbot*, Socket*);
	~Parser();

	void		putQueue(std::string);

	void		start(void);
	void		stop(void);
        void    	detach() { this->_detach = true; };
	
};
#endif
