///////////////////////////////////////
// 개 발 자 : 김경석
// 설    명 : IOCP 서비스에 대한 구현
///////////////////////////////////////

/*
0. 프로퍼티 정보를 획득
1. 접속 관리용 IOCP 객체를 생성
2. 비동기 I/O용 IOCP 객체를 생성
3. 서버 소켓 객체를 오버랩드 모드로 생성
4. 서버 소켓을 접속 관리용 IOCP 객체에 바인딩
5. 디스패치 서비스 객체를 다수개 생성
6. 세션 객체를 다수개 생성하고 세션 내에 소켓, 회전형 버퍼를 생성 및 초기화한 후 접속에 대한 트리거를 설정하고 세션 팩토리에 추가
7. 디스패치 서비스 객체를 실행
8. 비동기 I/O용 쓰레드를 다수개 생성하고 실행
9. 접속 관리용 쓰레드를 1개 생성하고 실행

8.1. 접속 관리용 IOCP 객체로부터 콘트롤이 전이
8.2. 전이된 오버랩드 구조체에서 세션 객체를 획득
8.3. 세션 객체를 비동기 I/O용 IOCP 객체에 바인딩
8.4. 비동기 입력에 대한 트리거를 설정

9.1. 비동기 I/O용 IOCP 객체로부터 콘트롤이 전이
9.2. 전이된 세션 객체에 대하여 최종 결과를 획득
9.3. 입력 모드인 경우 회전형 수신 버퍼의 테일 포인터를 전진시키고 마크 포인터를 전진시키고 패킷을 처리한 후 비동기 입력에 대한 트리거를 설정
9.4. 출력 모드인 경우 송신 버퍼의 헤드 포인터를 전진시키고 남은 비동기 출력에 대한 트리거를 설정

?. 출력시 회전형 송신 버퍼의 테일 포인터를 전진시키고 비동기 출력에 대한 트리거를 설정
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

    CIocpImpl* m_pAcceptIocp;                // 접속 관리용 IOCP 객체
    CIocpImpl* m_pAsyncIoIocp[MAX_NUM_IOCP]; // 비동기 I/O용 IOCP 객체

private:
	CLog*          m_pLog;            // 로그 시스템 객체
	CProperties*   m_pProperties;     // 프로퍼티 시스템 객체
	CServerSocket* m_pServerSocket;   // 서버 소켓 객체
	SOCKET         m_sServerSocketId; // 서버 소켓 ID

	CMonitor		m_cMonitor;
	DWORD		m_dwSendBytes;
	DWORD		m_dwRecvBytes;

	string         m_sHost;           // 서버 호스트 이름
	uint32         m_uiPort;          // 서버 포트 번호

	uint32         m_uiErrorCode;     // 에러 코드 번호
	uint32         m_uiMaxSession;    // 최대 세션 객체수
	uint32         m_uiNumDispatch;   // 디스패치 연산을 위한 쓰레드 갯수
	uint32         m_uiDispatchIndex; // 라운드 로빈 방식의 디스패치 서비스 인덱스
	uint32			m_uiKeepAlive;
	uint32			m_uiKeepTime;
	uint32			m_uiKeepInterval;

	sint32         m_siRefCount;      // 객체의 참조 카운트(서비스 시작시 증가, 서비스 중지시 감소)
	TSTRING        m_sServerName;     // 서버 이름
	bool           m_bServiceRunning; // 서비스 활성화 여부

	TSTRING		   m_sIocpServiceClass;
	TSTRING		   m_sMaxSessionProperty;
	TSTRING		   m_sNumDispatchProperty;
	TSTRING		   m_sRotativeBufferLevelProperty;

	CRotativeBuffer::LBufferLevel m_emBufferLevel; // 회전형 버퍼의 레벨

	DispatchServiceMap    m_mDispatchServices;       // 디스패치 서비스 맵
	IocpAcceptThreadList  m_liIocpAcceptThreadList;  // 접속 관리용 쓰레드 리스트
	IocpAsyncIoThreadList m_liIocpAsyncIoThreadList; // 비동기 I/O용 쓰레드 리스트

	EIocpServiceResult m_isrResult;

private:
	uint32 getThreadCount(void); // IOCP 서비스에 적절한 쓰레드 갯수를 리턴

	EIocpServiceResult createIocp(void);            // IOCP 객체를 생성
	EIocpServiceResult createSocket(void);          // 서버 소켓을 생성
	EIocpServiceResult createSession(void);         // 세션 객체를 지정된 갯수만큼 생성
	EIocpServiceResult createService(void);         // IOCP 서비스를 생성
	EIocpServiceResult destroyService(void);        // IOCP 서비스를 파괴
	EIocpServiceResult createDispatchService(void); // 디스패치 서비스 객체를 지정된 갯수만큼 생성

public:
	CIocpService(CLog*);
	~CIocpService(void);

	uint32             getPort(void);              // 포트 번호를 획득
	string             getIpAddress(void);         // IP 주소를 획득
	uint32             getErrorCode(void);
	CProperties*       getProperties(void);        // 프로퍼티 시스템을 획득
	TSTRING            getServerName(void);        // 서버 이름을 획득
	uint32             getMaxSession(void);        // 최대 동시 접속 수를 획득
	uint32             getNumDispatch(void);       // 디스패치 연산을 위한 쓰레드 갯수를 획득
	void               setServerName(LPTSTR);      // 서버 이름을 설정
	bool               isServiceRunning(void);     // 서비스 진행 여부를 리턴
	CDispatchService*  getDispatchService(void);   // 라운드 로빈 방식으로 디스패치 서비스 객체의 포인터를 리턴
	CDispatchService*  getDispatchService(uint32); // 디스패치 인덱스에 부합하는 디스패치 서비스 객체의 포인터를 리턴

	bool		getKeepAlive(void);
	uint32 getKeepTime(void);
	uint32 getKeepInterval(void);

	void incSendBytes(DWORD);
	void incRecvBytes(DWORD);
	DWORD	getSendBytes(DWORD);
	DWORD	getRecvBytes(DWORD);

	EIocpServiceResult startService(void);             // IOCP 서비스를 시작
	EIocpServiceResult setProperties(CProperties*, TSTRING = _T(""));    // 프로퍼티 시스템을 설정
	EIocpServiceResult stopService(bool bMode = true); // IOCP 서비스를 중지

	void setErrorCode(EIocpServiceResult);

	EIocpServiceResult setPort(uint32);         // 포트 번호를 설정
	EIocpServiceResult setIpAddress(string);    // IP 주소를 설정
	EIocpServiceResult setMaxSession(uint32);   // 최대 동시 접속 수를 설정
	EIocpServiceResult setNumDispatch(uint32);  // 디스패치 연산을 위한 쓰레드 갯수를 설정
	EIocpServiceResult setBufferLevel(TSTRING); // 버퍼 레벨을 설정

	EIocpServiceResult setKeepAlive(bool);
	EIocpServiceResult setKeepTime(uint32);
	EIocpServiceResult setKeepInterval(uint32);

	virtual void onCreateService(void)  = 0;
	virtual void onDestroyService(void) = 0; // 하위 클래스에서 반드시 구현

	virtual void callbackInitSession(CSession**)                   = 0;
	virtual void callbackCreateSession(CSession**)                 = 0;
	virtual void callbackCreateDispatchService(CDispatchService**) = 0; // 하위 클래스에서 반드시 구현
};

}

#endif
