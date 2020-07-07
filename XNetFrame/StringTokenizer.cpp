///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "StringTokenizer.h"

namespace XNetFrame{

CStringTokenizer::CStringTokenizer(TSTRING& sStr){
  m_sStr   = sStr;
  m_sDelim = _T(" ");

  m_bFirst = true;

  divide();
}

CStringTokenizer::CStringTokenizer(TSTRING& sStr, TSTRING sDelim){
  m_sStr   = sStr;
  m_sDelim = sDelim;

  m_bFirst = true;

  divide();
}

CStringTokenizer::~CStringTokenizer(void){
}

uint32 CStringTokenizer::countTokens(void){ // 
  return (uint32)m_mTokenList.size();
}

TSTRING CStringTokenizer::nextToken(void){ // 
	TSTRING sStr;

	if(m_bFirst == true){
		m_bFirst = false;

		m_itr = m_mTokenList.begin();
		sStr  = (m_itr == m_mTokenList.end())? _T("") : (*m_itr);
	}
	else{
		if(m_itr == m_mTokenList.end()) sStr = _T("");
		else{
			m_itr++;
			sStr = (m_itr == m_mTokenList.end())? _T("") : (*m_itr);
		};
	};

	return sStr;
}

bool CStringTokenizer::hasMoreTokens(void){ // 
	TokenList::iterator itr;
	
	if(m_bFirst == true){
		itr = m_mTokenList.begin();
		if(itr == m_mTokenList.end()) return false;
	}
	else{
		itr = m_itr;

		if(itr == m_mTokenList.end()) return false;
		else{
			itr++;
			if(itr == m_mTokenList.end()) return false;
		};
	};

	return true;
}

void CStringTokenizer::divide(void){ // 
	sint32 siLength, siStart, siStop;
	
	siLength = (sint32)m_sStr.length();
	siStart  = (sint32)m_sStr.find_first_not_of(m_sDelim, 0); // 
	
	while((siStart >= 0) && (siStart < siLength)){
		siStop = (sint32)m_sStr.find_first_of(m_sDelim, siStart); // 
		if((siStop < 0) || (siStop > siLength)) siStop = siLength;
    
		m_mTokenList.push_back(m_sStr.substr(siStart, siStop - siStart));
		
		siStart = (sint32)m_sStr.find_first_not_of(m_sDelim, siStop + 1);
	};
}

}
