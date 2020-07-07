///////////////////////////////////////
// Author : Hank Lee
// Name : GameDispatchService.h
///////////////////////////////////////

#ifndef __XNET_DISPATCH_SERVICE_H
#define __GAME_DISPATCH_SERVICE_H

#include "Log.h"
#include "DispatchService.h"

using namespace XNetFrame;

class CXNetDispatchService : public CDispatchService{
	
public:

	CLog* m_pLog;

public:

	CXNetDispatchService(CLog*);
	~CXNetDispatchService(void);

	void onCreateService(void);
	void onDestroyService(void);
};

#endif
