//--
// FLog class Definition
//
// Gouverneur Thomas - Cuisinier Gildas
//--

#include "flog.h"

ostream &DateTime ( ostream &os )
{
	time_t now;
	char *date;
	
	now = time(0);
	date = ctime(&now);
	date[24] = 0;
	return ( os << "[" << date << "] ");
}

FLog::FLog()
{
}

FLog::FLog(string n) : _name(n), _file(_name.c_str(), ios::out | ios::app)
{ 
	_file.close();
	_file.open(_name.c_str(), ios::out | ios::app | ios::trunc); 
}

bool	FLog::Open()
{
	_file.open(_name.c_str(), ios::out | ios::app);
	_file.close();
	_file.open(_name.c_str(), ios::out | ios::trunc);
}


FLog::~FLog()
{
}


FLog &FLog::operator<<(const string &s)
{
	_file << DateTime << s << endl;

	return *this;
}
