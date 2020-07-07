#ifndef THREAD_LINUX_OS_H
#define THREAD_LINUX_OS_H

#include <pthread.h>
#include <unistd.h>

typedef pthread_t Thread_t;
typedef pthread_mutex_t MutexType;

inline void InitMutex(MutexType& mu)
{
	//mu = PTHREAD_MUTEX_INITIALIZER;
	
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
	
	pthread_mutex_init(&mu, &attr);
}
inline void DestroyMutex(MutexType& mu)
{
	pthread_mutex_destroy(&mu);
}
inline void LockMutex(MutexType& mu)
{
	pthread_mutex_lock(&mu);
}
inline void UnlockMutex(MutexType& mu)
{
	pthread_mutex_unlock(&mu);
}

inline pthread_t GetCurrentThreadId()
{
	return pthread_self();
}

#endif // THREAD_LINUX_OS_H