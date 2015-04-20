#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>

#include "socket.h"


using namespace boost;
using namespace std;


Socket::Socket() 
: _thread(boost::bind(&Socket::main, this)),
  _detach(false),
  _sock(-1),
  _connected(false),
  _started(false)
{
}


Socket::Socket(std::string host, unsigned short port)
: _thread(boost::bind(&Socket::main, this)),
  _detach(false),
  _sock(false),
  _connected(false),
  m_host(host),
  m_port(port)
{
}

Socket::~Socket()
{
	// wait the thread to join()
}


void Socket::main()
{
	this->sleep(100);
	while(42) {

		// Stop the thread
		if (this->_detach) {
			if (this->_connected) {
				this->Disconnect();
			}
			return;
		}
		if (this->_connected) {

			if (this->_started) {

				if (this->m_oqueue.size()) { /* something to write */
					
					std::string buf = this->getOutput();
					this->write(buf);
				}
				if (this->poll()) { /* something to read */
					std::string buf = read();
					if (buf.size()) {
						this->pushInput(buf);
					}
				}
				this->sleep(100); // sleep 100ms
			} else {
				// not yet started, sleep 1 sec.
				this->sleep(1000);
			}
		} else {
			// not yet connected, sleep 1 sec.
			this->sleep(1000);
		}
	}
	return;
}



void Socket::sleep(int ms)const {

	boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}


bool Socket::Connect() {

	if (this->_connected) {
		return false;
	}
	if (!this->connect()) {
		return false;
	}
	return (this->objOnConnect->*onConnect)();
}


bool Socket::Disconnect() {

	if (!this->_connected) {
		return false;
	}
	return this->disconnect();
}


bool Socket::pushOutput(std::string what)
{
	boost::mutex::scoped_lock lock(mtx_write);
	this->m_oqueue.push(what);
}


bool    Socket::pushInput(std::string what)
{
	boost::mutex::scoped_lock lock(mtx_read);
	this->m_iqueue.push(what);
}


std::string Socket::getInput(void)
{
	boost::mutex::scoped_lock lock(mtx_read);
	if (this->m_iqueue.empty()) {
		// TODO: throw exception
		return std::string(); 
	}
	std::string buf = this->m_iqueue.front();
	this->m_iqueue.pop();
	return buf;
}



std::string Socket::getOutput(void)
{
	boost::mutex::scoped_lock lock(mtx_write);
	if (this->m_oqueue.empty()) {
		// TODO: throw exception
		return std::string(); 
	}
	std::string buf = this->m_oqueue.front();
	this->m_oqueue.pop();
	return buf;
}


bool Socket::connect()
{
	boost::mutex::scoped_lock lock(this->mtx_connect);

        struct hostent *he;
        struct in_addr saddr;
        struct sockaddr_in serv_addr;

        if ((he = gethostbyname(this->m_host.c_str())) == NULL)
               return false;

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(this->m_port);
        serv_addr.sin_addr = *((struct in_addr *)he->h_addr);
        memset(&(serv_addr.sin_zero), '\0', 8);


        if ((this->_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return false;

        if (::connect(this->_sock, (const sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		return false;

        if (fcntl(this->_sock, F_SETFL, O_NONBLOCK))
		return false;

	this->_connected = true;
        return true;
}


bool Socket::disconnect()
{
	boost::mutex::scoped_lock lock(this->mtx_connect);

	this->_connected = false;
	close(this->_sock);
}



std::string Socket::read()
{
        std::string readed;
	char   buffer[MAX_READ_BUF];
	int ret;

        if ((ret = ::read(this->_sock, &buffer, MAX_READ_BUF*sizeof(char))) == -1)
        { 
		return std::string();
        } else if (ret > 0) {  
		buffer[ret] = 0;
		readed = std::string(buffer);
        } else if (ret == 0) {
		this->Disconnect();
                this->Connect();
                return std::string();
	}
        return readed;
}



bool Socket::write(std::string w)
{
	if (::write(this->_sock, w.c_str(), strlen(w.c_str())))
		return true;
	else
		return false;
}


bool Socket::poll()
{
	struct pollfd pfd[1];
	pfd[0].fd = this->_sock;
	pfd[0].events = POLLIN | POLLRDHUP;
	if (::poll(pfd, 1, 100)) {
		if (pfd[0].revents & POLLIN) {
			return true;
		} else if (pfd[0].revents & POLLRDHUP) {
			this->Disconnect();
			this->Connect();
			return false;
		} else {
			return false;
		}
	}
	return false;
}


bool Socket::start()
{
	if (this->_connected) {
		this->_started = true;
		return true;
	}
	return false;
}


bool Socket::stop()
{
	this->_started = false;
	return true;
}



bool Socket::isReadData(void)
{
	boost::mutex::scoped_lock lock(mtx_read);
        if (this->m_iqueue.size())
                return true;
        else
                return false;
}



bool Socket::isWriteData(void)
{
	boost::mutex::scoped_lock lock(mtx_write);
	if (this->m_oqueue.size())
		return true;
	else
		return false;
}
