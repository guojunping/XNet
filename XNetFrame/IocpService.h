///////////////////////////////////////
// �� �� �� : ��漮
// ��    �� : IOCP ���񽺿� ���� ����
///////////////////////////////////////

/*
0. ������Ƽ ������ ȹ��
1. ���� ������ IOCP ��ü�� ����
2. �񵿱� I/O�� IOCP ��ü�� ����
3. ���� ���� ��ü�� �������� ���� ����
4. ���� ������ ���� ������ IOCP ��ü�� ���ε�
5. ����ġ ���� ��ü�� �ټ��� ����
6. ���� ��ü�� �ټ��� �����ϰ� ���� ���� ����, ȸ���� ���۸� ���� �� �ʱ�ȭ�� �� ���ӿ� ���� Ʈ���Ÿ� �����ϰ� ���� ���丮�� �߰�
7. ����ġ ���� ��ü�� ����
8. �񵿱� I/O�� �����带 �ټ��� �����ϰ� ����
9. ���� ������ �����带 1�� �����ϰ� ����

8.1. ���� ������ IOCP ��ü�κ��� ��Ʈ���� ����
8.2. ���̵� �������� ����ü���� ���� ��ü�� ȹ��
8.3. ���� ��ü�� �񵿱� I/O�� IOCP ��ü�� ���ε�
8.4. �񵿱� �Է¿� ���� Ʈ���Ÿ� ����

9.1. �񵿱� I/O�� IOCP ��ü�κ��� ��Ʈ���� ����
9.2. ���̵� ���� ��ü�� ���Ͽ� ���� ����� ȹ��
9.3. �Է� ����� ��� ȸ���� ���� ������ ���� �����͸� ������Ű�� ��ũ �����͸� ������Ű�� ��Ŷ�� ó���� �� �񵿱� �Է¿� ���� Ʈ���Ÿ� ����
9.4. ��� ����� ��� �۽� ������ ��� �����͸� ������Ű�� ���� �񵿱� ��¿� ���� Ʈ���Ÿ� ����

?. ��½� ȸ���� �۽� ������ ���� �����͸� ������Ű�� �񵿱� ��¿� ���� Ʈ���Ÿ� ����
*/

#ifndef __IOCP_SERVICE_H
#define __IOCP_SERVICE_H

#include "TypesDn.h"

#include "Log.h"
#include "System.h"
#include "Session.h"
#include "IocpImpl.h"
#include "Properties.h"
#include "SocketImpl.h"
#include "InetAddress.h"
#include "ServerSocket.h"
#include "SessionFactory.h"
#include "RotativeBuffer.h"
#include "DispatchService.h"
#include "IocpAcceptThread.h"
#include "IocpAsyncIoThread.h"
#include "Monitor.h"

namespace DnNetworkFramework{

#define MAX_NUM_IOCP   (32)

#define KEY_CLOSE_IOCP (0x00000003)

typedef hash_map<uint32, CDispatchService*> DispatchServiceMap;
typedef list<CIocpAcceptThread*>            IocpAcceptThreadList;
typedef list<CIocpAsyncIoThread*>           IocpAsyncIoThreadList;

#define SESSION_CLASS          (_T("Session"))
#define IOCP_SERVICE_CLASS     (_T("IocpService"))
#define ROTATIVE_BUFFER_CLASS  (_T("RotativeBuffer"))
#define DISPATCH_SERVICE_CLASS (_T("DispatchService"))

#define PORT_PROPERTY          (_T("Port"))
#define HOST_PROPERTY          (_T("Host"))
#define MAX_SESSION_PROPERTY   (_T("MaxSession"))
#define NUM_DISPATCH_PROPERTY  (_T("NumDispatch"))
#define BUFFER_LEVEL_PROPERTY  (_T("BufferLevel"))
#define KEEP_ALIVE			(_T("KeepAlive"))
#define KEEP_TIME				(_T("KeepTime"))
#define KEEP_INTERVAL		(_T("KeepInterval"))

class CIocpService : public CSessionFactory{
public:
	enum EIocpServiceResult { ISR_NULL, ISR_FAILURE, ISR_SESSION_CREATE_FAILURE, ISR_SESSION_INIT_FAILURE, ISR_SESSION_ADD_FAILURE,
		ISR_ALREADY_SERVICE_RUNNING, ISR_ALREADY_SERVICE_STOPPED, ISR_THREAD_START_FAILURE, ISR_INVALID_PROPERTY, ISR_OK };

    CIocpImpl* m_pAcceptIocp;                // ���� ������ IOCP ��ü
    CIocpImpl* m_pAsyncIoIocp[MAX_NUM_IOCP]; // �񵿱� I/O�� IOCP ��ü

private:
	CLog*          m_pLog;            // �α� �ý��� ��ü
	CProperties*   m_pProperties;     // ������Ƽ �ý��� ��ü
	CServerSocket* m_pServerSocket;   // ���� ���� ��ü
	SOCKET         m_sServerSocketId; // ���� ���� ID

	CMonitor		m_cMonitor;
	DWORD		m_dwSendBytes;
	DWORD		m_dwRecvBytes;

	string         m_sHost;           // ���� ȣ��Ʈ �̸�
	uint32         m_uiPort;          // ���� ��Ʈ ��ȣ

	uint32         m_uiErrorCode;     // ���� �ڵ� ��ȣ
	uint32         m_uiMaxSession;    // �ִ� ���� ��ü��
	uint32         m_uiNumDispatch;   // ����ġ ������ ���� ������ ����
	uint32         m_uiDispatchIndex; // ���� �κ� ����� ����ġ ���� �ε���
	uint32			m_uiKeepAlive;
	uint32			m_uiKeepTime;
	uint32			m_uiKeepInterval;

	sint32         m_siRefCount;      // ��ü�� ���� ī��Ʈ(���� ���۽� ����, ���� ������ ����)
	TSTRING        m_sServerName;     // ���� �̸�
	bool           m_bServiceRunning; // ���� Ȱ��ȭ ����

	TSTRING		   m_sIocpServiceClass;
	TSTRING		   m_sMaxSessionProperty;
	TSTRING		   m_sNumDispatchProperty;
	TSTRING		   m_sRotativeBufferLevelProperty;

	CRotativeBuffer::LBufferLevel m_emBufferLevel; // ȸ���� ������ ����

	DispatchServiceMap    m_mDispatchServices;       // ����ġ ���� ��
	IocpAcceptThreadList  m_liIocpAcceptThreadList;  // ���� ������ ������ ����Ʈ
	IocpAsyncIoThreadList m_liIocpAsyncIoThreadList; // �񵿱� I/O�� ������ ����Ʈ

	EIocpServiceResult m_isrResult;

private:
	uint32 getThreadCount(void); // IOCP ���񽺿� ������ ������ ������ ����

	EIocpServiceResult createIocp(void);            // IOCP ��ü�� ����
	EIocpServiceResult createSocket(void);          // ���� ������ ����
	EIocpServiceResult createSession(void);         // ���� ��ü�� ������ ������ŭ ����
	EIocpServiceResult createService(void);         // IOCP ���񽺸� ����
	EIocpServiceResult destroyService(void);        // IOCP ���񽺸� �ı�
	EIocpServiceResult createDispatchService(void); // ����ġ ���� ��ü�� ������ ������ŭ ����

public:
	CIocpService(CLog*);
	~CIocpService(void);

	uint32             getPort(void);              // ��Ʈ ��ȣ�� ȹ��
	string             getIpAddress(void);         // IP �ּҸ� ȹ��
	uint32             getErrorCode(void);
	CProperties*       getProperties(void);        // ������Ƽ �ý����� ȹ��
	TSTRING            getServerName(void);        // ���� �̸��� ȹ��
	uint32             getMaxSession(void);        // �ִ� ���� ���� ���� ȹ��
	uint32             getNumDispatch(void);       // ����ġ ������ ���� ������ ������ ȹ��
	void               setServerName(LPTSTR);      // ���� �̸��� ����
	bool               isServiceRunning(void);     // ���� ���� ���θ� ����
	CDispatchService*  getDispatchService(void);   // ���� �κ� ������� ����ġ ���� ��ü�� �����͸� ����
	CDispatchService*  getDispatchService(uint32); // ����ġ �ε����� �����ϴ� ����ġ ���� ��ü�� �����͸� ����

	bool		getKeepAlive(void);
	uint32 getKeepTime(void);
	uint32 getKeepInterval(void);

	void incSendBytes(DWORD);
	void incRecvBytes(DWORD);
	DWORD	getSendBytes(DWORD);
	DWORD	getRecvBytes(DWORD);

	EIocpServiceResult startService(void);             // IOCP ���񽺸� ����
	EIocpServiceResult setProperties(CProperties*, TSTRING = _T(""));    // ������Ƽ �ý����� ����
	EIocpServiceResult stopService(bool bMode = true); // IOCP ���񽺸� ����

	void setErrorCode(EIocpServiceResult);

	EIocpServiceResult setPort(uint32);         // ��Ʈ ��ȣ�� ����
	EIocpServiceResult setIpAddress(string);    // IP �ּҸ� ����
	EIocpServiceResult setMaxSession(uint32);   // �ִ� ���� ���� ���� ����
	EIocpServiceResult setNumDispatch(uint32);  // ����ġ ������ ���� ������ ������ ����
	EIocpServiceResult setBufferLevel(TSTRING); // ���� ������ ����

	EIocpServiceResult setKeepAlive(bool);
	EIocpServiceResult setKeepTime(uint32);
	EIocpServiceResult setKeepInterval(uint32);

	virtual void onCreateService(void)  = 0;
	virtual void onDestroyService(void) = 0; // ���� Ŭ�������� �ݵ�� ����

	virtual void callbackInitSession(CSession**)                   = 0;
	virtual void callbackCreateSession(CSession**)                 = 0;
	virtual void callbackCreateDispatchService(CDispatchService**) = 0; // ���� Ŭ�������� �ݵ�� ����
};

}

#endif
