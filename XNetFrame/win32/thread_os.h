#ifndef THREAD_WIN32_OS_H
#define THREAD_WIN32_OS_H

#include <Windows.h>

typedef HANDLE Thread_t;
typedef CRITICAL_SECTION MutexType;

inline void InitMutex(MutexType& mu)
{
	::InitializeCriticalSection(&mu);
}
inline void DestroyMutex(MutexType& mu)
{
	::DeleteCriticalSection(&mu);
}
inline void LockMutex(MutexType& mu)
{
	::EnterCriticalSection(&mu);
}
inline void UnlockMutex(MutexType& mu)
{
	::LeaveCriticalSection(&mu);
}

#endif // THREAD_LINUX_OS_H