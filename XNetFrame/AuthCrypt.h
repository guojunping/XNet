#ifndef	__AUTH_CRYPT_H
#define	__AUTH_CRYPT_H

#include "TypesDn.h"

namespace XNetFrame{

class CAuthCrypt{
public:
	CAuthCrypt(void);
	~CAuthCrypt(void);

	void init();
	void setKey(string = DEFAULT_CRYPT_STR);
	void addKeyElement(uchar8);

	void decryptRecv(uchar8*, size_t);
	void encryptSend(uchar8*, size_t);

private:
	vector<uchar8> m_vKey;
	uchar8 m_uiSendI, m_uiSendJ, m_uiRecvI, m_uiRecvJ;
};

}

#endif