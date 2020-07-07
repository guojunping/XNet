///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __UDP_SERVICE_H
#define __UDP_SERVICE_H

#include "TypesDn.h"

#include "Log.h"
#include "Context.h"
#include "Properties.h"
#include "RotativeBuffer.h"
#include "DispatchService.h"

namespace XNetFrame{

typedef list<CUdpThread*> UdpThreadList;

#define UDP_SERVICE_CLASS      (_T("UdpService"))
#define ROTATIVE_BUFFER_CLASS  (_T("RotativeBuffer"))

#define HOST_PROPERTY          (_T("Host"))
#define PORT_PROPERTY          (_T("Port"))
#define BUFFER_LEVEL_PROPERTY  (_T("BufferLevel"))

class CUdpService{
public:
	enum EUdpServiceResult { USR_NULL, USR_FAILURE, USR_CONTEXT_CREATE_FAILURE, USR_CONTEXT_INIT_FAILURE, USR_CONTEXT_CONNECT_FAILURE,
		USR_ALREADY_SERVICE_RUNNING, USR_ALREADY_SERVICE_STOPPED, USR_INVALID_PROPERTY, USR_THREAD_START_FAILURE, USR_OK };

private:
	CLog*             m_pLog;             // 
	CContext*         m_pContext;         // 
	CProperties*      m_pProperties;      // 
	CDispatchService* m_pDispatchService; // 

	string            m_sHost;            // 
	uint32            m_uiPort;           // 
	sint32            m_siRefCount;       // 
	uint32            m_uiErrorCode;      // 
	uint32            m_uiPortRange;      // 
	bool              m_bServiceRunning;  // 

	TSTRING			m_sUdpServiceClass; // 
	TSTRING			m_sPortProperty;
	TSTRING			m_sHostProperty;
	TSTRING			m_sRotativeBufferProperty;

	EUdpServiceResult             m_usrResult;       // 
	CRotativeBuffer::LBufferLevel m_emBufferLevel;   // 
	UdpThreadList                 m_liUdpThreadList; // 

private:
	EUdpServiceResult createService(void);         // 
	EUdpServiceResult createContext(void);         // 
	EUdpServiceResult destroyService(void);        // 
	EUdpServiceResult createDispatchService(void); // 

public:
	CUdpService(CLog*);
	virtual ~CUdpService(void);

	uint32    getPort(void);      // 
	CContext* getContext(void);   // 
	string    getIpAddress(void); // 
	uint32    getErrorCode(void);

	void setErrorCode(EUdpServiceResult); // 

	EUdpServiceResult setPort(uint32);         // 
	EUdpServiceResult setIpAddress(string);    // 
	EUdpServiceResult setBufferLevel(TSTRING); // 

	EUdpServiceResult stopService(void);                             // 
	EUdpServiceResult startService(uint32);                          // 
	EUdpServiceResult setProperties(CProperties*, TSTRING = _T(""), TSTRING = _T("")); // 

	virtual void onCreateService(void)  = 0;
	virtual void onDestroyService(void) = 0; // 

	virtual void callbackCreateContext(CContext**)                 = 0;
	
};

}

#endif
