/**
 * exception
 * 
 */

#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include "TypesDn.h"

namespace XNetFrame{

class CException{
protected:
	string m_sMessage;

public:
	CException(void);
	CException(const char*);
	virtual ~CException(void);

	virtual string getMessage(void);
};

class COutOfMemoryException : public CException{
public:
	COutOfMemoryException(void);
	COutOfMemoryException(const char*);
	~COutOfMemoryException(void);

	string getMessage(void);
};

class CUnknownHostException : public CException{
public:
	CUnknownHostException(void);
	CUnknownHostException(const char*);
	~CUnknownHostException(void);

	string getMessage(void);
};

class CEventException : public CException{
private:
	sint32 m_siErrorCode;

public:
	CEventException(void);
	CEventException(const char*, sint32);
	~CEventException(void);

	string getMessage(void);
};

}

#endif
