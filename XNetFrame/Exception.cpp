/**
 * exception
 * 
 */

#include "Exception.h"
#include <stdio.h>

namespace XNetFrame{

CException::CException(void){
}

CException::CException(const char* szMessage){
	m_sMessage = szMessage;
}

CException::~CException(void){
}

string CException::getMessage(void){
	return m_sMessage + " : General";
}

COutOfMemoryException::COutOfMemoryException(void) : CException(){
}

COutOfMemoryException::COutOfMemoryException(const char* szMessage) : CException(szMessage){
}

COutOfMemoryException::~COutOfMemoryException(void){
}

string COutOfMemoryException::getMessage(void){
	return m_sMessage + " : Out Of Memory";
}

CUnknownHostException::CUnknownHostException(void) : CException(){
}

CUnknownHostException::CUnknownHostException(const char* szMessage) : CException(szMessage){
}

CUnknownHostException::~CUnknownHostException(void){
}

string CUnknownHostException::getMessage(void){
	return m_sMessage + " : Unknown Host";
}

CEventException::CEventException(void) : CException(){
}

CEventException::CEventException(const char* szMessage, sint32 siErrorCode) : CException(szMessage){
	m_siErrorCode = siErrorCode;
}

CEventException::~CEventException(void){
}

string CEventException::getMessage(void){
	char   szSubMessage[32];
	string sSubMessage;

	sprintf(szSubMessage, " : (%d)", m_siErrorCode);
	sSubMessage = szSubMessage;

	return m_sMessage + sSubMessage;
}

}
