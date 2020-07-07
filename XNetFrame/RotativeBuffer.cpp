/**
 * 
 * 
 */

#include "RotativeBuffer.h"
#include <string.h>

namespace XNetFrame{

CRotativeBuffer::CRotativeBuffer(LBufferLevel emBufferLevel){
	m_uiRemainRecvLen  = 0;
	m_uiRemainSendLen  = 0;

	m_pHeadRecvPtr     = NULL;
	m_pTailRecvPtr     = NULL;
	m_pMarkRecvPtr     = NULL;

	m_pHeadSendPtr     = NULL;
	m_pTailSendPtr     = NULL;

	m_pFinalRecvPtr    = NULL;
	m_pFinalRecvExtPtr = NULL;

	m_pFinalSendPtr    = NULL;
	m_pFinalSendExtPtr = NULL;

	setBufferLength(emBufferLevel);

	m_uiHeaderPos      = 0;
	m_uiHeaderLen      = sizeof(uint32);

	m_uiMessageIdPos   = m_uiHeaderLen;
	m_uiMessageIdLen   = sizeof(uint32);

	m_szRecvBuffer = new char[m_uiMaxRecvLen + m_uiMaxRecvExtLen];
	m_szSendBuffer = new char[m_uiMaxSendLen + m_uiMaxSendExtLen];
}

CRotativeBuffer::CRotativeBuffer(void){
	m_uiRemainRecvLen  = 0;
	m_uiRemainSendLen  = 0;

	m_pHeadRecvPtr     = NULL;
	m_pTailRecvPtr     = NULL;
	m_pMarkRecvPtr     = NULL;

	m_pHeadSendPtr     = NULL;
	m_pTailSendPtr     = NULL;

	m_pFinalRecvPtr    = NULL;
	m_pFinalRecvExtPtr = NULL;

	m_pFinalSendPtr    = NULL;
	m_pFinalSendExtPtr = NULL;

	setBufferLength(BL_NORMAL);

	m_uiHeaderPos      = 0;
	m_uiHeaderLen      = sizeof(uint32);

	m_uiMessageIdPos   = m_uiHeaderLen;
	m_uiMessageIdLen   = sizeof(uint32);

	m_szRecvBuffer = new char[m_uiMaxRecvLen + m_uiMaxRecvExtLen];
	m_szSendBuffer = new char[m_uiMaxSendLen + m_uiMaxSendExtLen];
}

CRotativeBuffer::~CRotativeBuffer(void){
	if(m_szRecvBuffer != NULL){
		delete [] m_szRecvBuffer;
		m_szRecvBuffer = NULL;
	}
	if(m_szSendBuffer != NULL){
		delete [] m_szSendBuffer;
		m_szSendBuffer = NULL;
	}
}

void CRotativeBuffer::setBufferLength(LBufferLevel emBufferLevel){ // 
	switch(emBufferLevel){
		case BL_VERY_LARGE :
			m_uiMaxRecvLen    = m_uiMaxSendLen    = VERY_LARGE_LEVEL_BUFFER_LEN;
			m_uiMaxRecvExtLen = m_uiMaxSendExtLen = VERY_LARGE_LEVEL_EXT_BUFFER_LEN;
			break;
			
		case BL_LARGE :
			m_uiMaxRecvLen    = m_uiMaxSendLen    = LARGE_LEVEL_BUFFER_LEN;
			m_uiMaxRecvExtLen = m_uiMaxSendExtLen = LARGE_LEVEL_EXT_BUFFER_LEN;
			break;
			
		case BL_NORMAL :
			m_uiMaxRecvLen    = m_uiMaxSendLen    = NORMAL_LEVEL_BUFFER_LEN;
			m_uiMaxRecvExtLen = m_uiMaxSendExtLen = NORMAL_LEVEL_EXT_BUFFER_LEN;
			break;
			
		case BL_SMALL :
			m_uiMaxRecvLen    = m_uiMaxSendLen    = SMALL_LEVEL_BUFFER_LEN;
			m_uiMaxRecvExtLen = m_uiMaxSendExtLen = SMALL_LEVEL_EXT_BUFFER_LEN;
			break;

		case BL_VERY_SMALL :
			m_uiMaxRecvLen    = m_uiMaxSendLen    = VERY_SMALL_LEVEL_BUFFER_LEN;
			m_uiMaxRecvExtLen = m_uiMaxSendExtLen = VERY_SMALL_LEVEL_EXT_BUFFER_LEN;
			break;

		default :
			m_uiMaxRecvLen    = m_uiMaxSendLen    = NORMAL_LEVEL_BUFFER_LEN;
			m_uiMaxRecvExtLen = m_uiMaxSendExtLen = NORMAL_LEVEL_EXT_BUFFER_LEN;
			break;
	};
}

CRotativeBuffer::ERotativeBufferResult CRotativeBuffer::initBuffer(void){ // 
	if(m_szRecvBuffer == NULL) m_szRecvBuffer = new char[m_uiMaxRecvLen + m_uiMaxRecvExtLen];
	if(m_szSendBuffer == NULL) m_szSendBuffer = new char[m_uiMaxSendLen + m_uiMaxSendExtLen];

	if(m_szRecvBuffer == NULL || m_szSendBuffer == NULL) return RBR_NULL;

	memset(m_szRecvBuffer, 0, m_uiMaxRecvLen + m_uiMaxRecvExtLen);
	memset(m_szSendBuffer, 0, m_uiMaxSendLen + m_uiMaxSendExtLen);

	m_pHeadRecvPtr = m_szRecvBuffer;
	m_pTailRecvPtr = m_szRecvBuffer;
	m_pMarkRecvPtr = m_szRecvBuffer;

	m_pHeadSendPtr = m_szSendBuffer;
	m_pTailSendPtr = m_szSendBuffer;

	m_pFinalRecvPtr    = m_szRecvBuffer + m_uiMaxRecvLen;
	m_pFinalRecvExtPtr = m_szRecvBuffer + m_uiMaxRecvLen + m_uiMaxRecvExtLen;

	m_pFinalSendPtr    = m_szSendBuffer + m_uiMaxSendLen;
	m_pFinalSendExtPtr = m_szSendBuffer + m_uiMaxSendLen + m_uiMaxSendExtLen;

	m_uiRemainRecvLen = m_uiMaxRecvLen;
	m_uiRemainSendLen = 0;

	return RBR_OK;
}

uint32 CRotativeBuffer::getRemainRecvLen(void){ // 
	return m_uiRemainRecvLen;
}

uint32 CRotativeBuffer::getRemainSendLen(void){ // 
	return m_uiRemainSendLen;
}

char* CRotativeBuffer::getTailRecvPtr(void){ // 
	return m_pTailRecvPtr;
}

char* CRotativeBuffer::getHeadSendPtr(void){ //
	return m_pHeadSendPtr;
}

char* CRotativeBuffer::getMarkRecvPtr(void){ //
	return m_pMarkRecvPtr;
}

CRotativeBuffer::ERotativeBufferResult CRotativeBuffer::forwardTailRecvPtr(uint32 uiReceivedBytes){ // 
	sint32 siSize;

	if(uiReceivedBytes > m_uiMaxRecvLen) return RBR_PACKET_BIG;                   // 
	if(isOverflowRecvBuffer(uiReceivedBytes) == true) return RBR_BUFFER_OVERFLOW;

	siSize = (sint32)(m_pTailRecvPtr + uiReceivedBytes - m_pFinalRecvPtr);

	if(siSize >= 0){ // 
		memcpy(m_szRecvBuffer, m_pFinalRecvPtr, (size_t)siSize);
		m_pTailRecvPtr = m_szRecvBuffer + siSize;
	}
	else{ // 
		m_pTailRecvPtr += uiReceivedBytes;
	}

	m_uiRemainRecvLen -= uiReceivedBytes;

	return RBR_OK;
}

bool CRotativeBuffer::isOverflowRecvBuffer(uint32 uiReceivedBytes){ // 
	if(m_uiRemainRecvLen <= uiReceivedBytes) return true;
	else return false;
}

void CRotativeBuffer::forwardHeadSendPtr(uint32 uiSentBytes){ // 
	sint32 siSize;

	siSize = (sint32)(m_pHeadSendPtr + uiSentBytes - m_pFinalSendPtr);

	if(siSize >= 0){ // 
		m_pHeadSendPtr = m_szSendBuffer + siSize;
	}
	else{ // 
		m_pHeadSendPtr += uiSentBytes;
	}

	m_uiRemainSendLen -= uiSentBytes;
}

void CRotativeBuffer::forwardMarkRecvPtr(uint32 uiLen){ // 
	m_pMarkRecvPtr += uiLen;

	if(m_pMarkRecvPtr >= (m_szRecvBuffer + m_uiMaxRecvLen)) m_pMarkRecvPtr = m_pMarkRecvPtr - m_uiMaxRecvLen;
}

// add send packet into the buffer, the pTailSendPrt represents from where to add.
CRotativeBuffer::ERotativeBufferResult CRotativeBuffer::addSendBuffer(const char* szBuffer, uint32 uiSendBytes,
																	  char** pTailSendPtr){ // 
	sint32 siSize;

	if(uiSendBytes > m_uiMaxSendLen) return RBR_PACKET_BIG;                   // 
	if(isOverflowSendBuffer(uiSendBytes) == true) return RBR_BUFFER_OVERFLOW; // 

	memcpy(m_pTailSendPtr, szBuffer, (size_t)uiSendBytes);

	//m_acAuthCrypt.encryptSend((uchar8*)(m_pTailSendPtr + m_uiHeaderLen + m_uiMessageIdLen), (size_t)(uiSendBytes - m_uiHeaderLen - m_uiMessageIdLen));

	*pTailSendPtr = m_pTailSendPtr;
	siSize        = (sint32)(m_pTailSendPtr + uiSendBytes - m_pFinalSendPtr);

	if(siSize >= 0){ // 
		memcpy(m_szSendBuffer, &szBuffer[uiSendBytes - siSize], (size_t)siSize);

		m_pTailSendPtr = m_szSendBuffer + siSize;
	}
	else{ // 
		m_pTailSendPtr += uiSendBytes;
	}

	m_uiRemainSendLen += uiSendBytes;
	
	return RBR_OK;
}

bool CRotativeBuffer::isOverflowSendBuffer(uint32 uiSendBytes){ // 
	if((m_uiMaxSendLen - m_uiRemainSendLen) <= uiSendBytes) return true;
	else return false;
}

sint32 CRotativeBuffer::getDiffTailMarkRecvPtr(void){ // 
	sint32 siLen;

	siLen = (sint32)(m_pTailRecvPtr - m_pMarkRecvPtr);
	if(siLen < 0) siLen += m_uiMaxRecvLen; // 

	return siLen;
}

sint32 CRotativeBuffer::getDiffFinalMarkRecvPtr(void){ // 
	return (sint32)(m_pFinalRecvPtr - m_pMarkRecvPtr);
}

sint32 CRotativeBuffer::getDiffFinalHeadSendPtr(void){ // 
	return (sint32)(m_pFinalSendPtr - m_pHeadSendPtr);
}

void CRotativeBuffer::copyOriginFinalRecvBuffer(uint32 uiLen){ // 
	uint32 uiIndex;

	for(uiIndex = 0; uiIndex < uiLen; uiIndex++){
		m_szRecvBuffer[m_uiMaxRecvLen + uiIndex] = m_szRecvBuffer[uiIndex];
	};
}

// I guess this function is used after add message data into the buffer,
// then use this function to add address into the buffer, udp only
CRotativeBuffer::ERotativeBufferResult CRotativeBuffer::addRecvBuffer(CInetAddress* pInetAddress){ // 
	uint32 uiBytes;
	sint32 siSize;

	uiBytes = (uint32)sizeof(sockaddr_in);

	if(uiBytes > m_uiMaxRecvLen) return RBR_PACKET_BIG;                   // 
	if(isOverflowRecvBuffer(uiBytes) == true) return RBR_BUFFER_OVERFLOW; // 

	memcpy(m_pTailRecvPtr, (char*)&pInetAddress->m_saSocketAddress, (size_t)uiBytes);

	siSize = (sint32)(m_pTailRecvPtr + uiBytes - m_pFinalRecvPtr);

	if(siSize >= 0){ //
		memcpy(m_szRecvBuffer, m_pFinalRecvPtr, (size_t)siSize);
		m_pTailRecvPtr = m_szRecvBuffer + siSize;
	}
	else{ // 
		m_pTailRecvPtr += uiBytes;
	};

	m_uiRemainRecvLen -= uiBytes;

	return RBR_OK;
}

uint32 CRotativeBuffer::getMessageLen(char* szMessage){ // 
	uint32 uiMessageLen;

	memcpy(&uiMessageLen, szMessage + m_uiHeaderPos, (size_t)m_uiHeaderLen);

	return uiMessageLen;
}

// I guess it gets message from a tcp packet
CRotativeBuffer::ERotativeBufferResult CRotativeBuffer::getMessage(uint32* uiMessageId, char** szMessage){ // 
	uint32 uiMessageLen;
	sint32 siSize;

	if(uiMessageId == NULL || szMessage == NULL) return RBR_NULL;

	*szMessage = m_pHeadRecvPtr;
	
	siSize = (sint32)(m_pHeadRecvPtr + m_uiHeaderLen + m_uiMessageIdLen - m_pFinalRecvPtr);

	if(siSize >= 0){ // 
		copyOriginFinalRecvBuffer(siSize);
	};

	memcpy(uiMessageId, m_pHeadRecvPtr + m_uiMessageIdPos, (size_t)m_uiMessageIdLen);
	memcpy(&uiMessageLen, m_pHeadRecvPtr + m_uiHeaderPos, (size_t)m_uiHeaderLen);

	if(uiMessageLen <= 0 || uiMessageLen > MAX_PACKET_LEN) return RBR_MSG_LEN;

	siSize = (sint32)(m_pHeadRecvPtr + uiMessageLen - m_pFinalRecvPtr);

	if(siSize >= 0){ // 
		copyOriginFinalRecvBuffer(siSize);

		//m_acAuthCrypt.decryptRecv((uchar8*)(m_pHeadRecvPtr + m_uiHeaderLen + m_uiMessageIdLen), (size_t)(uiMessageLen - m_uiHeaderLen - m_uiMessageIdLen));

		m_pHeadRecvPtr = m_szRecvBuffer + siSize;
	}
	else{ // 
		//m_acAuthCrypt.decryptRecv((uchar8*)(m_pHeadRecvPtr + m_uiHeaderLen + m_uiMessageIdLen), (size_t)(uiMessageLen - m_uiHeaderLen - m_uiMessageIdLen));

		m_pHeadRecvPtr += uiMessageLen;
	};

	m_uiRemainRecvLen += uiMessageLen;

	return RBR_OK;
}

void CRotativeBuffer::delMessage(uint32 uiLen)
{
	sint32 siSize;
	
	siSize = (sint32)(m_pHeadRecvPtr + uiLen - m_pFinalRecvPtr);

	if(siSize >= 0){ // 
		copyOriginFinalRecvBuffer(siSize);

		//m_acAuthCrypt.decryptRecv((uchar8*)(m_pHeadRecvPtr + m_uiHeaderLen + m_uiMessageIdLen), (size_t)(uiMessageLen - m_uiHeaderLen - m_uiMessageIdLen));

		m_pHeadRecvPtr = m_szRecvBuffer + siSize;
	}
	else{ // 
		//m_acAuthCrypt.decryptRecv((uchar8*)(m_pHeadRecvPtr + m_uiHeaderLen + m_uiMessageIdLen), (size_t)(uiMessageLen - m_uiHeaderLen - m_uiMessageIdLen));

		m_pHeadRecvPtr += uiLen;
	};

	m_uiRemainRecvLen += uiLen;
}

// I guess it gets message from a udp packet
CRotativeBuffer::ERotativeBufferResult CRotativeBuffer::getMessage(uint32* uiMessageId, char** szMessage, CInetAddress* pInetAddress){ // 
	uint32 uiMessageLen, uiInetAddressLen;
	sint32 siSize;

	if(uiMessageId == NULL || szMessage == NULL || pInetAddress == NULL) return RBR_NULL;

	*szMessage = m_pHeadRecvPtr;

	// check if  part of the message header is stored at the front of the buffer or not
	siSize = (sint32)(m_pHeadRecvPtr + m_uiHeaderLen + m_uiMessageIdLen - m_pFinalRecvPtr);

	if(siSize >= 0){ // 
		copyOriginFinalRecvBuffer(siSize);
	};

	memcpy(uiMessageId, m_pHeadRecvPtr + m_uiMessageIdPos, (size_t)m_uiMessageIdLen);
	memcpy(&uiMessageLen, m_pHeadRecvPtr + m_uiHeaderPos, (size_t)m_uiHeaderLen);

	if(uiMessageLen <= 0 || uiMessageLen > MAX_PACKET_LEN) return RBR_MSG_LEN;

	// check if  part of the message data and address info is stored at the front of the buffer or not
	uiInetAddressLen = (uint32)sizeof(sockaddr_in);
	siSize           = (sint32)(m_pHeadRecvPtr + uiMessageLen + uiInetAddressLen - m_pFinalRecvPtr);

	if(siSize >= 0){ // 
		copyOriginFinalRecvBuffer(siSize);

		//m_acAuthCrypt.decryptRecv((uchar8*)(m_pHeadRecvPtr + m_uiHeaderLen + m_uiMessageIdLen), (size_t)(uiMessageLen - m_uiHeaderLen - m_uiMessageIdLen));
		pInetAddress->setSocketAddress((sockaddr_in*)(m_pHeadRecvPtr + uiMessageLen));

		m_pHeadRecvPtr = m_szRecvBuffer + siSize;
	}
	else{ // 
		//m_acAuthCrypt.decryptRecv((uchar8*)(m_pHeadRecvPtr + m_uiHeaderLen + m_uiMessageIdLen), (size_t)(uiMessageLen - m_uiHeaderLen - m_uiMessageIdLen));
		pInetAddress->setSocketAddress((sockaddr_in*)(m_pHeadRecvPtr + uiMessageLen));

		m_pHeadRecvPtr = m_pHeadRecvPtr + uiMessageLen + uiInetAddressLen;
	};

	m_uiRemainRecvLen = m_uiRemainRecvLen + uiMessageLen + uiInetAddressLen;

	return RBR_OK;
}

}
