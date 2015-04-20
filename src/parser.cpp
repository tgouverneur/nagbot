#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "parser.h"
#include "irc.h"
#include "include/ircraw.h"

using namespace std;

Parser::Parser(Irc *i, Nagbot *n, Socket *s) : _irc(i), _nagbot(n), _socket(s), _detach(false), _started(false), _thread(boost::bind(&Parser::loop, this))
{
	this->_cmds.push_back(Cmd<Parser>(std::string("PING"), &Parser::handlePing));
	this->_cmds.push_back(Cmd<Parser>(std::string(RPL_WELCOME), &Parser::handleWelcome));
	this->_cmds.push_back(Cmd<Parser>(std::string("PRIVMSG"), &Parser::handlePrivmsg));
}

Parser::~Parser()
{

}

void Parser::putQueue(std::string line)
{
	if (line.size()) {
		boost::mutex::scoped_lock lock(mtx_queue);
		m_queue.push(line);
	}
}

std::string Parser::readQueue(void)
{
	boost::mutex::scoped_lock lock(mtx_queue);
	if (!this->m_queue.empty()) {
		std::string ret = this->m_queue.front();
		this->m_queue.pop();
		return ret;
	}
	return std::string();
}

bool	Parser::isData(void)
{
	boost::mutex::scoped_lock lock(mtx_queue);
	if (this->m_queue.empty())
		return false;
	else
		return true;
}

void Parser::start() {
	
	this->_started = true;
}

void Parser::stop() {
	this->_started = false;
}

void Parser::loop(void)
{
	this->sleep(100);
	while(42) {
		if (this->_detach) {
			return;
		}
		if (this->_started) {
			if (this->isData()) { // something to parse
				std::string line = this->readQueue();
				this->parse(line);
			}
			this->sleep(100);
		} else {
	//		this->sleep(1000);
		}
	}
}

void Parser::sleep(int ms)const {

        boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}

bool Parser::parse(std::string raw)
{
	Message msg;
	msg.line = raw;

        if (raw[0] == ':')
        {
                raw = raw.substr(1,raw.size());
		msg.srv = true;
        } else {
		msg.srv = false;
	}

	std::vector<std::string> spaced;
        Tokenizer spaceTokenizer(raw, sep_space); // separe with ' '
        for (Tokenizer::const_iterator it = spaceTokenizer.begin(); it != spaceTokenizer.end(); it++)
                spaced.push_back(*it);

	if (spaced.size() == 2) {
		msg.cmd = spaced[0];
		msg.args = spaced[1];
	} else if (spaced.size() > 2) {
		msg.from.whole = spaced[0];
		msg.cmd = spaced[1];
		spaced.erase(spaced.begin());
		spaced.erase(spaced.begin());
		for (int i=0; i<spaced.size(); i++) {
			msg.args.append(spaced[i]);
			if (i<spaced.size()-1) {
				msg.args.push_back(' ');
			}
		}
		if (msg.from.whole.find("!") != string::npos) {
			this->splitHost(msg.from);
		}
	} else {
		return false; // bad raw
	}

	for (int i=0; i < this->_cmds.size(); i++) {
	
		if (!this->_cmds[i].token.compare(msg.cmd)) {
			// command found, run it!
			cout << "command found, execute it" << endl;
			(this->*(this->_cmds[i].f))(msg);
		}
	}
}

bool Parser::splitHost(Host &h) {

	size_t exclpos = h.whole.find_first_of("!");
	size_t atpos = h.whole.find_first_of("@");

	h.nick = h.whole.substr(0, exclpos);
	h.user = h.whole.substr(exclpos, atpos - exclpos);
	h.host = h.whole.substr(atpos, h.whole.size() - atpos);

	return true;
}

bool Parser::handlePrivmsg(Message &m)
{
	m.to.whole = m.args.substr(0, m.args.find_first_of(" "));
	if (m.to.whole.find("!@") != string::npos) {
		this->splitHost(m.to);
	}
	m.args = m.args.substr(m.args.find_first_of(" ") + 2, 
			       m.args.size() - m.args.find_first_of(" ") - 2 );

	return this->_irc->parseMsg(m);
}


bool Parser::handleWelcome(Message &m)
{
	cout << "welcome message received, doing the /join" << endl;
	return this->_irc->doJoin();
}

bool Parser::handlePing(Message &m)
{
	std::string srv = m.args;
	if (srv[0] == ':')
        {
                srv = srv.substr(1,srv.size());
        } 
	return this->_irc->sendPong(srv);
}
