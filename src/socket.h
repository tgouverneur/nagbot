#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <queue>
#include <exception>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

#define	MAX_READ_BUF	1024

class Irc;

class Socket
{
  private:

	boost::mutex		mtx_read;
	boost::mutex		mtx_write;
	boost::mutex		mtx_connect;

	int 			_sock;
	volatile bool		_detach;
	volatile bool		_connected;
	volatile bool		_started;

	std::string		m_host;
	unsigned short		m_port;
	std::queue<std::string>	m_iqueue;
	std::queue<std::string>	m_oqueue;

	void	sleep(int)const;
	void	main();

	/* queues private operations */
	bool		pushInput(std::string);
	std::string	getOutput(void);

	unsigned int	bytesInput;
	unsigned int	bytesOutput;

	/* network operations */
	bool		connect();
	bool		disconnect();
	std::string	read();
	bool		write(std::string);
	bool		poll();

	boost::thread		_thread;

	bool			(Irc::*onConnect)(void);
	Irc			*objOnConnect;

  public:

	Socket();
	Socket(std::string, unsigned short);
	~Socket();

	/* set/get */
	std::string	getHost()const { return this->m_host; };
	unsigned short	getPort()const { return this->m_port; };
	void		setHost(std::string w) { this->m_host = w; };
	void		setPort(unsigned short w) { this->m_port = w; };
	unsigned int	getIBytes()const { return this->bytesInput; };
	unsigned int	getOBytes()const { return this->bytesOutput; };

	/* queues public operations */
	std::string	getInput();
	bool		pushOutput(std::string);

	/* Network abstract */
	bool	Disconnect();
	bool	Connect();

	void	setOnConnect(bool(Irc::*mthd)(void), Irc*i) { onConnect = mthd; objOnConnect = i; };

	bool	isConnected(void)const { return this->_connected; };
	bool	isStarted(void)const { return this->_started; };
	bool	isReadData(void);
	bool	isWriteData(void);

	bool		start();
	bool		stop();
	void		detach() { this->_detach = true; };
};

#endif
