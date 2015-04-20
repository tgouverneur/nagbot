#include <iostream>

using namespace std;

#include "nagbot.h"


Nagbot::Nagbot() : _config("./nagbot.conf"), _irc(&_socket, this)
{
}

Nagbot::~Nagbot()
{
}

int Nagbot::main (int, char**)
{
	cout << "[-] Reading configuration file...";
	std::string server = this->_config.read<std::string>( "server" );
	unsigned short port = this->_config.read<unsigned short>( "port" );
	std::string nickname = this->_config.read<std::string>( "nickname" );
	std::string altnick = this->_config.read<std::string>( "altnick" );
	std::string channel = this->_config.read<std::string>( "channel" );
	std::string username = this->_config.read<std::string>( "username" );
	std::string realname = this->_config.read<std::string>( "realname" );
	std::string cmdprefix = this->_config.read<std::string>( "cmdprefix" );
	std::string cmdstatus = this->_config.read<std::string>( "cmdstatus" );
	int answerPv = this->_config.read<int>( "answerpv" );
	cout << "ok" << endl;
	cout << "[-] Initialize the IRC module...";
	this->_irc.setServer(server);
	this->_irc.setPort(port);
	this->_irc.setUsername(username);
	this->_irc.setNickname(nickname);
	this->_irc.setAltnick(altnick);
	this->_irc.setRealname(realname);
	this->_irc.setChannel(channel);
	this->_irc.setCmdPrefix(cmdprefix);
	this->_irc.setCmdStatus(cmdstatus);
	this->_irc.setAnswerPv((answerPv)?true:false);
	cout << "ok" << endl;
	cout << "[-] Establishing connection...";
	if (this->_irc.start()) {
		cout << "ok" << endl;
	} else {
		cout << "failed" << endl;
		return 42;
	}
	cout << "[-] Launching endless loop..." << endl;
	while (42) {
		this->_irc.evolve();
	}
}



