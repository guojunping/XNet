/**
 * type definition
 * 
 */ 
#ifndef __TYPES_DN_H
#define __TYPES_DN_H

#include "Platform.h"

#define DEFAULT_CRYPT_STR	("guojunping")

#include <string>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <algorithm>

namespace XNetFrame{
	
class CSocketImpl ;
class CServerSocket ;
class CTcpThread ;
class CUdpThread ;
class CLogThread ;
class CLog;
class CDispatchThread ;
class CMessageHandler ;
class CDispatchService ;
class CSessionFactory ;
class CSession ;
class CChannel ;
class CContext ;
class CEpollAsyncIoThread ;
class CEpollImpl ;
class CEpoll ;
class CEpollService ;
class CScheduleThread ;
class CScheduleService ;
class CEventHandlerFactory ;
class CAutomaticResetEvent;

}
#endif  
