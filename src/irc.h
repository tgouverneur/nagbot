#ifndef IRC_H
#define IRC_H

#include <string>
#include <iostream>
#include <boost/tokenizer.hpp>

#include "socket.h"
#include "parser.h"
#include "cmd.h"

class Nagbot;

class Irc
{
		Socket		*_socket;
		Nagbot 		*_nb;
		Parser		_parser;
		std::string	m_server;
		std::string	m_username;
		std::string	m_nickname;
		std::string	m_altnick;
		std::string	m_realname;
		std::string	m_channel;
		unsigned short	m_port;

		std::string	m_cmdprefix;
		std::string	m_cmdstatus;
		bool		m_answerpv;
	
		bool		_started;

		std::queue<std::string> split(std::string);
		std::vector<Cmd<Irc> >	_cmds;

		/* answers to msgs */
		bool		cmdStatus(Message &);
		
	public:

		Irc(Socket*, Nagbot*);
		~Irc();

		/* get/set */
		std::string getServer(void)const { return this->m_server; };
		std::string getUsername(void)const { return this->m_username; };
		std::string getNickname(void)const { return this->m_nickname; };
		std::string getAltnick(void)const { return this->m_altnick; };
		std::string getRealname(void)const { return this->m_realname; };
		std::string getChannel(void)const { return this->m_channel; };
		unsigned short getPort(void)const { return this->m_port; };

		bool	setServer(std::string w) { m_server = (_socket->isConnected())?m_server:w; return (_socket->isConnected())?false:true; };
		bool	setUsername(std::string w) { m_username = (_socket->isConnected())?m_username:w; return (_socket->isConnected())?false:true; };
		bool	setNickname(std::string w) { m_nickname = (_socket->isConnected())?m_nickname:w; return (_socket->isConnected())?false:true; };
		bool	setAltnick(std::string w) { m_altnick = (_socket->isConnected())?m_altnick:w; return (_socket->isConnected())?false:true; };
		bool	setRealname(std::string w) { m_realname = (_socket->isConnected())?m_realname:w; return (_socket->isConnected())?false:true; };
		bool	setChannel(std::string w) { m_channel = (_socket->isConnected())?m_channel:w; return (_socket->isConnected())?false:true; };
		bool	setPort(unsigned short w) { m_port = (_socket->isConnected())?m_port:w; return (_socket->isConnected())?false:true; };

		void	setCmdPrefix(std::string w) { m_cmdprefix = w; };
		void	setCmdStatus(std::string w) { m_cmdstatus = w; };
		void	setAnswerPv(bool w) { m_answerpv = w; };

		bool	start();
		bool	stop();

		void	evolve();

		/* IRC things */
		bool	sendRaw(std::string);
		bool	sendPong(std::string);
		bool	sendPrivmsg(std::string, std::string);
		bool	registerNick();
		bool	registerUser();
		bool	doJoin();
		bool	parseMsg(Message&);

		bool	connectCallback(void);
};
#endif
