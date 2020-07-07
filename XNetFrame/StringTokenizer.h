///////////////////////////////////////
// Author : Hank Lee
// Name : 
///////////////////////////////////////

#ifndef __STRING_TOKENIZER_H
#define __STRING_TOKENIZER_H

#include "TypesDn.h"

namespace XNetFrame{

typedef list<TSTRING> TokenList;

class CStringTokenizer{
private:
	TSTRING m_sStr;   // 
	TSTRING m_sDelim; // 

	TokenList m_mTokenList; // 

	TokenList::iterator m_itr;    // 
	bool                m_bFirst; // 

private:
	void divide(void); // 

public:
	CStringTokenizer(TSTRING&);
	CStringTokenizer(TSTRING&, TSTRING);
	virtual ~CStringTokenizer(void);

	TSTRING nextToken(void);     // 
	uint32  countTokens(void);   // 
	bool    hasMoreTokens(void); // 
};

}

#endif
