///////////////////////////////////////
// 与ChatServer的socket连接通道
// 基于 TCP socket
///////////////////////////////////////

#ifndef __X_CHANNEL_H
#define __X_CHANNEL_H

#include "Channel.h"

namespace XNetFrame{
class CXNetConnector;

class CXNetChannel : public CChannel{
public:
	CXNetChannel(CXNetConnector*,CLog*);
	~CXNetChannel(void);

	void onConnect(void);
	void onClose(CChannel::SCloseStatus);
	
	bool SendPacket(void *pData, uint32 uiLen);
private:
    CXNetConnector *m_pXNetConnector;
};
}
#endif // __X_CHANNEL_H
