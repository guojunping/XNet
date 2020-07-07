
#ifndef __BlockingQueue_H
#define __BlockingQueue_H

#include "TypesDn.h"
#include "Event.h"
#include <assert.h>

namespace XNetFrame{
	
template <class TYPE>
class CBlockingQueue
{
public:
	CBlockingQueue():m_nMax(-1),m_bDrop(false)
	{
	}

	~CBlockingQueue()
	{
		if(!m_bDrop)
			Destory();
	}
	
	void Push(TYPE& type)
	{
		m_cMonitor.enter() ;

		while ( m_nMax > 0 && (sint32)m_qQueue.size() >= m_nMax)
		{
#ifdef WIN32
			m_hPushEvent.reset();	
#endif
			m_cMonitor.leave() ;
			
			if( m_bDrop )
				return;
				
			m_hPushEvent.wait();
			
			m_cMonitor.enter() ;
		}
		m_qQueue.push(type);
		
		//printf("push queue size=%d\n",(sint32)m_qQueue.size());
				
		m_cMonitor.leave();
		m_hPopEvent.set();
	}
	
	void Insert(TYPE& type)
	{
		m_cMonitor.enter() ;

		while( m_nMax > 0 && (sint32)m_qQueue.size() >= m_nMax)
		{
#ifdef WIN32
			m_hPushEvent.reset();
#endif
			m_cMonitor.leave();

			if( m_bDrop )
				return;

			m_hPushEvent.wait();
			m_cMonitor.enter() ;
		}

		m_qQueue.push(type);
		//printf("insert queue size=%d\n",(sint32)m_qQueue.size());
		
		m_cMonitor.leave();
		m_hPopEvent.set();
	}

	TYPE Pop()
	{
		TYPE type;

		m_cMonitor.enter() ;

		while (m_qQueue.empty())
		{
#ifdef WIN32
			m_hPopEvent.reset();
#endif
			m_cMonitor.leave();
			
			if( m_bDrop )
				return type;
				
			m_hPopEvent.wait();
			
			m_cMonitor.enter() ;
		}

		type = m_qQueue.front();
		m_qQueue.pop();
		
		//printf("pop queue size=%d\n",(sint32)m_qQueue.size());
		
		m_cMonitor.leave();
		m_hPushEvent.set();
		
		return type;
	}
	
	int GetQueueCount()
	{
		int nCount = 0;
		nCount = (sint32)m_qQueue.size();
		return nCount;
	}

	void SetMaxCount(int nMax = -1)
	{
		synchronized sync(&m_cMonitor);
		m_nMax = nMax;
	}
	
	void Destory()
	{
		m_bDrop=true;
		m_hPushEvent.set();
		m_hPopEvent.set();
	}

	int GetMaxCount()
	{
		return m_nMax;
	}


private:

	CMonitor   m_cMonitor;
	queue<TYPE> m_qQueue;
	
	CAutomaticResetEvent m_hPopEvent;
	CAutomaticResetEvent m_hPushEvent;

	sint32 m_nMax;
	bool m_bDrop;

};
}
#endif
