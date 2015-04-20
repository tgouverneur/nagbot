#ifndef NAGBOT_H
#define NAGBOT_H

#include <string>

#include "ConfigFile.h"
#include "socket.h"
#include "irc.h"

class Nagbot
{
	ConfigFile	_config;
	Socket		_socket;
	Irc		_irc;
	
	public:
		Nagbot();
		~Nagbot();

		int main (int, char**);
};

#endif
