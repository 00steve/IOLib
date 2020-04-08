#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include "IIO.h"

class SerialPort : public IIO {
private:

	unsigned char* buffer;

	/* the length (bytes) that the data extends to. When the serial read gets towards the 
	end of the buffer, if it sees that it will overflow, it will set the last ending char 
	as the end point for the buffer and start writing to the beginning again.*/
	unsigned int bufferDataLength; 

	unsigned int bufferReadCursor;

	unsigned int bufferReadSizeB;

	unsigned int bufferSizeB;

	unsigned int bufferSizeKB;

	unsigned int bufferWriteCursor;

	DWORD bytesRead = 0;

	DWORD bytesWritten = 0;

	unsigned char serialData[128];

	bool error;

	std::string name;

	bool readWriteFlip;

	HANDLE serialPort;


public:
	bool ClearReadBuffer();

	bool Close();

	bool Open(std::string name);

	unsigned char* Read(unsigned int length);

	unsigned char* ReadAll();

	char ReadChar();

	bool ReadToBuffer();

	SerialPort();

	~SerialPort();
	
	void SetBufferSize(unsigned int bufferSizeMB);

	bool Write(unsigned char* data);

	bool WriteChar(unsigned char c);
};