#include "Directory.h"

namespace DnNetworkFramework{

CDirectory::CDirectory(void){
}

CDirectory::~CDirectory(void){
}

TSTRING CDirectory::getCurDir(void){
	TCHAR	szDirectory[MAX_PATH];

	if(::GetCurrentDirectory(MAX_PATH, szDirectory) == 0){
		return _T("");
	}

	return szDirectory;
}

bool CDirectory::setCurDir(TSTRING sDirectory){
	if(::SetCurrentDirectory(sDirectory.c_str()) == FALSE){
		return false;
	}

	return true;
}

bool CDirectory::findFirstFile(TSTRING sDirSpec, HANDLE& hFind, dirent& FindFileData){
	ZeroMemory(&FindFileData, sizeof(dirent));
	hFind = INVALID_HANDLE_VALUE;

	hFind = ::FindFirstFile(sDirSpec.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE){
		return false;
	}

	return true;
}

bool CDirectory::findNextFile(HANDLE& hFind, dirent& FindFileData){
	ZeroMemory(&FindFileData, sizeof(dirent));

	if(hFind == INVALID_HANDLE_VALUE) return false;

	if(::FindNextFile(hFind, &FindFileData) == FALSE){
		return false;
	}

	return true;
}

bool CDirectory::isDirectory(dirent& FindFileData){
	return ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) && ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) == 0);
}

bool CDirectory::findFirstDir(TSTRING sDirSpec, HANDLE& hFind, dirent& FindFileData){
	hFind = INVALID_HANDLE_VALUE;
	ZeroMemory(&FindFileData, sizeof(dirent));

	if(findFirstFile(sDirSpec, hFind, FindFileData) == false){
		return false;
	}

	while(!isDirectory(FindFileData)){
		if(findNextFile(hFind, FindFileData) == false){
			return false;
		}
	}

	return true;
}

bool CDirectory::findNextDir(HANDLE& hFind, dirent& FindFileData){
	ZeroMemory(&FindFileData, sizeof(dirent));

	if(hFind == INVALID_HANDLE_VALUE) return false;

	do 
	{
		if(findNextFile(hFind, FindFileData) == false){
			return false;
		}
	} while(!isDirectory(FindFileData));

	return true;
}

}