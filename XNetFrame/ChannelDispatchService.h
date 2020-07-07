#ifndef __CHEN_DISPATCH_SERVICE_H
#define __CHEN_DISPATCH_SERVICE_H

#include "DispatchService.h"

namespace XNetFrame{
	
class CChannelDispatchService : public CDispatchService{
public:
	explicit CChannelDispatchService(CLog*, sint32, bool); // 
	~CChannelDispatchService(void);
	
	sint32 dispatchChannel(void);  //
    bool   deliveryChannel(void); // 
	
	virtual void onCreateService(void) {};
	virtual void onDestroyService(void){};
};
}



#endif