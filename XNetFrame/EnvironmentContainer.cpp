///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "EnvironmentContainer.h"

namespace DnNetworkFramework{

CEnvironmentContainer::CEnvironmentContainer(void){
	m_pEnvironment = NULL; // 

	m_siMaxEnvironmentCount       = 0; // 
	m_siCurEnvironmentCount       = 0; // 
	m_siAvailableEnvironmentIndex = 0; // 
}

CEnvironmentContainer::~CEnvironmentContainer(void){
	if(m_pEnvironment != NULL){
		delete [] m_pEnvironment; // 
		m_pEnvironment = NULL;
	}
}

bool CEnvironmentContainer::initEnvironmentContainer(sint32 siMaxEnvironmentCount){ // 
	sint32 siIndex;

	m_siMaxEnvironmentCount = siMaxEnvironmentCount; // 

	m_pEnvironment = new CEnvironment[m_siMaxEnvironmentCount]; // 
	if(m_pEnvironment == NULL) return false;

	m_siCurEnvironmentCount       = 0; // 
	m_siAvailableEnvironmentIndex = 0; // 

	for(siIndex = 0; siIndex < m_siMaxEnvironmentCount; siIndex++){
		m_pEnvironment[siIndex].initEnvironment(siIndex); // 
	};

	return true;
}

void CEnvironmentContainer::clearEnvironmentContainer(void){ // 
	sint32 siIndex;

	if(m_pEnvironment == NULL) return;

	m_siCurEnvironmentCount       = 0; // 
	m_siAvailableEnvironmentIndex = 0; // 

	for(siIndex = 0; siIndex < m_siMaxEnvironmentCount; siIndex++){
		m_pEnvironment[siIndex].clearEnvironment(); // 
	};
}

CEnvironment* CEnvironmentContainer::createEnvironment(void){ // 
	sint32 siIndex;

	synchronized sync(&m_mMonitor);

	if(m_pEnvironment == NULL) return NULL; // 

	if(m_siAvailableEnvironmentIndex >= m_siMaxEnvironmentCount) m_siAvailableEnvironmentIndex = 0;

	for(siIndex = m_siAvailableEnvironmentIndex; siIndex < m_siMaxEnvironmentCount; siIndex++){
		if(m_pEnvironment[siIndex].m_bUsed == false){
			m_siCurEnvironmentCount++; // 

			m_pEnvironment[siIndex].m_bUsed = true; // 

			m_siAvailableEnvironmentIndex = siIndex + 1;

			return &m_pEnvironment[siIndex];
		};
	};

	for(siIndex = 0; siIndex < m_siAvailableEnvironmentIndex; siIndex++){
		if(m_pEnvironment[siIndex].m_bUsed == false){
			m_siCurEnvironmentCount++; // 

			m_pEnvironment[siIndex].m_bUsed = true; // 

			m_siAvailableEnvironmentIndex = siIndex + 1;

			return &m_pEnvironment[siIndex];
		};
	};

	return NULL;
}

void CEnvironmentContainer::destroyEnvironment(sint32 siEnvironmentIndex){ // 
	synchronized sync(&m_mMonitor);

	if(m_pEnvironment == NULL) return;

	if(siEnvironmentIndex < 0 || siEnvironmentIndex >= m_siMaxEnvironmentCount) return; // 
	
	if(m_pEnvironment[siEnvironmentIndex].m_bUsed == true){
		m_siCurEnvironmentCount--; // 

		m_pEnvironment[siEnvironmentIndex].clearEnvironment(); // 
	};
}

}
