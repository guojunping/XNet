
#include "Stream.h"

Stream::Stream()
: m_streamStatus(Closed)
{
	//
}

Stream::~Stream()
{
	//
}

const char* Stream::getStatusString(const Status in_status)
{
	switch (in_status) {
	  case Ok:
		  return "StreamOk";
	  case IOError:
		  return "StreamIOError";
	  case EOS:
		  return "StreamEOS";
	  case IllegalCall:
		  return "StreamIllegalCall";
	  case Closed:
		  return "StreamClosed";
	  case UnknownError:
		  return "StreamUnknownError";

	  default:
		  return "Invalid Stream::Status";
	}
}

void Stream::writeString(const char *string, sint32 maxLen)
{
	sint32 len = string ? strlen(string) : 0;
	if(len > maxLen)
		len = maxLen;

	write(uchar8(len));
	if(len)
		write(len, string);
}

void Stream::readString(char buf[256])
{
	uchar8 len;
	read(&len);
	read(sint32(len), buf);
	buf[len] = 0;
}

void Stream::readLongString(uint32 maxStringLen, char *stringBuf)
{
	uint32 len;
	read(&len);
	if(len > maxStringLen)
	{
		m_streamStatus = IOError;
		return;
	}
	read(len, stringBuf);
	stringBuf[len] = 0;
}

void Stream::writeLongString(uint32 maxStringLen, const char *string)
{
	uint32 len = (uint32)strlen(string);
	if(len > maxStringLen)
		len = maxStringLen;
	write(len);
	write(len, string);
}