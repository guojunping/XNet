/**
 * net address
 * 
 */

#include "InetAddress.h"
#include "Exception.h"
#include <string.h>
#include <netdb.h>
#include <stdio.h>

namespace XNetFrame{

CInetAddress::CInetAddress(void){
	init();

	m_saSocketAddress.sin_port = 0;
	memset(&m_saSocketAddress.sin_addr, 0, sizeof(in_addr));
}

CInetAddress::CInetAddress(CInetAddress& iaInetAddress){
	init();

	m_sHost  = iaInetAddress.m_sHost;
	m_bValid = iaInetAddress.m_bValid;
	memcpy(&m_saSocketAddress, &iaInetAddress.m_saSocketAddress, sizeof(sockaddr_in));
}

CInetAddress::CInetAddress(string& sHost, uint16 uiPort){ // 
	init();

	setPort(uiPort);
	setByName(sHost);
}

CInetAddress::~CInetAddress(void){
}

void CInetAddress::init(void){
	m_sHost.reserve(128); // 

	m_bValid = false;

	m_saSocketAddress.sin_family = AF_INET;
	memset(m_saSocketAddress.sin_zero, 0, 8); // 
}

void CInetAddress::setPort(uint16 uiPort){ // 
	m_saSocketAddress.sin_port = ::htons(uiPort); // 
}

bool CInetAddress::setByName(string& sHost){ // 
	hostent* pHostEnt;
	in_addr  iaInAddr;

	iaInAddr.s_addr = ::inet_addr(sHost.c_str()); // 
	if(iaInAddr.s_addr == INADDR_NONE){
		pHostEnt = ::gethostbyname(sHost.c_str()); // 
		if(pHostEnt == NULL){
			m_bValid = false;
			throw new CUnknownHostException("InetAddress::setByName");
		};

		m_sHost = string(pHostEnt->h_name);
		memcpy(&m_saSocketAddress.sin_addr, &pHostEnt->h_addr_list[0], sizeof(in_addr));
	}
	else{
		m_sHost = sHost;
		memcpy(&m_saSocketAddress.sin_addr, &iaInAddr, sizeof(in_addr));
	};

	m_bValid = true;

	return true;
}

void CInetAddress::setSocketAddress(sockaddr_in* pSocketAddress){
	memcpy(&m_saSocketAddress, pSocketAddress, sizeof(sockaddr_in));

	m_bValid = true;
}

CInetAddress& CInetAddress::operator=(const CInetAddress& iaInetAddress){ //
	m_sHost  = iaInetAddress.m_sHost;
	m_bValid = iaInetAddress.m_bValid;

	memcpy(&m_saSocketAddress, &iaInetAddress.m_saSocketAddress, sizeof(sockaddr_in));

	return *this;
}

bool operator==(const CInetAddress& iaInetAddress1, const CInetAddress& iaInetAddress2){ // 
	bool bResult;

	bResult = (memcmp(&iaInetAddress1.m_saSocketAddress, &iaInetAddress2.m_saSocketAddress, sizeof(sockaddr_in) - 8) == 0)? true : false; // 
	return bResult;
}

string CInetAddress::getHostAddress(void){ // 
	return string(::inet_ntoa(m_saSocketAddress.sin_addr));
}

string CInetAddress::getHostName(void){ // 
	return m_sHost;
}

uint16 CInetAddress::getPort(void){ // 
	return ::ntohs(m_saSocketAddress.sin_port); // 
}

bool CInetAddress::isValid(void){ // 
	return (m_bValid == true && m_saSocketAddress.sin_port != 0);
}

string CInetAddress::toString(void){
	char   szString[64];
	string  sString;
	
	sprintf(szString, "%s:%d", getHostAddress().c_str(), getPort());
	sString = szString;

	return sString;
}

}
