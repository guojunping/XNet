///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "ResourceInterface.h"

namespace XNetFrame{

CResourceInterface::CResourceInterface(void){
	m_sFile = _T("");
}

CResourceInterface::~CResourceInterface(void){
	deleteAll();
}

sint32 CResourceInterface::getCount(void){ // 
	return (sint32)m_mResourceMap.size();
}

void CResourceInterface::deleteAll(void){ // 
	LPTSTR                szResource;
	ResourceMap::iterator itr;

	for(itr = m_mResourceMap.begin(); itr != m_mResourceMap.end(); itr++){
		szResource = (LPTSTR)(*itr).second;
		delete [] szResource;
		szResource = NULL;
	};

	itr = m_mResourceMap.begin();
	if(itr != m_mResourceMap.end()) m_mResourceMap.erase(itr, m_mResourceMap.end());
}

LPTSTR CResourceInterface::getResource(sint32 siIndex){ // 
	LPTSTR                szResource;
	ResourceMap::iterator itr;

	itr = m_mResourceMap.find(siIndex);
	
	if(itr == m_mResourceMap.end()) szResource = NULL;
	else szResource = (LPTSTR)(*itr).second;

	return szResource;
}

CResourceInterface::EResourceInterfaceResult CResourceInterface::setFile(LPTSTR szFile){ // 
	m_sFile = szFile;

	if(m_sFile.empty()) return RIR_NULL;

	return RIR_OK;
}

CResourceInterface::EResourceInterfaceResult CResourceInterface::deleteResource(sint32 siIndex){ // 
	LPTSTR                szResource;
	ResourceMap::iterator itr;

	itr = m_mResourceMap.find(siIndex);
	if(itr == m_mResourceMap.end()) return RIR_NOT_FOUND; // 
	
	szResource = (LPTSTR)(*itr).second;

	m_mResourceMap.erase(siIndex);
	delete [] szResource;
	szResource = NULL;

	return RIR_OK;
}

CResourceInterface::EResourceInterfaceResult CResourceInterface::addResource(sint32 siIndex, LPTSTR szResource){ // 
	ResourceMap::iterator itr;

	if(szResource == NULL) return RIR_NULL;

	itr = m_mResourceMap.find(siIndex);
	if(itr != m_mResourceMap.end()) return RIR_ALREADY_EXIST; // 

	m_mResourceMap[siIndex] = szResource;
	
	return RIR_OK;
}

CResourceInterface::EResourceInterfaceResult CResourceInterface::loadResource(sint32 siIndexLen){ // 
	TCHAR  szBuffer[MAX_BUFFER_LEN + 2], szIndex[MAX_INDEX_LEN + 2];
	sint32 siIndex, siLen;
	TCHAR* szResource;
	FILE*  pFile;

	if(m_sFile.empty()) return RIR_NULL;
	if(siIndexLen > MAX_INDEX_LEN) return RIR_INDEX_OVERFLOW;

	pFile = _tfopen((LPTSTR)m_sFile.c_str(), _T("rb")); // 
	if(pFile == NULL) return RIR_OPEN_FAILURE;

	while(true){
		if(_fgetts(szBuffer, MAX_BUFFER_LEN, pFile) == NULL) break; // 

		siLen = (sint32)_tcslen(szBuffer);
		if(_tcsncmp(&szBuffer[siLen - 2], _T("\r\n"), 2) == 0) szBuffer[siLen - 2] = 0x00; // 

		_tcsncpy(szIndex, szBuffer, siIndexLen);
		szIndex[siIndexLen] = 0x00;

		siIndex = _ttoi(szIndex); // 

		szResource = new TCHAR[MAX_BUFFER_LEN + 2]; // 
		if(szResource == NULL) return RIR_NULL;

#ifdef _UNICODE
		wmemcpy(szResource, szBuffer + siIndexLen, MAX_BUFFER_LEN - siIndexLen);
#else
		memcpy(szResource, szBuffer + siIndexLen, MAX_BUFFER_LEN - siIndexLen);
#endif

		szResource[MAX_BUFFER_LEN - siIndexLen] = 0x00;

		if(addResource(siIndex, szResource) != RIR_OK){ // 
			delete [] szResource;
			szResource = NULL;
			return RIR_LOAD_FAILURE;
		};
	};

	fclose(pFile);

	return RIR_OK;
}

}
