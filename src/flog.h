//--
// FLog class header
//
// Gouverneur Thomas - Cuisinier Gildas
//--

#ifndef _FLOG_H
#define _FLOG_H

#include <fstream>
#include <iostream>

using namespace std;

ostream &DateTime (ostream &);

class FLog
{
	private:
			
		ofstream _file;
		string	_name;
		
	
	public:
		
		FLog(); 
		FLog(string n); 
		~FLog(); 
	
		bool	SetName(string n) { this->_name = n; return true; };

		string	GetName() { return this->_name; };
	
		bool	Close() { _file.close(); return true; };
		bool	Open();
		
		FLog &operator<<(const string &s);
};

#endif /* _FLOG_H */
