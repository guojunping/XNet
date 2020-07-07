///////////////////////////////////////
// Author : Hank Lee
// Name : GameDispatchService.cpp
///////////////////////////////////////

#include "XNetDispatchService.h"

CXNetDispatchService::CXNetDispatchService(CLog* pLog) : CDispatchService(pLog, 1, false){ //
}

CXNetDispatchService::~CXNetDispatchService(void){
}

void CXNetDispatchService::onDestroyService(void){
}

void CXNetDispatchService::onCreateService(void){
	//CMessage* pMessage;
	//REGISTER_CG_MESSAGE(CG_USER_LOGIN);
}

