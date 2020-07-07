//-----------------------------------------------------------------------------
// Î»Á÷²Ù×÷
//-----------------------------------------------------------------------------

#ifndef _BITSTREAM_H_
#define _BITSTREAM_H_

#include "Stream.h"

//-------------------------------------- Some caveats when using this class:
//                                        - Get/setPosition semantics are changed
//                                         to indicate bit position rather than
//                                         byte position.
//

inline sint32 mClamp(sint32 val, sint32 low, sint32 high)
{
	return getMax(getMin(val, high), low);
}

inline F32 mClampF(F32 val, F32 low, F32 high)
{
	return (F32) getMax(getMin(val, high), low);
}

uint32 getNextPow2(uint32 io_num)
{
	sint32 oneCount   = 0;
	sint32 shiftCount = -1;
	while (io_num) {
		if(io_num & 1)
			oneCount++;
		shiftCount++;
		io_num >>= 1;
	}
	if(oneCount > 1)
		shiftCount++;

	return uint32(1 << shiftCount);
}

//--------------------------------------
uint32 getBinLog2(uint32 io_num)
{
	sint32 shiftCount = 0;
	while (io_num) {
		shiftCount++;
		io_num >>= 1;
	}

	return uint32(shiftCount - 1);
}


class BitStream : public Stream
{
protected:
	uchar8 *dataPtr;
	sint32  bitNum;
	sint32  bufSize;
	bool error;
	sint32  maxReadBitNum;
	sint32  maxWriteBitNum;
	char *stringBuffer;

public:
	void setBuffer(void *bufPtr, sint32 bufSize, sint32 maxSize = 0);
	uchar8*  getBuffer() { return dataPtr; }
	uchar8*  getBytePtr();

	uint32   getReadByteSize();

	sint32   getCurPos() const;
	void     setCurPos(const uint32);

	BitStream(void *bufPtr, sint32 bufSize, sint32 maxWriteSize = -1) { setBuffer(bufPtr, bufSize,maxWriteSize); stringBuffer = NULL; }
	void clear();

	void setStringBuffer(char buffer[256]);
	void writeInt(sint32 value, sint32 bitCount);
	sint32  readInt(sint32 bitCount);

	void writeSignedInt(sint32 value, sint32 bitCount);
	sint32  readSignedInt(sint32 bitCount);

	void writeRangedU32(uint32 value, uint32 rangeStart, uint32 rangeEnd);
	uint32  readRangedU32(uint32 rangeStart, uint32 rangeEnd);

	/// Writes a clamped signed integer to the stream using 
	/// an optimal amount of bits for the range.
	void writeRangedS32( sint32 value, sint32 min, sint32 max );
	/// Reads a ranged signed integer written with writeRangedS32.
	sint32 readRangedS32( sint32 min, sint32 max );

	// read and write floats... floats are 0 to 1 inclusive, signed floats are -1 to 1 inclusive
	F32  readFloat(sint32 bitCount);
	F32  readSignedFloat(sint32 bitCount);

	void writeFloat(F32 f, sint32 bitCount);
	void writeSignedFloat(F32 f, sint32 bitCount);

	/// Writes a clamped floating point value to the 
	/// stream with the desired bits of precision.
	void writeRangedF32( F32 value, F32 min, F32 max, uint32 numBits );
	/// Reads a ranged floating point value written with writeRangedF32.
	F32 readRangedF32( F32 min, F32 max, uint32 numBits );

	virtual void writeBits(sint32 bitCount, const void *bitPtr);
	virtual void readBits(sint32 bitCount, void *bitPtr);
	virtual bool writeFlag(bool val);
	virtual bool readFlag();

	void setBit(sint32 bitCount, bool set);
	bool testBit(sint32 bitCount);

	bool isFull() { return bitNum > (bufSize << 3); }
	bool isValid() { return !error; }

	bool _read (const uint32 size,void* d);
	bool _write(const uint32 size,const void* d);

	void readString(char stringBuf[256]);
	void writeString(const char *stringBuf, sint32 maxLen=255);

	bool hasCapability(const Capability) const { return true; }
	uint32  getPosition() const;
	bool setPosition(const uint32 in_newPosition);
	uint32  getStreamSize();
};

//------------------------------------------------------------------------------
//-------------------------------------- INLINES
//
inline sint32 BitStream::getCurPos() const
{
	return bitNum;
}

inline void BitStream::setCurPos(const uint32 in_position)
{
	bitNum = sint32(in_position);
}

inline bool BitStream::readFlag()
{
	if(bitNum > maxReadBitNum)
	{
		error = true;
		return false;
	}
	sint32 mask = 1 << (bitNum & 0x7);
	bool ret = (*(dataPtr + (bitNum >> 3)) & mask) != 0;
	bitNum++;
	return ret;
}

inline void BitStream::writeRangedU32(uint32 value, uint32 rangeStart, uint32 rangeEnd)
{
	uint32 rangeSize = rangeEnd - rangeStart + 1;
	uint32 rangeBits = getBinLog2(getNextPow2(rangeSize));

	writeInt(sint32(value - rangeStart), sint32(rangeBits));
}

inline uint32 BitStream::readRangedU32(uint32 rangeStart, uint32 rangeEnd)
{
	uint32 rangeSize = rangeEnd - rangeStart + 1;
	uint32 rangeBits = getBinLog2(getNextPow2(rangeSize));

	uint32 val = uint32(readInt(sint32(rangeBits)));
	return val + rangeStart;
}

inline void BitStream::writeRangedS32( sint32 value, sint32 min, sint32 max )
{
	value = mClamp( value, min, max );
	writeRangedU32( ( value - min ), 0, ( max - min ) );
}

inline sint32 BitStream::readRangedS32( sint32 min, sint32 max )
{
	return readRangedU32( 0, ( max - min ) ) + min;
}

inline void BitStream::writeRangedF32( F32 value, F32 min, F32 max, uint32 numBits )
{
	value = ( mClampF( value, min, max ) - min ) / ( max - min );
	writeInt( (sint32)(value * ( (1 << numBits) - 1 )), numBits );
}

inline F32 BitStream::readRangedF32( F32 min, F32 max, uint32 numBits )
{
	F32 value = (F32)readInt( numBits );
	value /= F32( ( 1 << numBits ) - 1 );
	return min + value * ( max - min );
}

#endif //_BITSTREAM_H_
