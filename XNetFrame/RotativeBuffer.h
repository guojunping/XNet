/**
 * rotative buffer
 * 
 */

#ifndef __ROTATIVE_BUFFER_H
#define __ROTATIVE_BUFFER_H

#include "TypesDn.h"

#include "InetAddress.h"
#include "AuthCrypt.h"

namespace XNetFrame{

#define VERY_LARGE_LEVEL_BUFFER_LEN       (256000)
#define VERY_LARGE_LEVEL_EXT_BUFFER_LEN   (4096)

#define LARGE_LEVEL_BUFFER_LEN             (40960)
#define LARGE_LEVEL_EXT_BUFFER_LEN         (4096)

#define NORMAL_LEVEL_BUFFER_LEN             (20480)
#define NORMAL_LEVEL_EXT_BUFFER_LEN        (4096)

#define SMALL_LEVEL_BUFFER_LEN            (20480)
#define SMALL_LEVEL_EXT_BUFFER_LEN        (4096)

//#define VERY_SMALL_LEVEL_BUFFER_LEN       (10240)
//#define VERY_SMALL_LEVEL_EXT_BUFFER_LEN   (2048)
#define VERY_SMALL_LEVEL_BUFFER_LEN       (256000*3)
#define VERY_SMALL_LEVEL_EXT_BUFFER_LEN   (4096)

class CRotativeBuffer{
public:
	enum LBufferLevel          { BL_VERY_LARGE, BL_LARGE, BL_NORMAL, BL_SMALL, BL_VERY_SMALL };
	enum ERotativeBufferResult { RBR_NULL, RBR_FAILURE, RBR_PACKET_BIG, RBR_BUFFER_OVERFLOW, RBR_MSG_LEN, RBR_OK };

private:
	char* m_szRecvBuffer;     // 
	char* m_szSendBuffer;     // 

	char* m_pHeadRecvPtr;     // 
	char* m_pTailRecvPtr;     // 
	char* m_pMarkRecvPtr;     // 

	char* m_pHeadSendPtr;     // 
	char* m_pTailSendPtr;     // 

	char* m_pFinalRecvPtr;    // 
	char* m_pFinalRecvExtPtr; // 

	char* m_pFinalSendPtr;    // 
	char* m_pFinalSendExtPtr; // 

	uint32 m_uiMaxRecvLen;    // 
	uint32 m_uiMaxRecvExtLen; // 

	uint32 m_uiMaxSendLen;    // 
	uint32 m_uiMaxSendExtLen; // 

	uint32 m_uiRemainRecvLen; // 
	uint32 m_uiRemainSendLen; // 

	uint32 m_uiHeaderPos;     // 
	uint32 m_uiHeaderLen;     // 

	uint32 m_uiMessageIdPos;  // 
	uint32 m_uiMessageIdLen;  // 

	CAuthCrypt	m_acAuthCrypt;

private:
	void setBufferLength(LBufferLevel); // 

public:
	CRotativeBuffer(void);
	CRotativeBuffer(LBufferLevel);
	virtual ~CRotativeBuffer(void);

	bool isOverflowRecvBuffer(uint32); // 
	bool isOverflowSendBuffer(uint32); // 

	char*  getHeadSendPtr(void);          // 
	char*  getTailRecvPtr(void);          // 
	char*  getMarkRecvPtr(void);          // 
	uint32 getRemainRecvLen(void);        // 
	uint32 getRemainSendLen(void);        // 
	sint32 getDiffTailMarkRecvPtr(void);  // 
	sint32 getDiffFinalMarkRecvPtr(void); // 
	sint32 getDiffFinalHeadSendPtr(void); // 

	uint32 getMessageLen(char*);              // 
	void   copyOriginFinalRecvBuffer(uint32); // 

	ERotativeBufferResult initBuffer(void);                           // 
	void                  forwardHeadSendPtr(uint32);                 // 
	ERotativeBufferResult forwardTailRecvPtr(uint32);                 // 
	void                  forwardMarkRecvPtr(uint32);                 // 
	ERotativeBufferResult getMessage(uint32*, char**);                // 
	ERotativeBufferResult addRecvBuffer(CInetAddress*);               // 
	ERotativeBufferResult addSendBuffer(const char*, uint32, char**);       // 
	ERotativeBufferResult getMessage(uint32*, char**, CInetAddress*); // 
	void                  delMessage(uint32);
};

}

#endif
