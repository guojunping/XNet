///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __INTERNET_IMPL_H
#define __INTERNET_IMPL_H

#include "TypesDn.h"

namespace XNetFrame{

class CInternetImpl{
public:
	enum EInternetImplResult { IIR_OK = 0, IIR_FAILURE, IIR_NULL, IIR_UNKNOWN_SERVICE };

private:
	bool    m_bConnected;  // 
	uint32  m_uiErrorCode; // 

public:
	HINTERNET m_hFileId;     // 
	HINTERNET m_hSessionId;  // 
	HINTERNET m_hInternetId; // 

public:
	CInternetImpl(void);
	virtual ~CInternetImpl(void);

	void close(void);      // 
	void disconnect(void); // 

	bool   isConnected(void);  // 
	uint32 getErrorCode(void); // 

	EInternetImplResult readFile(void*, uint32&);  // 
	EInternetImplResult writeFile(void*, uint32&); // 

	void resetStatusCallback(void);                   // 
	bool setStatusCallback(INTERNET_STATUS_CALLBACK); // 

	EInternetImplResult open(TSTRING&);                                                  // 
	EInternetImplResult connect(string&, sint32, TCHAR*, TCHAR*, sint32, sint32, DWORD); // ¼¼¼ÇÀ» ¿­±â
};

}

#endif
