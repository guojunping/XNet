#include "AuthCrypt.h"

namespace XNetFrame{

CAuthCrypt::CAuthCrypt(void)
{
	
}

CAuthCrypt::~CAuthCrypt(void)
{
}

void CAuthCrypt::init(void)
{
	m_uiSendI = m_uiSendJ = m_uiRecvI = m_uiRecvJ = 0;
	setKey();
}

void CAuthCrypt::setKey(string str)
{
	m_vKey.clear();

	for(size_t t = 0; t < str.length(); t++)
	{
		m_vKey.push_back(str[t]);
	}
}

void CAuthCrypt::addKeyElement(uchar8 element)
{
	m_vKey.push_back(element);
}

void CAuthCrypt::decryptRecv(uchar8*data, size_t len)
{
	//if(m_vKey.size() == 0) return;

	init();

	for (size_t t = 0; t < len; t++)
	{
		m_uiRecvI %= m_vKey.size();
		uchar8 x = (data[t] - m_uiRecvJ) ^ m_vKey[m_uiRecvI];
		++m_uiRecvI;
		m_uiRecvJ = data[t];
		data[t] = x;
	}
}

void CAuthCrypt::encryptSend(uchar8*data, size_t len)
{
	//if(m_vKey.size() == 0) return;

	init();

	for (size_t t = 0; t < len; t++)
	{
		m_uiSendI %= m_vKey.size();
		uchar8 x = (data[t] ^ m_vKey[m_uiSendI]) + m_uiSendJ;
		++m_uiSendI;
		data[t] = m_uiSendJ = x;
	}
}

}