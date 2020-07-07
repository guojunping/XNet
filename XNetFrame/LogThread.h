#ifndef __LOG_THREAD_H
#define __LOG_THREAD_H

#include "TypesDn.h"

#include "Thread.h"


namespace XNetFrame
{

  class CLogThread : public CThread
  {

	private:

		CLog* m_pLog; // 

	public:
		explicit CLogThread(CLog*); // 
		~CLogThread(void);

		void run(void);
	};

}

#endif
