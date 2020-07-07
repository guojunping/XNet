///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __TCP_SERVICE_H
#define __TCP_SERVICE_H

#include "TypesDn.h"

#include "Log.h"
#include "Channel.h"
//#include "TcpThread.h"
#include "Properties.h"
#include "RotativeBuffer.h"
#include "DispatchService.h"

namespace XNetFrame{

typedef list<CTcpThread*> TcpThreadList;

#define TCP_SERVICE_CLASS      (_T("TcpService"))
#define ROTATIVE_BUFFER_CLASS  (_T("RotativeBuffer"))

#define PORT_PROPERTY          (_T("Port"))
#define HOST_PROPERTY          (_T("Host"))
#define BUFFER_LEVEL_PROPERTY  (_T("BufferLevel"))
#define KEEP_ALIVE			(_T("KeepAlive"))
#define KEEP_TIME				(_T("KeepTime"))
#define KEEP_INTERVAL		(_T("KeepInterval"))

class CTcpService{
public:
	enum ETcpServiceResult { TSR_NULL, TSR_FAILURE, TSR_CHANNEL_CREATE_FAILURE, TSR_CHANNEL_INIT_FAILURE, TSR_CHANNEL_CONNECT_FAILURE, 
		TSR_CHANNEL_BIND_FAILURE, TSR_CHANNEL_SET_OPTION_FAILURE, 
		TSR_ALREADY_SERVICE_RUNNING, TSR_ALREADY_SERVICE_STOPPED, TSR_INVALID_PROPERTY, TSR_THREAD_START_FAILURE, TSR_OK };
protected:
	CLog*             m_pLog;             // 
private:	
	CChannel*         m_pChannel;         // 
	CProperties*      m_pProperties;      // 
	CDispatchService* m_pDispatchService; // 

	string				m_sHost;            // 
	uint32				m_uiPort;           // 
	sint32				m_siRefCount;       // 
	uint32				m_uiErrorCode;      // 
	bool				m_bServiceRunning;  // 
	uint32				m_uiKeepAlive;
	uint32				m_uiKeepTime;
	uint32				m_uiKeepInterval;

	TSTRING		m_sTcpServiceClass; // 
	TSTRING		m_sPortProperty;
	TSTRING		m_sHostProperty;
	TSTRING		m_sRotativeBufferProperty;

	ETcpServiceResult             m_tsrResult;       // 
	CRotativeBuffer::LBufferLevel m_emBufferLevel;   // 
	TcpThreadList                 m_liTcpThreadList; // 

private:
	ETcpServiceResult createService(void);         // 
	ETcpServiceResult createChannel(void);         // 
	ETcpServiceResult destroyService(void);        // 
	ETcpServiceResult createDispatchService(void); // 

public:
	CTcpService(CLog*);
	virtual ~CTcpService(void);

	uint32    getPort(void);          // 
	CChannel* getChannel(void);       // 
	string    getIpAddress(void);     // 
	uint32    getErrorCode(void);
	bool      isServiceRunning(void); // 

	void setErrorCode(ETcpServiceResult);

	ETcpServiceResult setPort(uint32);         // 
	ETcpServiceResult setIpAddress(string);    // 
	ETcpServiceResult setBufferLevel(TSTRING); // 

	ETcpServiceResult stopService(void);                             // 
	ETcpServiceResult startService(void);                            // 
	ETcpServiceResult startService(string&, uint16);
	ETcpServiceResult setProperties(CProperties*, TSTRING = _T("")); //
	
	CDispatchService* getDispatchService(){  return m_pDispatchService ; };

	virtual void onCreateService(void)  = 0;
	virtual void onDestroyService(void) = 0; // 

	virtual void callbackCreateChannel(CChannel**)                 = 0;
	
};

}

#endif

