///////////////////////////////////////
// Author : Mr Kim
// Name : A buffer with adjustable size
///////////////////////////////////////

#ifndef __ADJUSTABLE_BUFFER_H
#define __ADJUSTABLE_BUFFER_H

#include "TypesDn.h"

namespace XNetFrame{

//#define BUFFER_LEN       (10240)
#define BUFFER_LEN       (102400)
#define EXT_BUFFER_LEN   (4096)

class CAdjustableBuffer{
public:
	enum EAdjustableBufferResult { ABR_NULL, ABR_OK, ABR_PACKET_BIG, ABR_BUFFER_OVERFLOW, ABR_MSG_LEN, ABR_FAILURE };

private:
	char* m_szRecvBuffer;     // The buffer for receiving data

	char* m_pHeadRecvPtr;     // 
	char* m_pTailRecvPtr;     // 
	char* m_pMarkRecvPtr;     // 

	char* m_pFinalRecvPtr;    // 
	char* m_pFinalRecvExtPtr; // 

	uint32 m_uiMaxRecvLen;    // The maximum length of the receive buffer
	uint32 m_uiMaxRecvExtLen; // The maximum length for the extension

	uint32 m_uiRemainRecvLen; // The size of already received messages

	uint32 m_uiHeaderPos;     // The message head position
	uint32 m_uiHeaderLen;     // The size of message length

	uint32 m_uiMessageIdPos;  // The message id position
	uint32 m_uiMessageIdLen;  // The size of message id

private:
	bool isOverflowRecvBuffer(uint32); // 

public:
	CAdjustableBuffer(void);
	virtual ~CAdjustableBuffer(void);

	char*  getMarkRecvPtr(void);          // 
	sint32 getDiffTailMarkRecvPtr(void);  // 
	sint32 getDiffFinalMarkRecvPtr(void); // 

	void forwardMarkRecvPtr(uint32);        // 
	void copyOriginFinalRecvBuffer(uint32); // 

	EAdjustableBufferResult initBuffer(void);             // 
	EAdjustableBufferResult getMessage(uint32*, char**);  // 
	EAdjustableBufferResult addRecvBuffer(char*, uint32); // 
};

}

#endif
