/**
 * properties class
 * 
 */
 
#include "Loader.h"
#include "Properties.h"

namespace XNetFrame{

CLoader::CLoader(const char* szName/* = ""*/){
	m_sName = szName;

	m_pProperties = NULL;
}

CLoader::~CLoader(void){
}

}
