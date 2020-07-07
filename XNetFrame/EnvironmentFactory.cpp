///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "EnvironmentFactory.h"

namespace DnNetworkFramework{

CEnvironment::CEnvironment(void){
}

CEnvironment::~CEnvironment(void){
}

void CEnvironment::initEnvironment(sint32 siIndex){ // 
	m_siIndex = siIndex; // 

	m_bUsed = false; // 
}

void CEnvironment::clearEnvironment(void){ // 
	m_bUsed = false; // 
}

CEnvironmentFactory::CEnvironmentFactory(void){
	m_siFileCount     = 0; // 
	m_siFileLength    = 0; // 
	m_siCurFileCount  = 0; // 
	m_siCurFileLength = 0; // 

	m_pEnvironmentContainer = NULL; // 
}

CEnvironmentFactory::~CEnvironmentFactory(void){
	deleteAll(); // 
}

bool CEnvironmentFactory::setEnvironmentContainer(CEnvironmentContainer* pEnvironmentContainer){ // 
	if(pEnvironmentContainer == NULL) return false;

	m_pEnvironmentContainer = pEnvironmentContainer; // 

	if(m_pEnvironmentContainer->initEnvironmentContainer(4096) == false){ // 
		return false;
	};

	return true;
}

bool CEnvironmentFactory::deleteAll(void){ // 
	CEnvironment*            pEnvironment;
	EnvironmentMap::iterator itr;

	synchronized sync(&m_mMonitor);

	if(m_pEnvironmentContainer == NULL) return false; // 

	m_siFileCount     = 0; // 
	m_siFileLength    = 0; // 
	m_siCurFileCount  = 0; // 
	m_siCurFileLength = 0; // 

	for(itr = m_mEnvironmentMap.begin(); itr != m_mEnvironmentMap.end(); itr++){
		pEnvironment = (CEnvironment*)(*itr).second;
		m_pEnvironmentContainer->destroyEnvironment(pEnvironment->m_siIndex);
	};

	itr = m_mEnvironmentMap.begin();
	if(itr != m_mEnvironmentMap.end()) m_mEnvironmentMap.erase(itr, m_mEnvironmentMap.end());

	return true;
}

CEnvironment* CEnvironmentFactory::getEnvironment(sint32 siIndex){ // 
	CEnvironment*            pEnvironment;
	EnvironmentMap::iterator itr;

	itr = m_mEnvironmentMap.find(siIndex);
	
	if(itr == m_mEnvironmentMap.end()) pEnvironment = NULL;
	else pEnvironment = (CEnvironment*)(*itr).second;

	return pEnvironment;
}

bool CEnvironmentFactory::addEnvironment(CEnvironment* pEnvironment){ // 
	EnvironmentMap::iterator itr;

	synchronized sync(&m_mMonitor);

	if(pEnvironment == NULL) return false;

	itr = m_mEnvironmentMap.find(pEnvironment->m_siIndex);
	if(itr != m_mEnvironmentMap.end()) return false; // 

	m_siFileCount++;                            // 
	m_siFileLength += pEnvironment->m_siLength; // 

	m_mEnvironmentMap[pEnvironment->m_siIndex] = pEnvironment;
	
	return true;
}

bool CEnvironmentFactory::deleteEnvironment(sint32 siIndex){ // 
	CEnvironment*            pEnvironment;
	EnvironmentMap::iterator itr;

	synchronized sync(&m_mMonitor);

	if(m_pEnvironmentContainer == NULL) return false; // 

	itr = m_mEnvironmentMap.find(siIndex);
	if(itr == m_mEnvironmentMap.end()) return false; // 
	
	pEnvironment = (CEnvironment*)(*itr).second;

	m_siFileCount--;                            // 
	m_siFileLength -= pEnvironment->m_siLength; // 

	m_mEnvironmentMap.erase(siIndex);
	m_pEnvironmentContainer->destroyEnvironment(siIndex);

	return true;
}

}
