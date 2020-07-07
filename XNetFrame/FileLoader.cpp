///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "FileLoader.h"


namespace XNetFrame{

CFileLoader::CFileLoader(void) : CLoader(_T("FL")){
	m_sFile = _T("");
}

CFileLoader::CFileLoader(LPTSTR szLoader) : CLoader(szLoader){
	m_sFile = _T("");
}

CFileLoader::~CFileLoader(void){
	deleteAll();
}

CLoader::ELoaderResult CFileLoader::setFile(LPTSTR szFile){ // 
	if(szFile == NULL) return LR_NULL;

	m_sFile = szFile;

	return LR_OK;
}

void CFileLoader::deleteAll(void){ // 
	SectionMap::iterator itr;

	itr = m_mSectionMap.begin();
	if(itr != m_mSectionMap.end()) m_mSectionMap.erase(itr, m_mSectionMap.end());
}

CLoader::ELoaderResult CFileLoader::loadProperties(void){

	if(m_sFile.empty() == true || m_pProperties == NULL) return LR_NULL;

	if(loadSectionInfo() != LR_OK) return LR_BUFFER_OVERFLOW; // 
	
#ifdef WIN32
	TSTRING              sSection;
	SectionMap::iterator itr;
	for(itr = m_mSectionMap.begin(); itr != m_mSectionMap.end(); itr++){ // 
		sSection = (*itr).second;
		
		if(loadSectionInfo(sSection) != LR_OK) return LR_BUFFER_OVERFLOW; // 
	};
#endif

	return LR_OK;
}

#ifdef WIN32
CLoader::ELoaderResult CFileLoader::loadSectionInfo(void){ // 
	TCHAR                szBuffer[2050];
	sint32               siIndex, siLen;
	LPTSTR               szSection;
	TSTRING              sSection;
	SectionMap::iterator itr;

	deleteAll(); // 

	siLen = (sint32)::GetPrivateProfileSectionNames(szBuffer, 2048, m_sFile.c_str()); // 
	if(siLen == (2048 - 2)) return LR_BUFFER_OVERFLOW;

	szSection = &szBuffer[0];

	for(siIndex = 0; siIndex < siLen; siIndex++){
		if(szBuffer[siIndex] == 0x00){ // 
			sSection = szSection;

			itr = m_mSectionMap.find(sSection);
			if(itr == m_mSectionMap.end()) m_mSectionMap[sSection] = sSection; // 

			szSection = &szBuffer[siIndex + 1];
		};
	};

	return LR_OK;
}

CFileLoader::ELoaderResult CFileLoader::loadSectionInfo(TSTRING& sSection){ // 
	TSTRING             sInfo, sDelim, sKey, sValue;
	TCHAR               szBuffer[2050];
	sint32              siIndex, siLen;
	CStringTokenizer*  pTokenizer;
	LPTSTR              szInfo;

	siLen = (sint32)::GetPrivateProfileSection(sSection.c_str(), szBuffer, 2048, m_sFile.c_str()); // 
	if(siLen == (2048 - 2)) return LR_BUFFER_OVERFLOW;

	sDelim = _T("=");
	szInfo = &szBuffer[0];

	for(siIndex = 0; siIndex < siLen; siIndex++){
		if(szBuffer[siIndex] == 0x00){ // 
			sInfo = szInfo;

			pTokenizer = new CStringTokenizer(sInfo, sDelim);
			if(pTokenizer == NULL) return LR_NULL;

			sKey   = pTokenizer->nextToken(); // 
			sValue = pTokenizer->nextToken(); // 

			if(sKey.empty() == false && sValue.empty() == false){
				m_pProperties->addProperty((LPTSTR)m_sName.c_str(), (LPTSTR)sSection.c_str(), (LPTSTR)sKey.c_str(), (LPTSTR)sValue.c_str());
			};

			delete pTokenizer;
			pTokenizer = NULL;

			szInfo = &szBuffer[siIndex + 1];
		};
	};

	return LR_OK;
}

#else

CLoader::ELoaderResult CFileLoader::loadSectionInfo(void){
	return loadFileData(m_sFile);
}
	
CLoader::ELoaderResult CFileLoader::loadFileData(string& sFilePath)
{
	ifstream inifile;
    inifile.open(sFilePath.c_str());
    if(!inifile)
    {
        perror("open file error:");
        //string tmp = string("IniConfig: open ini file failed -> ") + inipath;
        return LR_FAILURE;
    }

    char tmp[2048];
    int num = 0;
    string currentline;
    string::size_type posi1, posi2;
    string section, key, value, infile;
	SectionMap::iterator itr;
	
    while(inifile.getline(tmp, sizeof(tmp)))
    {
        num++;
        currentline = tmp;
		
		posi1=currentline.find("\r");
	    currentline=currentline.substr(0,posi1);
        posi1 = currentline.find_first_not_of(" \t");
		if(currentline[posi1] == '#' || currentline.length() == 0 || posi1 == string::npos)
            continue;

        if(currentline[posi1] == '[')
        {
            if((posi2 = currentline.find(']')) != string::npos)
            {
                section = currentline.substr(posi1 + 1, posi2 - posi1 - 1);
                if(section.empty())
                {
                    printf("IniConfig: detect section is NULL in line %d", num);
                    return LR_FAILURE;
                }
            }
            else
            {
                printf("IniConfig: detect section lost \']\' in line %d", num);
				return LR_FAILURE;
            }
            continue;
        }

        posi2 = currentline.find('#');
        currentline = currentline.substr(posi1, posi2 - posi1);

        if((posi1 = currentline.find('=')) != string::npos)
        {
            key = currentline.substr(0, posi1);
            posi2 = key.find_last_not_of(" \t");
            key = key.substr(0, posi2 + 1);
            if(key.empty())
            {
                printf("IniConfig: detect key is NULL in line %d", num);
				return LR_FAILURE;
            }

            if((posi2 = currentline.find_first_not_of(" \t", posi1 + 1)) != string::npos)
            {
                value = currentline.substr(posi2);
                posi2 = value.find_last_not_of(" \t");
                value = value.substr(0, posi2 + 1);
            }
            else
            {
                printf("IniConfig: detect no value in line %d", num);
				return LR_FAILURE;
            }

			itr = m_mSectionMap.find(section);
			if(itr == m_mSectionMap.end()) m_mSectionMap[section] = section; // 
			
			m_pProperties->addProperty((LPTSTR)m_sName.c_str(), (LPTSTR)section.c_str(), (LPTSTR)key.c_str(), (LPTSTR)value.c_str());
	
        }
        else if((posi1 = currentline.find("include")) != string::npos)
        {
            infile = currentline.substr(posi1 + 7);
            if((posi1 = infile.find_first_not_of(" \t")) != string::npos)
            {
                posi2 = infile.find_last_not_of(" \t");
                infile = infile.substr(posi1, posi2 + 1);
                loadFileData(infile);
            }
            else
            {
                printf("IniConfig: detect invalid line at %d", num);
				return LR_FAILURE;
            }
        }
        else
        {
            printf("IniConfig: detect invalid line at %d", num);
			return LR_FAILURE;
        }
    }
	
	return LR_OK;
}
#endif

CLoader::ELoaderResult CFileLoader::setProperty(TSTRING& sSection, TSTRING& sKey, TSTRING& sValue){
	if(m_sFile.empty() == true) return LR_NULL;

	if(sSection.empty() == true || sKey.empty() == true || sValue.empty() == true) return LR_NULL;
#ifdef WIN32
	if(::WritePrivateProfileString(sSection.c_str(), sKey.c_str(), sValue.c_str(), m_sFile.c_str()) == false) return LR_FAILURE;
#endif
	return LR_OK;
}

CLoader::ELoaderResult CFileLoader::resetProperty(TSTRING& sSection, TSTRING& sKey){
	if(m_sFile.empty() == true) return LR_NULL;

	if(sSection.empty() == true || sKey.empty() == true) return LR_NULL;
#ifdef WIN32
	if(::WritePrivateProfileString(sSection.c_str(), sKey.c_str(), NULL, m_sFile.c_str()) == false) return LR_FAILURE;
#endif
	return LR_OK;
}

}
