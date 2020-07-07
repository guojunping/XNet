#ifndef	__REG_EDITOR_H
#define	__REG_EDITOR_H

#include "TypesDn.h"

namespace DnNetworkFramework{
	
class CRegEditor{
private:
	DWORD m_dwLastError;
	HKEY	m_hKey;
public:
	enum ERegEditorResult{ RE_SUCCESS, RE_FAILURE, RE_OPEN_FAILURE, RE_CLOSE_FAILURE, RE_QUERY_FAILURE,
												RE_DELETE_FAILURE, RE_SET_FAILURE, RE_CREATE_FAILURE };
public:
	CRegEditor();
	~CRegEditor();

	ERegEditorResult openRegKey(HKEY, LPCTSTR, REGSAM);
	ERegEditorResult deleteRegKey(HKEY, LPCTSTR);
	ERegEditorResult createRegKey(HKEY, LPCTSTR, DWORD, REGSAM);

	ERegEditorResult closeRegKey();
	ERegEditorResult queryRegValue(LPCTSTR, TSTRING&);
	ERegEditorResult deleteRegValue(LPCTSTR);
	ERegEditorResult setRegValue(LPCTSTR, DWORD, TSTRING);

	TSTRING getValue(HKEY, LPCTSTR, LPCTSTR);
	bool			deleteValue(HKEY, LPCTSTR, LPCTSTR);
	bool			setValue(HKEY, LPCTSTR, LPCTSTR, DWORD, TSTRING);
};

}

#endif