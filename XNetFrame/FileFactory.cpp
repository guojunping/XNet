#include "FileFactory.h"

namespace DnNetworkFramework{

CFileInfo::CFileInfo(void){
	m_sFilename = _T("");
}

CFileInfo::~CFileInfo(void){
}

CFileFactory::CFileFactory(void){
	m_bBegin = true;
}

CFileFactory::~CFileFactory(void){
	deleteAll();
}

uint32 CFileFactory::getCount(void){
	return (uint32)m_mFileInfoMap.size();
}

void CFileFactory::deleteAll(void){
	CFileInfo*	pFileInfo;
	FileInfoMap::iterator itr;

	for(itr = m_mFileInfoMap.begin(); itr != m_mFileInfoMap.end(); itr++){
		pFileInfo = (CFileInfo*)(*itr).second;
		delete pFileInfo;
	}

	itr = m_mFileInfoMap.begin();
	if(itr != m_mFileInfoMap.end()) m_mFileInfoMap.erase(itr, m_mFileInfoMap.end());
}

CFileInfo* CFileFactory::getFileInfo(TSTRING sFilename){
	CFileInfo*            pFileInfo;
	FileInfoMap::iterator itr;

	itr = m_mFileInfoMap.find(sFilename);

	if(itr == m_mFileInfoMap.end()) pFileInfo = NULL;
	else pFileInfo = (CFileInfo*)(*itr).second;

	return pFileInfo;
}

bool CFileFactory::deleteFileInfo(TSTRING sFilename){
	CFileInfo*            pFileInfo;
	FileInfoMap::iterator itr;

	itr = m_mFileInfoMap.find(sFilename);
	if(itr == m_mFileInfoMap.end()) return false; // 

	pFileInfo = (CFileInfo*)(*itr).second;

	m_mFileInfoMap.erase(sFilename);
	delete pFileInfo;

	return true;
}

bool CFileFactory::addFileInfo(CFileInfo* pFileInfo){
	FileInfoMap::iterator itr;

	if(pFileInfo == NULL) return false;

	itr = m_mFileInfoMap.find(pFileInfo->m_sFilename);
	if(itr != m_mFileInfoMap.end()) return false; // 

	m_mFileInfoMap[pFileInfo->m_sFilename] = pFileInfo;

	return true;
}

bool CFileFactory::loadFilesByExtension(TSTRING sCurDirectory, TSTRING sExtension, bool bRecursive){
	TSTRING sDirSpec;

	if(_tcscmp(sCurDirectory.c_str(), _T("")) == 0) return false;

	if(bRecursive){
		HANDLE hFind;
		dirent	FindFileData;

		sDirSpec = sCurDirectory + _T("\\*.*");

		CDirectory::findFirstDir(sDirSpec, hFind, FindFileData);

		while(true){
			if(_tcscmp(FindFileData.cFileName, _T(".")) != 0 && _tcscmp(FindFileData.cFileName, _T("..")) != 0){
				loadFilesByExtension(sCurDirectory + _T("\\") + FindFileData.cFileName, sExtension, true);
			}

			if(CDirectory::findNextDir(hFind, FindFileData) == false){
				break;
			}
		}

		::FindClose(hFind);
	}

	loadFilesByExtensionInDir(sCurDirectory, sExtension);

	return true;
}

bool CFileFactory::loadFilesByExtensionInDir(TSTRING sDirectory, TSTRING sExtension){
	TSTRING sDirSpec;
	HANDLE hFind;
	dirent	FindFileData;

	sDirSpec = sDirectory + _T("\\*.") + sExtension;

	if(CDirectory::findFirstFile(sDirSpec, hFind, FindFileData) == false){
		::FindClose(hFind);
		return false;
	}

	while(true){
		CFileInfo*	pFileInfo = new CFileInfo;

		pFileInfo->m_sFilename = sDirectory + _T("\\") + FindFileData.cFileName;

		if(addFileInfo(pFileInfo) == false){
			delete pFileInfo;
			::FindClose(hFind);
			return false;
		}

		if(CDirectory::findNextFile(hFind, FindFileData) == false){
			::FindClose(hFind);
			return false;
		}
	}

	::FindClose(hFind);

	return true;
}

CFileInfo* CFileFactory::nextElement(){
	CFileInfo* pFileInfo = NULL;

	if(m_bBegin){
		m_bBegin = false;
		m_itrFileInfo = m_mFileInfoMap.begin();
	}

	if(m_itrFileInfo != m_mFileInfoMap.end()){
		pFileInfo = (CFileInfo*)(*m_itrFileInfo).second;

		m_itrFileInfo++;
	}
	else{
		m_bBegin = true;
	}

	return pFileInfo;
}

}