///////////////////////////////////////
// °³ ¹ß ÀÚ : ±è°æ¼®
// ¼³    ¸í : º¸Á¤Çü ¹öÆÛ °ü·Ã Å¬·¡½º¿¡ ´ëÇÑ ±¸Çö
///////////////////////////////////////

#include "AdjustableBuffer.h"

namespace XNetFrame{

CAdjustableBuffer::CAdjustableBuffer(void){
	m_uiMaxRecvLen     = BUFFER_LEN;
	m_uiMaxRecvExtLen  = EXT_BUFFER_LEN;

	m_uiRemainRecvLen  = 0;

	m_pHeadRecvPtr     = NULL;
	m_pTailRecvPtr     = NULL;
	m_pMarkRecvPtr     = NULL;

	m_pFinalRecvPtr    = NULL;
	m_pFinalRecvExtPtr = NULL;

	m_uiHeaderPos      = 0;
	m_uiHeaderLen      = sizeof(uint32);

	m_uiMessageIdPos   = m_uiHeaderLen;
	m_uiMessageIdLen   = sizeof(uint32);

	m_szRecvBuffer = new char[m_uiMaxRecvLen + m_uiMaxRecvExtLen];
}

CAdjustableBuffer::~CAdjustableBuffer(void){
	if(m_szRecvBuffer != NULL){
		delete [] m_szRecvBuffer;
		m_szRecvBuffer = NULL;
	}
}

CAdjustableBuffer::EAdjustableBufferResult CAdjustableBuffer::initBuffer(void){ //
	if(m_szRecvBuffer == NULL) m_szRecvBuffer = new char[m_uiMaxRecvLen + m_uiMaxRecvExtLen];
	if(m_szRecvBuffer == NULL) return ABR_NULL;

	memset(m_szRecvBuffer, 0, m_uiMaxRecvLen + m_uiMaxRecvExtLen);

	m_pHeadRecvPtr = m_szRecvBuffer;
	m_pTailRecvPtr = m_szRecvBuffer;
	m_pMarkRecvPtr = m_szRecvBuffer;

	m_pFinalRecvPtr    = m_szRecvBuffer + m_uiMaxRecvLen;
	m_pFinalRecvExtPtr = m_szRecvBuffer + m_uiMaxRecvLen + m_uiMaxRecvExtLen;

	m_uiRemainRecvLen = 0;

	return ABR_OK;
}

CAdjustableBuffer::EAdjustableBufferResult CAdjustableBuffer::addRecvBuffer(char* szBuffer, uint32 uiRecvBytes){ // 
	sint32 siSize;

	if(uiRecvBytes > m_uiMaxRecvLen) return ABR_PACKET_BIG;                   // 
	if(isOverflowRecvBuffer(uiRecvBytes) == true) return ABR_BUFFER_OVERFLOW; // 

	memcpy(m_pTailRecvPtr, szBuffer, (size_t)uiRecvBytes);

	siSize = (sint32)(m_pTailRecvPtr + uiRecvBytes - m_pFinalRecvPtr);

	if(siSize >= 0){ // 
		memcpy(m_szRecvBuffer, &szBuffer[uiRecvBytes - siSize], (size_t)siSize);
		m_pTailRecvPtr = m_szRecvBuffer + siSize;
	}
	else{ // 
		m_pTailRecvPtr += uiRecvBytes;
	};

	m_uiRemainRecvLen += uiRecvBytes;
	
	return ABR_OK;
}

bool CAdjustableBuffer::isOverflowRecvBuffer(uint32 uiRecvBytes){ // 
	if((m_uiMaxRecvLen - m_uiRemainRecvLen) <= uiRecvBytes) return true;
	else return false;
}

char* CAdjustableBuffer::getMarkRecvPtr(void){ // 
	return m_pMarkRecvPtr;
}

void CAdjustableBuffer::forwardMarkRecvPtr(uint32 uiLen){ // 
	m_pMarkRecvPtr += uiLen;

	// if the m_pMarkRecvPtr is bigger than the buffer size, it will point at the front of the buffer.
	if(m_pMarkRecvPtr >= (m_szRecvBuffer + m_uiMaxRecvLen)) m_pMarkRecvPtr = m_pMarkRecvPtr - m_uiMaxRecvLen;
}

sint32 CAdjustableBuffer::getDiffTailMarkRecvPtr(void){ // 
	sint32 siLen;

	siLen = (sint32)(m_pTailRecvPtr - m_pMarkRecvPtr);
	if(siLen < 0) siLen += m_uiMaxRecvLen; // 

	return siLen;
}

sint32 CAdjustableBuffer::getDiffFinalMarkRecvPtr(void){ // 
	return (sint32)(m_pFinalRecvPtr - m_pMarkRecvPtr);
}

void CAdjustableBuffer::copyOriginFinalRecvBuffer(uint32 uiLen){ // 
	uint32 uiIndex;

	for(uiIndex = 0; uiIndex < uiLen; uiIndex++){
		m_szRecvBuffer[m_uiMaxRecvLen + uiIndex] = m_szRecvBuffer[uiIndex];
	};
}

CAdjustableBuffer::EAdjustableBufferResult CAdjustableBuffer::getMessage(uint32* uiMessageId, char** szMessage){ // 
	uint32 uiMessageLen;
	sint32 siSize;

	if(uiMessageId == NULL || szMessage == NULL) 
		return ABR_NULL;

	*szMessage = m_pHeadRecvPtr;
	
	// calculate the size of the message header
	siSize = (sint32)(m_pHeadRecvPtr + m_uiHeaderLen + m_uiMessageIdLen - m_pFinalRecvPtr);

	// the message header has some part stored at the beginning of the buffer
	if(siSize >= 0){ // copy that part to the end of the buffer
		copyOriginFinalRecvBuffer(siSize);
	};

	// get the message id and message length
	memcpy(uiMessageId, m_pHeadRecvPtr + m_uiMessageIdPos, (size_t)m_uiMessageIdLen);
	memcpy(&uiMessageLen, m_pHeadRecvPtr + m_uiHeaderPos, (size_t)m_uiHeaderLen);

	if(uiMessageLen <= 0 || uiMessageLen > MAX_PACKET_LEN) 
		return ABR_MSG_LEN;

	// calculate the size of the message data
	siSize = (sint32)(m_pHeadRecvPtr + uiMessageLen - m_pFinalRecvPtr);

	// the message data has some part stored at the front of the buffer
	if(siSize >= 0){ // 
		copyOriginFinalRecvBuffer(siSize);

		m_pHeadRecvPtr = m_szRecvBuffer + siSize;
	}
	else{ // 
		m_pHeadRecvPtr += uiMessageLen;
	};

	m_uiRemainRecvLen -= uiMessageLen;

	return ABR_OK;
}

}
