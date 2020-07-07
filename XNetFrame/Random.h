///////////////////////////////////////
// °³ ¹ß ÀÚ : ±è°æ¼®
// ¼³    ¸í : ³­¼ö ¹ß»ý±â¿¡ ´ëÇÑ ±¸Çö
///////////////////////////////////////

#ifndef __RANDOM_H
#define __RANDOM_H

#include "TypesDn.h"

namespace XNetFrame{

class CRandom{
private:
	uint32 m_uiMax;

public:
	CRandom(void);
	CRandom(uint32); // ³­¼öÀÇ ÃÖ´ë°ªÀ» ÁöÁ¤ÇÏ´Â »ý¼ºÀÚ
	virtual ~CRandom(void);

	uint32 next(void);  // ³­¼ö¸¦ È¹µæ
	void   reset(void); // ³­¼ö ¹ß»ý±â¸¦ ¸®¼Â
	bool   reset(uint32);
};

}

#endif
