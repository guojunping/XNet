//-----------------------------------------------------------------------------
// Êý¾ÝÁ÷²Ù×÷µÄ·â×°
// ¶Ô×Ö½ÚË³ÐòÖ§³Ö£ºÐ¡Í·¶Ë¡¢´óÍ·¶Ë
//-----------------------------------------------------------------------------

#ifndef _STREAM_H_
#define _STREAM_H_

#include "Platform.h"
#include "TypesDn.h"
#define BIT(x) (1 << (x))                       ///< Returns value with bit x set (2^x)

//------------------------------------------------------------------------------
//-------------------------------------- Base Stream class
//
/// Base stream class for streaming data across a specific media
class Stream {
	// Public structs and enumerations...
public:
	/// Status constantants for the stream
	enum Status {
		Ok = 0,           ///< Ok!
		IOError,          ///< Read or Write error
		EOS,              ///< End of Stream reached (mostly for reads)
		IllegalCall,      ///< An unsupported operation used.  Always w/ accompanied by AssertWarn
		Closed,           ///< Tried to operate on a closed stream (or detached filter)
		UnknownError      ///< Catchall
	};

	enum Capability {
		StreamWrite    = BIT(0), ///< Can this stream write?
		StreamRead     = BIT(1), ///< Can this stream read?
		StreamPosition = BIT(2)  ///< Can this stream position?
	};

	// Accessible only through inline accessors
private:
	Status m_streamStatus;

	// Derived accessible data modifiers...
protected:
	void setStatus(const Status in_newStatus) { m_streamStatus = in_newStatus; }

public:
	Stream();
	virtual ~Stream();

	/// Gets the status of the stream
	Stream::Status getStatus() const { return m_streamStatus; }
	/// Gets a printable string form of the status
	static const char* getStatusString(const Status in_status);

	// Derived classes must override these...
protected:
	virtual bool _read(const uint32 in_numBytes,  void* out_pBuffer)      = 0;
	virtual bool _write(const uint32 in_numBytes, const void* in_pBuffer) = 0;
public:
	/// Checks to see if this stream has the capability of a given function
	virtual bool hasCapability(const Capability) const = 0;

	/// Gets the position in the stream
	virtual uint32  getPosition() const                      = 0;
	/// Sets the position of the stream.  Returns if the new position is valid or not
	virtual bool setPosition(const uint32 in_newPosition) = 0;
	/// Gets the size of the stream
	virtual uint32  getStreamSize() = 0;

	/// Reads a string of maximum 255 characters long
	virtual void readString(char stringBuf[256]);
	/// Reads a string that could potentially be more than 255 characters long.
	/// @param maxStringLen Maximum length to read.  If the string is longer than maxStringLen, only maxStringLen bytes will be read.
	/// @param stringBuf buffer where data is read into
	void readLongString(uint32 maxStringLen, char *stringBuf);
	/// Writes a string to the stream.  This function is slightly unstable.
	/// Only use this if you have a valid string that is not empty.
	/// writeString is safer.
	void writeLongString(uint32 maxStringLen, const char *string);

	/// Writes a string to the stream.
	virtual void writeString(const char *stringBuf, sint32 maxLen=255);


	// Overloaded write and read ops..
public:
	bool read(const uint32 in_numBytes,  void* out_pBuffer) {
		return _read(in_numBytes, out_pBuffer);
	}
	bool write(const uint32 in_numBytes, const void* in_pBuffer) {
		return _write(in_numBytes, in_pBuffer);
	}
	bool read(schar8* out_read) {             
		return read(sizeof(schar8), out_read); 
	}
	bool write(schar8 in_write) {               
		return write(sizeof(schar8), &in_write); 
	}
	bool read(uchar8* out_read) {             
		return read(sizeof(uchar8), out_read); 
	}
	bool write(uchar8 in_write) {               
		return write(sizeof(uchar8), &in_write); 
	}

	bool read(sint16* out_read) {                     
		sint16 temp;                                  
		bool success = read(sizeof(sint16), &temp);    
		*out_read = convertLEndianToHost(temp);      
		return success;                              
	}
	bool write(sint16 in_write) {                     
		sint16 temp = convertHostToLEndian(in_write);  
		return write(sizeof(sint16), &temp);           
	}
	bool read(sint32* out_read) {                     
		sint32 temp;                                  
		bool success = read(sizeof(sint32), &temp);    
		*out_read = convertLEndianToHost(temp);      
		return success;                              
	}
	bool write(sint32 in_write) {                     
		sint32 temp = convertHostToLEndian(in_write);  
		return write(sizeof(sint32), &temp);           
	}
	bool read(sint64* out_read) {                     
		sint64 temp;                                  
		bool success = read(sizeof(sint64), &temp);    
		*out_read = convertLEndianToHost(temp);      
		return success;                              
	}
	bool write(sint64 in_write) {                     
		sint64 temp = convertHostToLEndian(in_write);  
		return write(sizeof(sint64), &temp);           
	}
	bool read(uint16* out_read) {                     
		uint16 temp;                                  
		bool success = read(sizeof(uint16), &temp);    
		*out_read = convertLEndianToHost(temp);      
		return success;                              
	}
	bool write(uint16 in_write) {                     
		uint16 temp = convertHostToLEndian(in_write);  
		return write(sizeof(uint16), &temp);           
	}
	bool read(uint32* out_read) {                     
		uint32 temp;                                  
		bool success = read(sizeof(uint32), &temp);    
		*out_read = convertLEndianToHost(temp);      
		return success;                              
	}
	bool write(uint32 in_write) {                     
		uint32 temp = convertHostToLEndian(in_write);  
		return write(sizeof(uint32), &temp);           
	}
	bool read(F32* out_read) {                     
		F32 temp;                                  
		bool success = read(sizeof(F32), &temp);    
		*out_read = convertLEndianToHost(temp);      
		return success;                              
	}
	bool write(F32 in_write) {                     
		F32 temp = convertHostToLEndian(in_write);  
		return write(sizeof(F32), &temp);           
	}
	bool read(F64* out_read) {                     
		F64 temp;                                  
		bool success = read(sizeof(F64), &temp);    
		*out_read = convertLEndianToHost(temp);      
		return success;                              
	}
	bool write(F64 in_write) {                     
		F64 temp = convertHostToLEndian(in_write);  
		return write(sizeof(F64), &temp);           
	}

	// We have to do the bool's by hand, since they are different sizes
	//  on different compilers...
	//
	bool read(bool* out_pRead) {
		uchar8 translate;
		bool success = read(&translate);
		if (success == false)
			return false;

		*out_pRead = translate != 0;
		return true;
	}
	bool write(const bool& in_rWrite) {
		uchar8 translate = in_rWrite ? uchar8(1) : uchar8(0);
		return write(translate);
	}
};

#endif //_STREAM_H_
