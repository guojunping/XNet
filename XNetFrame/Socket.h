///////////////////////////////////////
// °³ ¹ß ÀÚ : ±è°æ¼®
// ¼³    ¸í : ¼ÒÄÏ °ü·Ã Å¬·¡½º¿¡ ´ëÇÑ ±¸Çö
///////////////////////////////////////

#ifndef __SOCKET_H
#define __SOCKET_H

#include "TypesDn.h"

#include "SocketImpl.h"

namespace XNetFrame{

class CSocket{
private:
	CSocketImpl* m_pSocketImpl; // ½ÇÁ¦ ¼ÒÄÏ ±¸Çö °´Ã¼

public:
	CSocket(void);
	virtual ~CSocket(void);

	CSocketImpl* getSocketImpl(void);

	void                           close(void);              // ¼ÒÄÏÀ» ÆÄ±«
	CSocketImpl::ESocketImplResult create(void);             // ¼ÒÄÏÀ» »ý¼º
	CSocketImpl::ESocketImplResult connect(CInetAddress&);   // ¿ÜºÎ È£½ºÆ®¿Í ¿¬°á
	CSocketImpl::ESocketImplResult connect(string&, uint16); // ¿ÜºÎ È£½ºÆ®¿Í ¿¬°á

// ºí·ÏÅ· ¸ðµåÀÏ °æ¿ì ÁÖ¾îÁø Å©±â¸¸Å­ µ¥ÀÌÅÍ°¡ Àü¼ÛµÉ ¶§±îÁö ºí·ÏµÊ
// ³Íºí·ÏÅ· ¸ðµåÀÏ °æ¿ì ÁÖ¾îÁø Å©±âº¸´Ù ÀûÀº µ¥ÀÌÅÍ Àü¼ÛÀÌ µÇ°í ¸®ÅÏÇÒ ¼ö ÀÖÀ½

	CSocketImpl::ESocketImplResult send(char*, uint32&); // ¼ÒÄÏÀ» ÅëÇÑ µ¥ÀÌÅÍ ¼Û½Å
	CSocketImpl::ESocketImplResult recv(char*, uint32&); // ¼ÒÄÏÀ» ÅëÇÑ µ¥ÀÌÅÍ ¼ö½Å

	CSocketImpl::ESocketImplResult setSendBufferSize(sint32);
	CSocketImpl::ESocketImplResult setReceiveBufferSize(sint32);

	CSocketImpl::ESocketImplResult getSendBufferSize(sint32*);
	CSocketImpl::ESocketImplResult getReceiveBufferSize(sint32*);

	CSocketImpl::ESocketImplResult setTcpNodelay(bool);
	CSocketImpl::ESocketImplResult getTcpNodelay(bool*);

	CSocketImpl::ESocketImplResult setTcpKeepAlive(uint32, uint32, uint32);

	CSocketImpl::ESocketImplResult available(void); // µ¥ÀÌÅÍ°¡ µµÂøÇß´ÂÁö È®ÀÎ

	sint16        getPort(void);     // ¿ø°ÝÁö Æ÷Æ® ¹øÈ£¸¦ ¸®ÅÏ
	CInetAddress* getAddress(void);  // ¿ø°ÝÁö IP Á¤º¸¸¦ ¸®ÅÏ
	SOCKET        getSocketId(void); // ¼ÒÄÏ ID¸¦ ¸®ÅÏ
	bool          isConnected(void);

	uint32        getSentByte(void);
	uint32        getReceivedByte(void);

	uint32        getErrorCode(void);
	TSTRING       getErrorString(uint32);

	TSTRING       toString(void); // ¿ø°ÝÁö IP Á¤º¸¸¦ Ãâ·Â

	void          setLocalAddress(sockaddr_in*);
	void          setRemoteAddress(sockaddr_in*);
	
	CSocketImpl::ESocketImplResult setNonBlockingMode(bool);

    #ifdef WIN32
	CSocketImpl::ESocketImplResult createForOverlapped(void); // Overlapped ¼ÒÄÏÀ» »ý¼º

	CSocketImpl::ESocketImplResult getResultForOverlapped(OVERLAPPED*, DWORD*);     // Overlapped ¼ÒÄÏ¿¡ ´ëÇÑ ÃÖÁ¾ I/O »óÅÂ¸¦ È¹µæ
	CSocketImpl::ESocketImplResult recvForOverlapped(WSABUF*, DWORD*, OVERLAPPED*); // Overlapped ¼ÒÄÏ¿¡ ´ëÇÑ ÀÔ·Â ¿¬»ê ¼³Á¤
	CSocketImpl::ESocketImplResult sendForOverlapped(WSABUF*, DWORD*, OVERLAPPED*); // Overlapped ¼ÒÄÏ¿¡ ´ëÇÑ Ãâ·Â ¿¬»ê ¼³Á¤
    #endif
};

}

#endif
