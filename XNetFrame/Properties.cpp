///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "Properties.h"
#include "Loader.h"
#include <string.h>
#include <stdio.h>

namespace XNetFrame{

static char DEFAULT_CLASS[] = "Default";
	
CProperties::CProperties(void){
}

CProperties::~CProperties(void){
	deleteAll();
}

uint32 CProperties::countLoader(void){ // 
	return (uint32)m_liLoaders.size();
}

CLoader* CProperties::getLoader(LPTSTR szLoaderName){ // 
	LoaderList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	if(szLoaderName == NULL || szLoaderName[0] == 0x00) return NULL;

	for(itr = m_liLoaders.begin(); itr != m_liLoaders.end(); itr++){
		if(strcmp((*itr)->m_sName.c_str(), szLoaderName) == 0) return *itr;
	};

	return NULL;
}

CProperties::EPropertiesResult CProperties::addLoader(CLoader* pLoader){ // 
	LoaderList::iterator itr;
	
	synchronized sync(&m_cMonitor);
	
	if(pLoader == NULL) return PR_NULL;

	itr = find(m_liLoaders.begin(), m_liLoaders.end(), pLoader);

	if(itr == m_liLoaders.end()){
		pLoader->m_pProperties = this; // 
		m_liLoaders.push_back(pLoader);

		return PR_OK;
	};

	return PR_ALREADY_EXIST;
}

CProperties::EPropertiesResult CProperties::deleteLoader(CLoader* pLoader){ // 
	LoaderList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	if(pLoader == NULL) return PR_NULL;

	itr = find(m_liLoaders.begin(), m_liLoaders.end(), pLoader);

	if(itr != m_liLoaders.end()){
		pLoader->m_pProperties = NULL; // 
		m_liLoaders.erase(itr);

		return PR_OK;
	};

	return PR_NOT_FOUND;
}

CProperties::EPropertiesResult CProperties::deleteLoader(LPTSTR szLoaderName){ // 
	LoaderList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	if(szLoaderName == NULL || szLoaderName[0] == 0x00) return PR_NULL;

	for(itr = m_liLoaders.begin(); itr != m_liLoaders.end();){
		if(strcmp((*itr)->m_sName.c_str(), szLoaderName) == 0){
			(*itr)->m_pProperties = NULL; //
			m_liLoaders.erase(itr);

			return PR_OK;
		}
		else itr++;
	};

	return PR_NOT_FOUND;
}

CProperties::EPropertiesResult CProperties::isConnected(CLoader* pLoader){ // 
	LoaderList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	if(pLoader == NULL) return PR_NULL;

	itr = find(m_liLoaders.begin(), m_liLoaders.end(), pLoader);
	if(itr != m_liLoaders.end()) return PR_OK;

	return PR_NOT_CONNECTED;
}

CProperties::EPropertiesResult CProperties::isConnected(LPTSTR szLoaderName){ // 
	LoaderList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	if(szLoaderName == NULL || szLoaderName[0] == 0x00) return PR_NULL;

	for(itr = m_liLoaders.begin(); itr != m_liLoaders.end(); itr++){
		if(strcmp((*itr)->m_sName.c_str(), szLoaderName) == 0) return PR_OK;
	};

	return PR_NOT_CONNECTED;
}

uint32 CProperties::countProperty(void){ // 
	return (uint32)m_mProperties.size();
}

CProperties::EPropertiesResult CProperties::addProperty(LPTSTR szLoader, LPTSTR szProperty, sint32 siValue){ // 
	return addProperty(szLoader, DEFAULT_CLASS, szProperty, siValue);
}

CProperties::EPropertiesResult CProperties::addProperty(LPTSTR szLoader, LPTSTR szClass, LPTSTR szProperty, sint32 siValue){ // 
	string sLoader, sClass, sProperty, sKey, sValue;
	char   szValue[64];

	if(szLoader == NULL || szClass == NULL || szProperty == NULL) return PR_NULL;

	sClass    = szClass;
	sProperty = szProperty;
	sKey      = sClass + ("|") + sProperty;

	sprintf(szValue, ("%d"), siValue);
	sValue = szValue;

	m_mProperties[sKey] = sValue;

	sLoader = szLoader;

	m_mLoaders[sKey] = sLoader;

	return PR_OK;
}

CProperties::EPropertiesResult CProperties::addProperty(LPTSTR szLoader, LPTSTR szProperty, float fValue){ // 
	return addProperty(szLoader, DEFAULT_CLASS, szProperty, fValue);
}

CProperties::EPropertiesResult CProperties::addProperty(LPTSTR szLoader, LPTSTR szClass, LPTSTR szProperty, float fValue){ // 
	string sLoader, sClass, sProperty, sKey, sValue;
	char   szValue[128];

	if(szLoader == NULL || szClass == NULL || szProperty == NULL) return PR_NULL;

	sClass    = szClass;
	sProperty = szProperty;
	sKey      = sClass + ("|") + sProperty;

	sprintf(szValue, ("%f"), fValue);
	sValue = szValue;

	m_mProperties[sKey] = sValue;

	sLoader = szLoader;

	m_mLoaders[sKey] = sLoader;

	return PR_OK;
}

CProperties::EPropertiesResult CProperties::addProperty(LPTSTR szLoader, LPTSTR szProperty, LPTSTR szValue){ // 
	return addProperty(szLoader, DEFAULT_CLASS, szProperty, szValue);
}

CProperties::EPropertiesResult CProperties::addProperty(LPTSTR szLoader, LPTSTR szClass, LPTSTR szProperty, LPTSTR szValue){ // 
	string sLoader, sClass, sProperty, sKey, sValue;

	if(szLoader == NULL || szClass == NULL || szProperty == NULL || szValue == NULL) return PR_NULL;

	sClass    = szClass;
	sProperty = szProperty;
	sKey      = sClass + ("|") + sProperty;
	sValue    = szValue;

	m_mProperties[sKey] = sValue;

	sLoader = szLoader;

	m_mLoaders[sKey] = sLoader;

	return PR_OK;
}

sint32 CProperties::getIntProperty(LPTSTR szClass, LPTSTR szProperty){ // 
	string               sClass, sProperty, sKey, sValue;
	PropertyMap::iterator itr;

	sValue = ("");
	
	if(szClass != NULL && szProperty != NULL){
		sClass    = szClass;
		sProperty = szProperty;
		sKey      = sClass + ("|") + sProperty;

		itr = m_mProperties.find(sKey);
		if(itr != m_mProperties.end()) sValue = (*itr).second;
	};

	return (sint32)atoi(sValue.c_str());
}

sint32 CProperties::getIntProperty(LPTSTR szProperty){ // 
	return getIntProperty(DEFAULT_CLASS, szProperty);
}

float CProperties::getFloatProperty(LPTSTR szClass, LPTSTR szProperty){ // 
	string               sClass, sProperty, sKey, sValue;
	PropertyMap::iterator itr;

	sValue = ("");
	
	if(szClass != NULL && szProperty != NULL){
		sClass    = szClass;
		sProperty = szProperty;
		sKey      = sClass + ("|") + sProperty;

		itr = m_mProperties.find(sKey);
		if(itr != m_mProperties.end()) sValue = (*itr).second;
	};

	return (float)atof(sValue.c_str());
}

float CProperties::getFloatProperty(LPTSTR szProperty){ // 
	return getFloatProperty(DEFAULT_CLASS, szProperty);
}

string CProperties::getStringProperty(LPTSTR szClass, LPTSTR szProperty){ // 
	string               sClass, sProperty, sKey, sValue;
	PropertyMap::iterator itr;

	sValue = ("");
	
	if(szClass != NULL && szProperty != NULL){
		sClass    = szClass;
		sProperty = szProperty;
		sKey      = sClass + ("|") + sProperty;

		itr = m_mProperties.find(sKey);
		if(itr != m_mProperties.end()) sValue = (*itr).second;
	};

	return sValue;
}

string CProperties::getStringProperty(LPTSTR szProperty){ // 
	return getStringProperty(DEFAULT_CLASS, szProperty);
}

void CProperties::deleteProperty(LPTSTR szProperty){ // 
	deleteProperty(DEFAULT_CLASS, szProperty);
}

void CProperties::deleteProperty(LPTSTR szClass, LPTSTR szProperty){ // 
	string sClass, sProperty, sKey;

	if(szClass != NULL && szProperty != NULL){
		sClass    = szClass;
		sProperty = szProperty;
		sKey      = sClass + ("|") + sProperty;

		m_mProperties.erase(sKey);
		m_mLoaders.erase(sKey);
	};
}

bool CProperties::isProperty(LPTSTR szProperty){ // 
	return isProperty(DEFAULT_CLASS, szProperty);
}

bool CProperties::isProperty(LPTSTR szClass, LPTSTR szProperty){ // 
	string               sClass, sProperty, sKey;
	PropertyMap::iterator itr;

	if(szClass != NULL && szProperty != NULL){
		sClass    = szClass;
		sProperty = szProperty;
		sKey      = sClass + ("|") + sProperty;

		itr = m_mProperties.find(sKey);
		if(itr != m_mProperties.end()) return true;
	};

	return false;
}

void CProperties::deleteAll(void){ // 
	CLoader*              pLoader;
	LoaderList::iterator  itr1;
	PropertyMap::iterator itr2;
	LoaderMap::iterator   itr3;

	synchronized sync(&m_cMonitor);

	for(itr1 = m_liLoaders.begin(); itr1 != m_liLoaders.end(); itr1++){
		pLoader = (CLoader*)(*itr1);
		delete pLoader;
		pLoader = NULL;
	};

	itr1 = m_liLoaders.begin();
	if(itr1 != m_liLoaders.end()) m_liLoaders.erase(itr1, m_liLoaders.end());
	
	itr2 = m_mProperties.begin();
	if(itr2 != m_mProperties.end()) m_mProperties.erase(itr2, m_mProperties.end());

	itr3 = m_mLoaders.begin();
	if(itr3 != m_mLoaders.end()) m_mLoaders.erase(itr3, m_mLoaders.end());
}

CProperties::EPropertiesResult CProperties::loadPropertis(void){  // 
	CLoader*             pLoader;
	LoaderList::iterator itr;

	synchronized sync(&m_cMonitor);
	
	for(itr = m_liLoaders.begin(); itr != m_liLoaders.end(); itr++){
		pLoader = (CLoader*)(*itr);
		if(pLoader->loadProperties() != CLoader::LR_OK) return PR_FAILURE;
	};

	return PR_OK;
}

CProperties::EPropertiesResult CProperties::addPropertyEx(LPTSTR szLoader, LPTSTR szClass, LPTSTR szProperty, sint32 siValue){ // 
	string  sLoader, sClass, sProperty, sKey, sValue;
	char    szValue[128];
	CLoader* pLoader;

	if(szLoader == NULL || szClass == NULL || szProperty == NULL) return PR_NULL;

	sClass    = szClass;
	sProperty = szProperty;
	sKey      = sClass + ("|") + sProperty;

	sprintf(szValue, ("%d"), siValue);
	sValue = szValue;

	sLoader = szLoader;

	pLoader = getLoader((LPTSTR)sLoader.c_str());
	if(pLoader == NULL) return PR_NULL;

	if(pLoader->setProperty(sClass, sProperty, sValue) != CLoader::LR_OK) return PR_FAILURE;

	m_mProperties[sKey] = sValue;
	m_mLoaders[sKey]    = sLoader;

	return PR_OK;
}

CProperties::EPropertiesResult CProperties::addPropertyEx(LPTSTR szLoader, LPTSTR szClass, LPTSTR szProperty, float fValue){ // 
	string  sLoader, sClass, sProperty, sKey, sValue;
	char    szValue[128];
	CLoader* pLoader;

	if(szLoader == NULL || szClass == NULL || szProperty == NULL) return PR_NULL;

	sClass    = szClass;
	sProperty = szProperty;
	sKey      = sClass + ("|") + sProperty;

	sprintf(szValue, ("%f"), fValue);
	sValue = szValue;

	sLoader = szLoader;

	pLoader = getLoader((LPTSTR)sLoader.c_str());
	if(pLoader == NULL) return PR_NULL;

	if(pLoader->setProperty(sClass, sProperty, sValue) != CLoader::LR_OK) return PR_FAILURE;

	m_mProperties[sKey] = sValue;
	m_mLoaders[sKey]    = sLoader;

	return PR_OK;
}

CProperties::EPropertiesResult CProperties::addPropertyEx(LPTSTR szLoader, LPTSTR szClass, LPTSTR szProperty, LPTSTR szValue){ // 
	string  sLoader, sClass, sProperty, sKey, sValue;
	CLoader* pLoader;

	if(szLoader == NULL || szClass == NULL || szProperty == NULL || szValue == NULL) return PR_NULL;

	sClass    = szClass;
	sProperty = szProperty;
	sKey      = sClass + ("|") + sProperty;
	sValue    = szValue;

	sLoader = szLoader;

	pLoader = getLoader((LPTSTR)sLoader.c_str());
	if(pLoader == NULL) return PR_NULL;

	if(pLoader->setProperty(sClass, sProperty, sValue) != CLoader::LR_OK) return PR_FAILURE;

	m_mProperties[sKey] = sValue;
	m_mLoaders[sKey]    = sLoader;

	return PR_OK;
}

CProperties::EPropertiesResult CProperties::deletePropertyEx(LPTSTR szLoader, LPTSTR szClass, LPTSTR szProperty){ // 
	string  sLoader, sClass, sProperty, sKey;
	CLoader* pLoader;

	if(szLoader == NULL || szClass == NULL || szProperty == NULL) return PR_NULL;

	sClass    = szClass;
	sProperty = szProperty;
	sKey      = sClass + ("|") + sProperty;

	sLoader = szLoader;

	pLoader = getLoader((LPTSTR)sLoader.c_str());
	if(pLoader == NULL) return PR_NULL;

	if(pLoader->resetProperty(sClass, sProperty) != CLoader::LR_OK) return PR_FAILURE;

	m_mProperties.erase(sKey);
	m_mLoaders.erase(sKey);

	return PR_OK;
}

}
