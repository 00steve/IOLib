#pragma once






class IIO {
public:

	virtual bool ClearReadBuffer() = 0;

	virtual unsigned char* Read(unsigned int length) = 0;

	virtual unsigned char* ReadAll() = 0;

	virtual char ReadChar() = 0;

	virtual bool TryRead(unsigned char*& data, unsigned int length) = 0;

	virtual bool Write(unsigned char* data) = 0;

	virtual bool WriteChar(unsigned char c) = 0;

};