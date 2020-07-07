///////////////////////////////////////
// °³ ¹ß ÀÚ : ±è°æ¼®
// ¼³    ¸í : ¼¼¼Ç ÆÑÅä¸® °ü·Ã Å¬·¡½º¿¡ ´ëÇÑ ±¸Çö
///////////////////////////////////////

#ifndef __SESSION_FACTORY_H
#define __SESSION_FACTORY_H

#include "TypesDn.h"

#include "Session.h"
#include "Monitor.h"

namespace XNetFrame{

typedef hash_map<uint32, CSession*> SessionMap;

#ifndef WIN32 
typedef list<CSession*> SessionList;
#endif

class CSessionFactory{ // Å¬·¡½º ³»ÀÇ ¸ðµç ¸â¹ö ÇÔ¼ö ÀÌ¿ë½Ã ¸ð´ÏÅÍ °´Ã¼¿¡¼­ ¹ß»ýÇÏ´Â ¿¹¿Ü »óÈ²¿¡ ´ëÇÑ Ã¼Å©
private:
	CMonitor   m_cMonitor;    // ¸ð´ÏÅÍ °´Ã¼
	SessionMap m_mSessionMap; // ¼¼¼Ç °´Ã¼¸¦ À¯ÁöÇÏ´Â STL °´Ã¼
	
#ifndef WIN32
	SessionList m_lSessionUnusedList;
#endif

	volatile uint32 m_uiUsedCount  ;        // ÀüÃ¼ ¼¼¼Ç °´Ã¼¼ö

public:
	CSessionFactory(void);
	virtual ~CSessionFactory(void);

	CSession* getSession(uint32);        // ¼¼¼Ç ID¿¡ ºÎÇÕÇÏ´Â ¼¼¼Ç °´Ã¼¸¦ ¸®ÅÏ

	bool      addSession(CSession*);     // ¼¼¼Ç °´Ã¼¸¦ Ãß°¡(¼¼¼Ç ID´Â À¯ÀÏÇØ¾ß ÇÔ)
	bool      deleteSession(CSession*);  // ¼¼¼Ç °´Ã¼¸¦ »èÁ¦(¼¼¼Ç °´Ã¼ ÀÚÃ¼µµ Ãß°¡·Î ÆÄ±«)
	CSession* getAvailableSession(void); // »ç¿ë °¡´ÉÇÑ ¼¼¼Ç °´Ã¼¸¦ ¸®ÅÏ
    bool      putAuailableSession(CSession*);

	uint32    getCount(void);          // ÇöÀç µî·ÏµÈ ¼¼¼Ç °´Ã¼¼ö¸¦ ¸®ÅÏ
	uint32    getUsedCount(void);      // »ç¿ëÁßÀÎ ¼¼¼Ç °´Ã¼¼ö¸¦ ¸®ÅÏ
	uint32    getAvailableCount(void); // »ç¿ë °¡´ÉÇÑ ¼¼¼Ç °´Ã¼¼ö¸¦ ¸®ÅÏ

	void      closeAll(void);  // ¸ðµç ¼¼¼Ç °´Ã¼¿¡ ´ëÇØ doClose ÇÔ¼ö¸¦ È£Ãâ
	void      deleteAll(void); // ¼¼¼Ç °´Ã¼¸¦ ¸ðµÎ »èÁ¦(¼¼¼Ç °´Ã¼ ÀÚÃ¼µµ Ãß°¡·Î ¸ðµÎ ÆÄ±«)


};

}

#endif
