/**
 * netaddress
 * 
 */
#ifndef __INET_ADDRESS_H
#define __INET_ADDRESS_H

#include "TypesDn.h"

namespace XNetFrame{

class CInetAddress{
private:
	string      m_sHost;  // 
	bool        m_bValid; // 

public:
	sockaddr_in m_saSocketAddress;

private:
	void init(void);
	
public:
	CInetAddress(void);
	CInetAddress(CInetAddress&);
	CInetAddress(string&, uint16); // 
	virtual ~CInetAddress(void);

	void   setPort(uint16);    // 
	bool   setByName(string&); // 

	void   setSocketAddress(sockaddr_in*);

	uint16 getPort(void);        // 
	string getHostName(void);    // 
	string getHostAddress(void); // 

	CInetAddress& operator=(const CInetAddress&);                     // 
	friend bool operator==(const CInetAddress&, const CInetAddress&); // 

	bool    isValid(void); // 
	string  toString(void);
};

}

#endif
