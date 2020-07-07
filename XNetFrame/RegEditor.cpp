#include "RegEditor.h"

namespace DnNetworkFramework{

CRegEditor::CRegEditor(){
	m_dwLastError = 0;
	m_hKey = NULL;
}

CRegEditor::~CRegEditor(){
	closeRegKey();
}

CRegEditor::ERegEditorResult CRegEditor::openRegKey(HKEY hKey, LPCTSTR lpSubKey, REGSAM rsAccessMask){
	LONG  lResult;

	lResult = ::RegOpenKeyEx(hKey, lpSubKey, 0, rsAccessMask, &m_hKey);
	if(lResult != ERROR_SUCCESS){
		m_dwLastError = ::GetLastError();
		return RE_OPEN_FAILURE;
	}

	return RE_SUCCESS;
}

CRegEditor::ERegEditorResult CRegEditor::deleteRegKey(HKEY hKey, LPCTSTR lpSubKey){
	LONG lResult;

	lResult = ::RegDeleteKey(hKey, lpSubKey);
	if(lResult != ERROR_SUCCESS){
		m_dwLastError = ::GetLastError();
		return RE_DELETE_FAILURE;
	}

	return RE_SUCCESS;
}

CRegEditor::ERegEditorResult CRegEditor::createRegKey(HKEY hKey, LPCTSTR lpSubKey, DWORD dwOptions, REGSAM rsAccessMask){
	LONG lResult;
	DWORD dwDisp;

	lResult = ::RegCreateKeyEx(hKey, lpSubKey, 0, NULL, dwOptions, rsAccessMask, NULL, &m_hKey, &dwDisp);
	if(lResult != ERROR_SUCCESS){
		m_dwLastError = ::GetLastError();
		return RE_CREATE_FAILURE;
	}

	return RE_SUCCESS;
}

CRegEditor::ERegEditorResult CRegEditor::closeRegKey(){
	LONG  lResult;

	lResult = ::RegCloseKey(m_hKey);
	if(lResult != ERROR_SUCCESS){
		m_dwLastError = ::GetLastError();
		return RE_CLOSE_FAILURE;
	}

	return RE_SUCCESS;
}

CRegEditor::ERegEditorResult CRegEditor::queryRegValue(LPCTSTR lpValueName, TSTRING& sValue){
	LONG lResult;
	TCHAR	szValue[1024];
	DWORD	dwBufferLen = 1024; 
	
	lResult = ::RegQueryValueEx(m_hKey, lpValueName, 0, NULL, (LPBYTE)&szValue, &dwBufferLen);
	if(lResult != ERROR_SUCCESS){
		m_dwLastError = ::GetLastError();
		return RE_QUERY_FAILURE;
	}

	sValue = (TSTRING)szValue;

	return RE_SUCCESS;
}

CRegEditor::ERegEditorResult CRegEditor::deleteRegValue(LPCTSTR lpValueName){
	LONG lResult;

	lResult = ::RegDeleteValue(m_hKey, lpValueName);
	if(lResult != ERROR_SUCCESS){
		m_dwLastError = ::GetLastError();
		return RE_DELETE_FAILURE;
	}

	return RE_SUCCESS;
}

CRegEditor::ERegEditorResult CRegEditor::setRegValue(LPCTSTR lpValueName, DWORD dwType, TSTRING sValue){
	LONG lResult;

	lResult = ::RegSetValueEx(m_hKey, lpValueName, 0, dwType, (const BYTE*)sValue.c_str(), (DWORD)sValue.length());
	if(lResult != ERROR_SUCCESS){
		m_dwLastError = ::GetLastError();
		return RE_SET_FAILURE;
	}

	return RE_SUCCESS;
}

TSTRING CRegEditor::getValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName){
	TSTRING sValue;

	if(openRegKey(hKey, lpSubKey, KEY_QUERY_VALUE) != RE_SUCCESS) sValue = _T("");
	if(queryRegValue(lpValueName, sValue) != RE_SUCCESS) sValue = _T("");
	if(closeRegKey() != RE_SUCCESS) sValue = _T("");

	return sValue;
}

bool CRegEditor::deleteValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName){

	if(openRegKey(hKey, lpSubKey, KEY_SET_VALUE) != RE_SUCCESS) return false;
	if(deleteRegValue(lpValueName) != RE_SUCCESS) return false;

	return true;
}

bool CRegEditor::setValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwType, TSTRING sValue){

	if(openRegKey(hKey, lpSubKey, KEY_SET_VALUE) != RE_SUCCESS) return false;
	if(setRegValue(lpValueName, dwType, sValue) != RE_SUCCESS) return false;

	return true;
}

}