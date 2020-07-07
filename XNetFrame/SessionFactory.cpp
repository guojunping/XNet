///////////////////////////////////////
// °³ ¹ß ÀÚ : ±è°æ¼®
// ¼³    ¸í : ¼¼¼Ç ÆÑÅä¸® °ü·Ã Å¬·¡½º¿¡ ´ëÇÑ ±¸Çö
///////////////////////////////////////

#include "SessionFactory.h"

namespace XNetFrame{

CSessionFactory::CSessionFactory(void){
}

CSessionFactory::~CSessionFactory(void){
	deleteAll();
}

uint32 CSessionFactory::getCount(void){ // ÇöÀç µî·ÏµÈ ¼¼¼Ç °´Ã¼¼ö¸¦ ¸®ÅÏ
	return (uint32)m_mSessionMap.size();
}

uint32 CSessionFactory::getAvailableCount(void){ // »ç¿ë °¡´ÉÇÑ ¼¼¼Ç °´Ã¼¼ö¸¦ ¸®ÅÏ

	synchronized sync(&m_cMonitor);
	return (uint32)m_lSessionUnusedList.size();
}

uint32 CSessionFactory::getUsedCount(void){ // »ç¿ëÁßÀÎ ¼¼¼Ç °´Ã¼¼ö¸¦ ¸®ÅÏ
	return m_uiUsedCount;
}

void CSessionFactory::deleteAll(void){ // ¼¼¼Ç °´Ã¼¸¦ ¸ðµÎ »èÁ¦(¼¼¼Ç °´Ã¼ ÀÚÃ¼µµ Ãß°¡·Î ¸ðµÎ ÆÄ±«)
	CSession*  pSession;
	SessionMap::iterator  mitr;
	SessionList::iterator litr;

	synchronized sync(&m_cMonitor);

	for(mitr = m_mSessionMap.begin(); mitr != m_mSessionMap.end(); ++mitr){
		pSession = (CSession*)(*mitr).second;
		delete pSession;
		pSession = NULL;
	}

	mitr = m_mSessionMap.begin();
	if(mitr != m_mSessionMap.end()) m_mSessionMap.erase(mitr, m_mSessionMap.end());
	
	litr = m_lSessionUnusedList.begin();
	if(litr != m_lSessionUnusedList.end()) m_lSessionUnusedList.erase(litr, m_lSessionUnusedList.end());
	
}

void CSessionFactory::closeAll(void){ // ¸ðµç ¼¼¼Ç °´Ã¼¿¡ ´ëÇØ doClose ÇÔ¼ö¸¦ È£Ãâ
	CSession*  pSession;
	SessionMap::iterator  mitr;

	synchronized sync(&m_cMonitor);

	for(mitr = m_mSessionMap.begin(); mitr != m_mSessionMap.end(); ++mitr){
		pSession = (CSession*)(*mitr).second;
		pSession->realClose(CSession::CS_SERVICE_STOP);
	};
}

CSession* CSessionFactory::getSession(uint32 uiSessionId){ // ¼¼¼Ç ID¿¡ ºÎÇÕÇÏ´Â ¼¼¼Ç °´Ã¼¸¦ ¸®ÅÏ
	CSession*  pSession;
	SessionMap::iterator itr;

	synchronized sync(&m_cMonitor);

	itr = m_mSessionMap.find(uiSessionId);
	
	if(itr == m_mSessionMap.end()) pSession = NULL;
	else pSession = (CSession*)(*itr).second;

	return pSession;
}


CSession* CSessionFactory::getAvailableSession(void){ // »ç¿ë °¡´ÉÇÑ ¼¼¼Ç °´Ã¼¸¦ ¸®ÅÏ

	CSession* pSession;
	synchronized sync(&m_cMonitor);

	if(!m_lSessionUnusedList.empty())
	{
		SessionList::iterator itr = m_lSessionUnusedList.begin();
		pSession=*itr;
		m_lSessionUnusedList.erase(itr);
		m_uiUsedCount++;
		
		return pSession;
	}

	return NULL;
}

bool CSessionFactory::putAuailableSession(CSession* pSession)
{
	if(pSession==NULL)
		return false;
		
	synchronized sync(&m_cMonitor);
	m_lSessionUnusedList.push_back(pSession);
	m_uiUsedCount--;
	
	return true;
}

bool CSessionFactory::addSession(CSession* pSession){ // ¼¼¼Ç °´Ã¼¸¦ Ãß°¡(¼¼¼Ç ID´Â À¯ÀÏÇØ¾ß ÇÔ)
	uint32  uiSessionId;
	SessionMap::iterator itr;

	synchronized sync(&m_cMonitor);

	if(pSession == NULL) return false;

	uiSessionId = pSession->getSessionId();

	itr = m_mSessionMap.find(uiSessionId);
	if(itr != m_mSessionMap.end()) return false; // ÀÌ¹Ì ¼¼¼Ç °´Ã¼°¡ Á¸ÀçÇÏ´Â °æ¿ì

	m_mSessionMap[uiSessionId] = pSession;
	m_lSessionUnusedList.push_back(pSession);
	
	return true;
}

bool CSessionFactory::deleteSession(CSession* pSession){ // ¼¼¼Ç °´Ã¼¸¦ »èÁ¦(¼¼¼Ç °´Ã¼ ÀÚÃ¼µµ Ãß°¡·Î ÆÄ±«)
	uint32 uiSessionId;
	
	SessionMap::iterator  mitr;
	SessionList::iterator litr;
	
	synchronized sync(&m_cMonitor);

	if(pSession == NULL) return false;

	uiSessionId = pSession->getSessionId();

	mitr = m_mSessionMap.find(uiSessionId);
	if(mitr == m_mSessionMap.end()) return false; // ¼¼¼Ç °´Ã¼°¡ Á¸ÀçÇÏÁö ¾Ê´Â °æ¿ì

	m_mSessionMap.erase(uiSessionId);
	
	m_uiUsedCount--;
	
	for(litr=m_lSessionUnusedList.begin();litr!=m_lSessionUnusedList.end();++litr)
		if(*litr==pSession)
		{
			m_uiUsedCount++;
			m_lSessionUnusedList.erase(litr);
			break;
		}
	
	delete pSession;
	pSession = NULL;
	
	return true;
}

}
