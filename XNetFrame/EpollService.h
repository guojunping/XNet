///////////////////////////////////////
// °³ ¹ß ÀÚ : ±è°æŒ®
// Œ³    ží : EPOLL Œ­ºñœº¿¡ ŽëÇÑ ±žÇö
///////////////////////////////////////

/*
0. ÇÁ·ÎÆÛÆŒ Á€ºžžŠ È¹µæ
1. Á¢ŒÓ °üž®¿ë EPOLL °ŽÃŒžŠ »ýŒº
2. ºñµ¿±â I/O¿ë EPOLL °ŽÃŒžŠ »ýŒº
3. Œ­¹ö ŒÒÄÏ °ŽÃŒžŠ ¿À¹ö·Šµå žðµå·Î »ýŒº
4. Œ­¹ö ŒÒÄÏÀ» Á¢ŒÓ °üž®¿ë EPOLL °ŽÃŒ¿¡ ¹ÙÀÎµù
5. µðœºÆÐÄ¡ Œ­ºñœº °ŽÃŒžŠ ŽÙŒö°³ »ýŒº
6. ŒŒŒÇ °ŽÃŒžŠ ŽÙŒö°³ »ýŒºÇÏ°í ŒŒŒÇ ³»¿¡ ŒÒÄÏ, ÈžÀüÇü ¹öÆÛžŠ »ýŒº ¹× ÃÊ±âÈ­ÇÑ ÈÄ Á¢ŒÓ¿¡ ŽëÇÑ Æ®ž®°ÅžŠ Œ³Á€ÇÏ°í ŒŒŒÇ ÆÑÅäž®¿¡ Ãß°¡
7. µðœºÆÐÄ¡ Œ­ºñœº °ŽÃŒžŠ œÇÇà
8. ºñµ¿±â I/O¿ë Ÿ²·¹µåžŠ ŽÙŒö°³ »ýŒºÇÏ°í œÇÇà
9. Á¢ŒÓ °üž®¿ë Ÿ²·¹µåžŠ 1°³ »ýŒºÇÏ°í œÇÇà

8.1. Á¢ŒÓ °üž®¿ë EPOLL °ŽÃŒ·ÎºÎÅÍ ÄÜÆ®·ÑÀÌ ÀüÀÌ
8.2. ÀüÀÌµÈ ¿À¹ö·Šµå ±žÁ¶ÃŒ¿¡Œ­ ŒŒŒÇ °ŽÃŒžŠ È¹µæ
8.3. ŒŒŒÇ °ŽÃŒžŠ ºñµ¿±â I/O¿ë EPOLL °ŽÃŒ¿¡ ¹ÙÀÎµù
8.4. ºñµ¿±â ÀÔ·Â¿¡ ŽëÇÑ Æ®ž®°ÅžŠ Œ³Á€

9.1. ºñµ¿±â I/O¿ë EPOLL °ŽÃŒ·ÎºÎÅÍ ÄÜÆ®·ÑÀÌ ÀüÀÌ
9.2. ÀüÀÌµÈ ŒŒŒÇ °ŽÃŒ¿¡ ŽëÇÏ¿© ÃÖÁŸ °á°úžŠ È¹µæ
9.3. ÀÔ·Â žðµåÀÎ °æ¿ì ÈžÀüÇü ŒöœÅ ¹öÆÛÀÇ Å×ÀÏ Æ÷ÀÎÅÍžŠ ÀüÁøœÃÅ°°í ž¶Å© Æ÷ÀÎÅÍžŠ ÀüÁøœÃÅ°°í ÆÐÅ¶À» Ã³ž®ÇÑ ÈÄ ºñµ¿±â ÀÔ·Â¿¡ ŽëÇÑ Æ®ž®°ÅžŠ Œ³Á€
9.4. Ãâ·Â žðµåÀÎ °æ¿ì ŒÛœÅ ¹öÆÛÀÇ Çìµå Æ÷ÀÎÅÍžŠ ÀüÁøœÃÅ°°í ³²Àº ºñµ¿±â Ãâ·Â¿¡ ŽëÇÑ Æ®ž®°ÅžŠ Œ³Á€

?. Ãâ·ÂœÃ ÈžÀüÇü ŒÛœÅ ¹öÆÛÀÇ Å×ÀÏ Æ÷ÀÎÅÍžŠ ÀüÁøœÃÅ°°í ºñµ¿±â Ãâ·Â¿¡ ŽëÇÑ Æ®ž®°ÅžŠ Œ³Á€
*/

#ifndef __EPOLL_SERVICE_H
#define __EPOLL_SERVICE_H

#include "TypesDn.h"

#include "Log.h"
#include "System.h"
#include "Session.h"
#include "EpollImpl.h"
#include "Properties.h"
#include "SocketImpl.h"
#include "InetAddress.h"
#include "ServerSocket.h"
#include "SessionFactory.h"
#include "RotativeBuffer.h"
#include "DispatchService.h"
#include "EpollAcceptThread.h"
#include "EpollAsyncIoThread.h"
#include "Monitor.h"

namespace XNetFrame{

#define MAX_NUM_EPOLL   (32)

#define KEY_CLOSE_EPOLL (0x00000003)

typedef hash_map<uint32, CDispatchService*> DispatchServiceMap;


#define SESSION_CLASS          (_T("Session"))
#define EPOLL_SERVICE_CLASS    (_T("EpollService"))
#define ROTATIVE_BUFFER_CLASS  (_T("RotativeBuffer"))
#define DISPATCH_SERVICE_CLASS (_T("DispatchService"))

#define PORT_PROPERTY          (_T("Port"))
#define HOST_PROPERTY          (_T("Host"))
#define MAX_SESSION_PROPERTY   (_T("MaxSession"))
#define NUM_DISPATCH_PROPERTY  (_T("NumDispatch"))
#define BUFFER_LEVEL_PROPERTY  (_T("BufferLevel"))
#define KEEP_ALIVE			   (_T("KeepAlive"))
#define KEEP_TIME			   (_T("KeepTime"))
#define KEEP_INTERVAL		   (_T("KeepInterval"))

class CEpollService : public CSessionFactory{
public:
	enum EEpollServiceResult { ISR_NULL, ISR_FAILURE, ISR_SESSION_CREATE_FAILURE, ISR_SESSION_INIT_FAILURE, ISR_SESSION_ADD_FAILURE,
		ISR_ALREADY_SERVICE_RUNNING, ISR_ALREADY_SERVICE_STOPPED, ISR_THREAD_START_FAILURE, ISR_INVALID_PROPERTY, ISR_OK };

   CEpoll*            m_pAsyncIoEpoll;                // Á¢ŒÓ °üž®¿ë EPOLL °ŽÃŒ
protected:	
	CLog*           m_pLog;            // ·Î±× œÃœºÅÛ °ŽÃŒ
private:
	CProperties*   m_pProperties;     // ÇÁ·ÎÆÛÆŒ œÃœºÅÛ °ŽÃŒ
	CServerSocket* m_pServerSocket;   // Œ­¹ö ŒÒÄÏ °ŽÃŒ
	SOCKET          m_sServerSocketId; // Œ­¹ö ŒÒÄÏ ID

	CMonitor		m_cMonitor;
	DWORD		    m_dwSendBytes;
	DWORD		    m_dwRecvBytes;

	string         m_sHost;           // Œ­¹ö È£œºÆ® ÀÌž§
	uint32         m_uiPort;          // Œ­¹ö Æ÷Æ® ¹øÈ£

	uint32         m_uiErrorCode;     // ¿¡·¯ ÄÚµå ¹øÈ£
	uint32         m_uiMaxSession;    // ÃÖŽë ŒŒŒÇ °ŽÃŒŒö
	uint32         m_uiNumDispatch;   // µðœºÆÐÄ¡ ¿¬»êÀ» À§ÇÑ Ÿ²·¹µå °¹Œö
	uint32         m_uiDispatchIndex; // ¶ó¿îµå ·Îºó ¹æœÄÀÇ µðœºÆÐÄ¡ Œ­ºñœº ÀÎµŠœº
	uint32			m_uiKeepAlive;
	uint32			m_uiKeepTime;
	uint32			m_uiKeepInterval;

	sint32         m_siRefCount;      // °ŽÃŒÀÇ ÂüÁ¶ Ä«¿îÆ®(Œ­ºñœº œÃÀÛœÃ Áõ°¡, Œ­ºñœº ÁßÁöœÃ °šŒÒ)
	TSTRING        m_sServerName;     // Œ­¹ö ÀÌž§
	bool           m_bServiceRunning; // Œ­ºñœº È°ŒºÈ­ ¿©ºÎ

	TSTRING		   m_sEpollServiceClass;
	TSTRING		   m_sMaxSessionProperty;
	TSTRING		   m_sNumDispatchProperty;
	TSTRING		   m_sRotativeBufferLevelProperty;

	CRotativeBuffer::LBufferLevel m_emBufferLevel; // ÈžÀüÇü ¹öÆÛÀÇ ·¹º§

	DispatchServiceMap    m_mDispatchServices;       // µðœºÆÐÄ¡ Œ­ºñœº žÊ
	
	CEpollAsyncIoThread*  m_pEpollAsyncIoThread; // ºñµ¿±â I/O¿ë Ÿ²·¹µå ž®œºÆ®

	EEpollServiceResult m_isrResult;

private:
	uint32 getThreadCount(void); // EPOLL Œ­ºñœº¿¡ ÀûÀýÇÑ Ÿ²·¹µå °¹ŒöžŠ ž®ÅÏ

	EEpollServiceResult createEpoll(void);            // EPOLL °ŽÃŒžŠ »ýŒº
	EEpollServiceResult createSocket(void);          // Œ­¹ö ŒÒÄÏÀ» »ýŒº
	EEpollServiceResult createSession(void);         // ŒŒŒÇ °ŽÃŒžŠ ÁöÁ€µÈ °¹ŒöžžÅ­ »ýŒº
	EEpollServiceResult createService(void);         // EPOLL Œ­ºñœºžŠ »ýŒº
	EEpollServiceResult destroyService(void);        // EPOLL Œ­ºñœºžŠ ÆÄ±«
	EEpollServiceResult createDispatchService(void); // µðœºÆÐÄ¡ Œ­ºñœº °ŽÃŒžŠ ÁöÁ€µÈ °¹ŒöžžÅ­ »ýŒº

public:
	CEpollService(CLog*);
	~CEpollService(void);

	uint32             getPort(void);              // Æ÷Æ® ¹øÈ£žŠ È¹µæ
	string             getIpAddress(void);         // IP ÁÖŒÒžŠ È¹µæ
	uint32             getErrorCode(void);
	CProperties*       getProperties(void);        // ÇÁ·ÎÆÛÆŒ œÃœºÅÛÀ» È¹µæ
	TSTRING            getServerName(void);        // Œ­¹ö ÀÌž§À» È¹µæ
	uint32             getMaxSession(void);        // ÃÖŽë µ¿œÃ Á¢ŒÓ ŒöžŠ È¹µæ
	uint32             getNumDispatch(void);       // µðœºÆÐÄ¡ ¿¬»êÀ» À§ÇÑ Ÿ²·¹µå °¹ŒöžŠ È¹µæ
	void               setServerName(LPTSTR);      // Œ­¹ö ÀÌž§À» Œ³Á€
	bool               isServiceRunning(void);     // Œ­ºñœº ÁøÇà ¿©ºÎžŠ ž®ÅÏ
	CDispatchService*  getDispatchService(void);   // ¶ó¿îµå ·Îºó ¹æœÄÀž·Î µðœºÆÐÄ¡ Œ­ºñœº °ŽÃŒÀÇ Æ÷ÀÎÅÍžŠ ž®ÅÏ
	CDispatchService*  getDispatchService(uint32); // µðœºÆÐÄ¡ ÀÎµŠœº¿¡ ºÎÇÕÇÏŽÂ µðœºÆÐÄ¡ Œ­ºñœº °ŽÃŒÀÇ Æ÷ÀÎÅÍžŠ ž®ÅÏ
	CServerSocket*     getServerSocket();
	
	bool   getKeepAlive(void);
	uint32 getKeepTime(void);
	uint32 getKeepInterval(void);

	void incSendBytes(DWORD);
	void incRecvBytes(DWORD);
	DWORD	getSendBytes(DWORD);
	DWORD	getRecvBytes(DWORD);

	EEpollServiceResult startService(void);             // EPOLL Œ­ºñœºžŠ œÃÀÛ
	EEpollServiceResult setProperties(CProperties*, TSTRING = _T(""));    // ÇÁ·ÎÆÛÆŒ œÃœºÅÛÀ» Œ³Á€
	EEpollServiceResult stopService(bool bMode = true); // EPOLL Œ­ºñœºžŠ ÁßÁö

	void setErrorCode(EEpollServiceResult);

	EEpollServiceResult setPort(uint32);         // Æ÷Æ® ¹øÈ£žŠ Œ³Á€
	EEpollServiceResult setIpAddress(string);    // IP ÁÖŒÒžŠ Œ³Á€
	EEpollServiceResult setMaxSession(uint32);   // ÃÖŽë µ¿œÃ Á¢ŒÓ ŒöžŠ Œ³Á€
	EEpollServiceResult setNumDispatch(uint32);  // µðœºÆÐÄ¡ ¿¬»êÀ» À§ÇÑ Ÿ²·¹µå °¹ŒöžŠ Œ³Á€
	EEpollServiceResult setBufferLevel(TSTRING); // ¹öÆÛ ·¹º§À» Œ³Á€

	EEpollServiceResult setKeepAlive(bool);
	EEpollServiceResult setKeepTime(uint32);
	EEpollServiceResult setKeepInterval(uint32);
	
	void eventHandle(sint32, sint32, void*);

	virtual void onCreateService(void)  = 0;
	virtual void onDestroyService(void) = 0; // ÇÏÀ§ Å¬·¡œº¿¡Œ­ ¹ÝµåœÃ ±žÇö

	virtual void callbackInitSession(CSession**)                   = 0;
	virtual void callbackCreateSession(CSession**)                 = 0;
	virtual void callbackCreateDispatchService(CDispatchService**) = 0; // ÇÏÀ§ Å¬·¡œº¿¡Œ­ ¹ÝµåœÃ ±žÇö
};

}

#endif