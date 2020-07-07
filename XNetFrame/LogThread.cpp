///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "LogThread.h"
#include "Log.h"
namespace XNetFrame{

	CLogThread::CLogThread(CLog* pLog) : CThread(){

		m_pLog = pLog;
	}

	CLogThread::~CLogThread(void){

	}

	void CLogThread::run(void){

		while(true){
			m_pLog->logOutput();
			if(CThread::getThreadFlag() == false) break; // 
		}

	}


}
