///////////////////////////////////////
// °³ ¹ß ÀÚ : ±è°æ¼®
// ¼³    ¸í : ¼­¹ö ¼ÒÄÏ °ü·Ã Å¬·¡½º¿¡ ´ëÇÑ ±¸Çö
///////////////////////////////////////

#ifndef __SERVER_SOCKET_H
#define __SERVER_SOCKET_H

#include "TypesDn.h"

#include "Socket.h"
#include "SocketImpl.h"

namespace XNetFrame{

class CServerSocket{
private:
	CSocketImpl* m_pSocketImpl; // ½ÇÁ¦ ¼ÒÄÏ ±¸Çö °´Ã¼

public:
	CServerSocket(void);
	virtual ~CServerSocket(void);

	void                           close(void);           // ¼ÒÄÏÀ» ÆÄ±«
	CSocketImpl::ESocketImplResult create(void);          // ¼ÒÄÏÀ» »ý¼º
	CSocketImpl::ESocketImplResult listen(uint32);        // ¼­¹ö Æ÷Æ®¿¡ ¸®½¼
	CSocketImpl::ESocketImplResult accept(CSocket*);      // Á¢¼ÓÀ» ¹Þ¾ÆµéÀÌ°í Å¬¶óÀÌ¾ðÆ® Á¤º¸¸¦ ¼³Á¤
	CSocketImpl::ESocketImplResult bind(CInetAddress&);   // ¼ÒÄÏÀ» Æ¯Á¤ IP ÁÖ¼Ò ¹× Æ÷Æ®¿¡ ¹ÙÀÎµù
	CSocketImpl::ESocketImplResult bind(string&, uint16); // ¼ÒÄÏÀ» Æ¯Á¤ IP ÁÖ¼Ò ¹× Æ÷Æ®¿¡ ¹ÙÀÎµù
	
	CSocketImpl::ESocketImplResult setReuseAddress(bool);
	CSocketImpl::ESocketImplResult getReuseAddress(bool*);
	CSocketImpl::ESocketImplResult setKeepAlive(uint32, uint32, uint32);
	CSocketImpl::ESocketImplResult setNonBlockingMode(bool);
	
	sint16        getPort(void);     // ¼­¹ö Æ÷Æ® ¹øÈ£¸¦ ¸®ÅÏ
	CInetAddress* getAddress(void);  // ¼­¹ö IP Á¤º¸¸¦ ¸®ÅÏ
	SOCKET        getSocketId(void); // ¼­¹ö ¼ÒÄÏ ID¸¦ ¸®ÅÏ

	uint32        getErrorCode(void);
	TSTRING       getErrorString(uint32);

    #ifdef WIN32
	CSocketImpl::ESocketImplResult createForOverlapped(void);                                   // Overlapped ¼ÒÄÏÀ» »ý¼º
	CSocketImpl::ESocketImplResult acceptExForOverlapped(CSocket*, char*, DWORD*, OVERLAPPED*); // Overlapped ¸ðµå »ó¿¡¼­ Á¢¼ÓÀ» ¼³Á¤
    #endif
};

}

#endif
