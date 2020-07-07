///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#ifndef __FTP_INTERNET_H
#define __FTP_INTERNET_H

#include "InternetImpl.h"

namespace XNetFrame{

class CFtpInternet{
private:
	uint32 m_uiErrorCode; // 

public:
	HINTERNET m_hFileId; // 
	HINTERNET m_hFindId; // 

	CInternetImpl* m_pInternetImpl; // 

public:
	CFtpInternet(void);
	virtual ~CFtpInternet(void);

	void closeFile(void); // 
	void closeFind(void); // 

	uint32 getErrorCode(void); // 

	CInternetImpl::EInternetImplResult createFolder(TSTRING&);     // 
	CInternetImpl::EInternetImplResult removeFolder(TSTRING&);     // 
	CInternetImpl::EInternetImplResult createDirectory(TSTRING&);  // 
	CInternetImpl::EInternetImplResult setCurrentFolder(TSTRING&); // 
	CInternetImpl::EInternetImplResult getCurrentFolder(TSTRING&); // 

	CInternetImpl::EInternetImplResult deleteFile(TSTRING&);           // 
	CInternetImpl::EInternetImplResult renameFile(TSTRING&, TSTRING&); // 
	CInternetImpl::EInternetImplResult getFileSize(TSTRING&, uint32&); // 
	CInternetImpl::EInternetImplResult openFile(TSTRING&, bool, bool); // 

	CInternetImpl::EInternetImplResult findNextFile(WIN32_FIND_DATA*);            // 
	CInternetImpl::EInternetImplResult findFirstFile(TSTRING&, WIN32_FIND_DATA*); // 
};

}

#endif
