#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#ifndef linux
#define linux
#endif

#if defined(__WIN32__) || defined(_WIN32)

#include "./win32/time_os.h"
#include "./win32/thread_os.h"

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
typedef	WSAEVENT	IOEVENT;
typedef int			socklen;

#elif defined(linux)

#include "./linux/time_os.h"
#include "./linux/thread_os.h"

#include <semaphore.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>
#include <signal.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <math.h>
#include <hash_map>
#include <fstream>


using namespace std;
using namespace __gnu_cxx;


typedef int			   SOCKET;
typedef unsigned long  DWORD;
typedef socklen_t	   socklen;
typedef const char*	   LPTSTR;
typedef string         TSTRING;
typedef char           TCHAR;

#define IOEVENT        sem_t*    
#define _T(STR)        (STR)
#define _tcscmp        strcmp
#define _tcslen        strlen
#define _tfopen        fopen
#define _ftprintf      fprintf
#define _fgetts        fgets
#define _tcsncmp       strncmp
#define _tcsncpy       strncpy
#define _ttoi          atoi
#define _vsntprintf    vsnprintf
#define _tcscat        strcat
#define _tcschr        strchr
#define _tcsstr        strstr
#define _tcsftime      strftime
#define _stprintf      sprintf

#define INVALID_SOCKET    (-1)
#define SOCKET_ERROR	  (-1)
#define closesocket	close
#define WSAIoctl ioctl
#define SD_BOTH	SHUT_RDWR
#define TRUE 1
#define FALSE 0

inline int WSAGetLastError()
{
	return errno;
}

#define   WSAEINTR           EINTR
#define   WSAEBADF           EBADF
#define   WSAEACCES          EACCES
#define   WSAEFAULT          EFAULT
#define   WSAEINVAL          EINVAL
#define   WSAEMFILE          EMFILE
#define   WSAEWOULDBLOCK     EWOULDBLOCK
#define   WSAEINPROGRESS     EINPROGRESS
#define   WSAEALREADY        EALREADY
#define   WSAENOTSOCK        ENOTSOCK
#define   WSAEDESTADDRREQ    EDESTADDRREQ
#define   WSAEMSGSIZE        EMSGSIZE
#define   WSAEPROTOTYPE      EPROTOTYPE
#define   WSAENOPROTOOPT     ENOPROTOOPT
#define   WSAEOPNOTSUPP      EOPNOTSUPP
#define   WSAEPFNOSUPPORT    EPFNOSUPPORT
#define   WSAEAFNOSUPPORT    EAFNOSUPPORT
#define   WSAEADDRINUSE      EADDRINUSE
#define   WSAEADDRNOTAVAIL   EADDRNOTAVAIL
#define   WSAENETDOWN        ENETDOWN
#define   WSAENETUNREACH     ENETUNREACH
#define   WSAENETRESET       ENETRESET
#define   WSAECONNABORTED    ECONNABORTED
#define   WSAECONNRESET      ECONNRESET
#define   WSAENOBUFS         ENOBUFS
#define   WSAEISCONN         EISCONN
#define   WSAENOTCONN        ENOTCONN
#define   WSAESHUTDOWN       ESHUTDOWN
#define   WSAETOOMANYREFS    ETOOMANYREFS
#define   WSAETIMEDOUT       ETIMEDOUT
#define   WSAECONNREFUSED    ECONNREFUSED
#define   WSAELOOP           ELOOP
#define   WSAENAMETOOLONG    ENAMETOOLONG
#define   WSAEHOSTDOWN       EHOSTDOWN
#define   WSAEHOSTUNREACH    EHOSTUNREACH
#define   WSAHOST_NOT_FOUND  HOST_NOT_FOUND
#define   WSATRY_AGAIN       TRY_AGAIN
#define   WSANO_RECOVERY     NO_RECOVERY
#define   WSANO_DATA         NO_DATA


#elif defined(__OpenBSD__)

#elif defined(__FreeBSD__)

#elif defined(__APPLE__)

#else 
#  error "Unsupported Operating System"
#endif

typedef	signed		char		schar8; 
typedef	unsigned	char		uchar8;
typedef	signed		short		sint16; 
typedef	unsigned	short		uint16;
typedef	signed		int		sint32; 
typedef	unsigned	int		uint32;
typedef	signed		long long		sint64;
typedef	unsigned	long long		uint64;

typedef	signed		int			sint;
typedef	unsigned	int			uint;

typedef float	F32;
typedef double	F64;
typedef char	BYTE;

#define MAX_PACKET_LEN                       (4096)

/// Returns the lesser of the two parameters: a & b.
inline uint32 getMin(uint32 a, uint32 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline uint16 getMin(uint16 a, uint16 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline uchar8 getMin(uchar8 a, uchar8 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline sint32 getMin(sint32 a, sint32 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline sint16 getMin(sint16 a, sint16 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline schar8 getMin(schar8 a, schar8 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline F32 getMin(F32 a, F32 b)
{
	return a>b ? b : a;
}

/// Returns the lesser of the two parameters: a & b.
inline F64 getMin(F64 a, F64 b)
{
	return a>b ? b : a;
}

/// Returns the greater of the two parameters: a & b.
inline uint32 getMax(uint32 a, uint32 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline uint16 getMax(uint16 a, uint16 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline uchar8 getMax(uchar8 a, uchar8 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline sint32 getMax(sint32 a, sint32 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline sint16 getMax(sint16 a, sint16 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline schar8 getMax(schar8 a, schar8 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline F32 getMax(F32 a, F32 b)
{
	return a>b ? a : b;
}

/// Returns the greater of the two parameters: a & b.
inline F64 getMax(F64 a, F64 b)
{
	return a>b ? a : b;
}

inline uint16 convertHostToLEndian(uint16 i) { return i; }
inline uint16 convertLEndianToHost(uint16 i) { return i; }
inline uint32 convertHostToLEndian(uint32 i) { return i; }
inline uint32 convertLEndianToHost(uint32 i) { return i; }
inline sint16 convertHostToLEndian(sint16 i) { return i; }
inline sint16 convertLEndianToHost(sint16 i) { return i; }
inline sint32 convertHostToLEndian(sint32 i) { return i; }
inline sint32 convertLEndianToHost(sint32 i) { return i; }
inline sint64 convertHostToLEndian(sint64 i) { return i; }
inline sint64 convertLEndianToHost(sint64 i) { return i; }

inline float convertHostToLEndian(float i) { return i; }
inline float convertLEndianToHost(float i) { return i; }

inline double convertHostToLEndian(double i) { return i; }
inline double convertLEndianToHost(double i) { return i; }

inline uint16 convertHostToBEndian(uint16 i)
{
	return uint16((i << 8) | (i >> 8));
}

inline uint16 convertBEndianToHost(uint16 i)
{
	return uint16((i << 8) | (i >> 8));
}

inline sint16 convertHostToBEndian(sint16 i)
{
	return sint16(convertHostToBEndian(uint16(i)));
}

inline sint16 convertBEndianToHost(sint16 i)
{
	return sint16(convertBEndianToHost(sint16(i)));
}

inline uint32 convertHostToBEndian(uint32 i)
{
	return ((i << 24) & 0xff000000) |
		((i <<  8) & 0x00ff0000) |
		((i >>  8) & 0x0000ff00) |
		((i >> 24) & 0x000000ff);
}

inline uint32 convertBEndianToHost(uint32 i)
{
	return ((i << 24) & 0xff000000) |
		((i <<  8) & 0x00ff0000) |
		((i >>  8) & 0x0000ff00) |
		((i >> 24) & 0x000000ff);
}

inline sint32 convertHostToBEndian(sint32 i)
{
	return sint32(convertHostToBEndian(uint32(i)));
}

inline sint32 convertBEndianToHost(sint32 i)
{
	return sint32(convertBEndianToHost(uint32(i)));
}

inline sint64 convertBEndianToHost(sint64 i)
{
	sint32 *inp = (sint32 *) &i;
	sint64 ret;
	sint32 *outp = (sint32 *) &ret;
	outp[0] = convertBEndianToHost(inp[1]);
	outp[1] = convertBEndianToHost(inp[0]);
	return ret;
}

inline sint64 convertHostToBEndian(sint64 i)
{
	sint32 *inp = (sint32 *) &i;
	sint64 ret;
	sint32 *outp = (sint32 *) &ret;
	outp[0] = convertHostToBEndian(inp[1]);
	outp[1] = convertHostToBEndian(inp[0]);
	return ret;
}

inline uint64 convertBEndianToHost(uint64 i)
{
	uint32 *inp = (uint32 *) &i;
	uint64 ret;
	uint32 *outp = (uint32 *) &ret;
	outp[0] = convertBEndianToHost(inp[1]);
	outp[1] = convertBEndianToHost(inp[0]);
	return ret;
}

inline uint64 convertHostToBEndian(uint64 i)
{
	uint32 *inp = (uint32 *) &i;
	uint64 ret;
	uint32 *outp = (uint32 *) &ret;
	outp[0] = convertHostToBEndian(inp[1]);
	outp[1] = convertHostToBEndian(inp[0]);
	return ret;
}

inline double convertBEndianToHost(double in_swap)
{
	uint64 result = convertBEndianToHost(* ((uint64 *) &in_swap) );
	return * ((double *) &result);
}

inline double convertHostToBEndian(double in_swap)
{
	uint64 result = convertHostToBEndian(* ((uint64 *) &in_swap) );
	return * ((double *) &result);
}

#endif