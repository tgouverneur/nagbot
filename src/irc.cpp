#include <iostream>
#include "irc.h"

using namespace std;

Irc::Irc(Socket *s, Nagbot *nb) : _socket(s), _nb(nb), _started(false), _parser(this, nb, s)
{
	this->_socket->setOnConnect(&Irc::connectCallback, this);

}


Irc::~Irc()
{

}


bool    Irc::start()
{
	_socket->setHost(this->m_server);
	_socket->setPort(this->m_port);
	if (!_socket->Connect()) {
		return false;
	}
	_socket->start();
	_parser.start();
	
	std::string cmdstatus = this->m_cmdprefix;
	cmdstatus.append(this->m_cmdstatus);
	this->_cmds.push_back(Cmd<Irc>(cmdstatus, &Irc::cmdStatus));

	return true;
}


bool    Irc::stop()
{
	_socket->stop();
	_socket->Disconnect();
}

void	Irc::evolve()
{
	static std::string remain;

	if (_socket->isConnected() && _socket->isStarted()) {
		
		if (_socket->isReadData()) {
			std::string buffer = _socket->getInput();
			bool incomplete = false;
			if (buffer[buffer.size() - 1] != '\n' &&
			    buffer[buffer.size() - 1] != '\r') {
				  incomplete = true;
			} // the last line will be incomplete

			std::queue<std::string> q = this->split(buffer);
			std::string line;
			bool firstline = true;
			while(!q.empty()) {
				line = q.front();
				q.pop();
				if (firstline) {
					firstline = false;
					if (remain.size()) {
						remain.append(line);
						line = remain;
						remain = std::string();
					}
				}
				if (q.empty() && incomplete) {
					remain = line;
				} else {
					this->_parser.putQueue(line);
				}
			}
		}	
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}
	boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
}

std::queue<std::string> Irc::split(std::string buffer)
{
	istringstream		is(buffer);
	std::queue<std::string>	q;

	std::string line;
	while(getline(is, line, '\r')) {

		q.push(line);
		char c;
		if (!(c = is.get()) == '\n') {
			is.putback(c);
		}
	}
	return q;
}

bool Irc::sendPrivmsg(std::string to, std::string w)
{
	std::string msg("PRIVMSG ");
	msg.append(to);
	msg.append(std::string(" :"));
	msg.append(w);
	this->sendRaw(msg);
}

bool Irc::sendPong(std::string srv)
{
	std::string msg("PONG ");
	msg.append(srv);
	this->sendRaw(msg);
}

bool Irc::sendRaw(std::string raw)
{
	raw.append(std::string("\r\n"));
	this->_socket->pushOutput(raw);
}

bool Irc::registerNick()
{
	std::string msg("NICK ");
	msg.append(this->m_nickname);
	this->sendRaw(msg);
}

bool Irc::registerUser()
{
	std::string msg("USER ");
	msg.append(this->m_username);
	msg.append(" ");
	msg.append(this->m_username);
	msg.append(" ");
	msg.append(this->m_server);
	msg.append(" ");
	msg.append(this->m_realname);

	this->sendRaw(msg);
}

bool Irc::connectCallback(void)
{
	this->registerNick();
	this->registerUser();
	return true;
}

bool Irc::doJoin()
{
	std::string msg("JOIN ");
	msg.append(this->m_channel);
	this->sendRaw(msg);
}

bool    Irc::parseMsg(Message &m)
{
	if (!m.args.size()) {
		return true;
	}

	std::string cmd;
	std::string args;

	if (m.args.find(" ") != string::npos) {
		cmd = m.args.substr(0,m.args.find_first_of(" "));
		args = m.args.substr(m.args.find_first_of(" "), 
				     m.args.size() - m.args.find_first_of(" "));
		m.args = args;
		m.cmd = cmd;
	} else
	{
		cmd = m.args;
		m.cmd = cmd;
		m.args = std::string();
	}

	if (cmd[0] != this->m_cmdprefix[0]) {

		return true;
	}
	for (int i=0; i < this->_cmds.size(); i++) {
		if (m.cmd == this->_cmds[i].token) {
			//runit!
			(this->*(this->_cmds[i].f))(m);
		}
	}
	return true;
}

bool	Irc::cmdStatus(Message &m)
{
	std::string reply("c'est nin co fe ca hein m'fi");
	if (m.to.whole[0] == '#') {

		this->sendPrivmsg(m.to.whole, reply);
	} else if (m.to.whole == this->m_nickname && this->m_answerpv) {

		this->sendPrivmsg(m.from.nick, reply);
	}
}
