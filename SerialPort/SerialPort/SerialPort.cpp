// SerialPort.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "SerialPort.h"



std::wstring s2ws(const std::string& s) {
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}


bool SerialPort::ClearReadBuffer(){
    bufferDataLength = 0;
    bufferWriteCursor = 0;
    bufferReadCursor = 0;
    return false;
}

bool SerialPort::Close() {
    if (serialPort != INVALID_HANDLE_VALUE) {
        FindClose(serialPort);
        std::cout << "Closed serial port " << name << ".\n";
        return true;
    }
    else {
        std::cout << "Couldn't close serial port " << name << ". It doesn't seem to be valid.\n";
    }
    return false;
}


bool SerialPort::Open(std::string name) {
    this->name = name;
	serialPort = CreateFile(s2ws(name).c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    error = serialPort == INVALID_HANDLE_VALUE;
    if (error) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            //serial port does not exist. Inform user.
            std::cout << "Serial port error, " << serialPort << " does not exist" << std::endl;
        }
        //some other error occurred. Inform user.
        std::cout << "Serial port probably in use" << std::endl;
        return false;
    }

    
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(serialPort, &dcbSerialParams)) {
        std::cout << "error getting state" << std::endl;
    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    error = !SetCommState(serialPort, &dcbSerialParams);
    if (error) {
        std::cout << "error setting serial port state" << std::endl;
        return false;
    }
    

    COMMTIMEOUTS timeouts = { 0 };

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    error = !SetCommTimeouts(serialPort, &timeouts);
    if (error) {
        std::cout << "Error occurred" << std::endl;
    }
    



    if (!error) {
        std::cout << "Successfully created and setup serial port " << name << ".\n";
    }
	return !error;
}

unsigned char* SerialPort::Read(unsigned int length) {
    //if write cursor not far enough in front of the read cursor so the read cursor 
    //can read all of the bytes that it wants, return null, or do that shit by default
    //because there's no code in here yet.
	return nullptr;
}

unsigned char* SerialPort::ReadAll() {
    unsigned char* data = new unsigned char[0];
    unsigned int length = 0;
    /*If the read cursor and write cursor have flipped (the read cursor > write cursor) because the 
    write cursor got to the end of the buffer and the read cursor is still trying to catch up, get 
    the length of data left to read between the buffer data length and read cursor. If it is more than
    0, read all of the data that is left. 
    
    No matter what reset the read cursor to the beginning of the buffer and set the read write flip 
    flag back to the defualt of false*/
    if (readWriteFlip) {
        length = bufferDataLength - bufferReadCursor;
        if (length > 0) {
            data = new unsigned char[length + 1];
            memcpy(data, &buffer[bufferReadCursor], sizeof(char) * length);
        }
        readWriteFlip = false;
        bufferReadCursor = 0;
    }
    /*If the read and write cursors have not flipped, and no data has been read yet, read everything
    from the read cursor to the buffer write buffer (everything that has has been written since the 
    last read all.
    
    If I wanted to get fancy we could read all of this too, so if the write cursor looped and wrote 
    more data the above part would get the data left and this would add to it. But we have the 
    length == 0 so that if something was already written we say it's good enough.*/
    if(!readWriteFlip && length == 0) {
        length = bufferWriteCursor - bufferReadCursor;
        data = new unsigned char[length+1];
        memcpy(data, &buffer[bufferReadCursor], sizeof(char)*length);
        bufferReadCursor += length;
    }
    data[length] = '\0';
    //std::cout << data;
    return data;
}

char SerialPort::ReadChar() {
    if (bufferReadCursor == bufferDataLength) {
        bufferReadCursor = 0;
        readWriteFlip = true;
    }
    char c = 0;
    //if read cursor is behind the write cursor and the index of the read cursor is lower than the write cursor.
    //or read cursor is ahead of the write cursor and the index of the read cursor is higher than the write cursor.
    if (readWriteFlip ^ (bufferReadCursor < bufferWriteCursor)) {
        c = buffer[bufferReadCursor];
        ++bufferReadCursor;
    }
    std::cout << "character[" << bufferReadCursor << "] : " << c << std::endl;
    return c;
}

bool SerialPort::ReadToBuffer() {
    /*If read and write have flipped and the amount of data that will read into the buffer would make it overlap
    the read buffer, don't read anything, just return false*/
    if (readWriteFlip && bufferReadSizeB + bufferWriteCursor > bufferReadCursor) {
        bytesRead = 0;
        return false;
    }
    
    ReadFile(serialPort, &serialData, bufferReadSizeB, &bytesRead, NULL); 

    if (bytesRead == 0) {
        bytesRead = 0;
        return false;
    }

    /*check to make sure what is going to be written won't overflow the buffer. If it is going 
    to, set the buffer data length flag (so reading doesn't go past it), and reset the write 
    cursor to the beginning of the buffer.*/
    if (bytesRead + bufferWriteCursor > bufferSizeB) {
        bufferDataLength = bufferWriteCursor;
        bufferWriteCursor = 0;
        readWriteFlip = true;
    }

    //copy the data read from the serial port to the buffer at the correct offset.
    memcpy(&buffer[bufferWriteCursor], serialData, sizeof(char)*bytesRead);

    //increment the write cursor by the number of bytes read.
    bufferWriteCursor += bytesRead;
    
    return true;
}

SerialPort::SerialPort() :
    buffer(NULL),
    bufferDataLength(0),
    bufferReadCursor(0),
    bufferReadSizeB(128),
    bufferSizeB(0),
    bufferSizeKB(4),
    bufferWriteCursor(0),
    bytesRead(0),
    bytesWritten(0),
    error(false),
    readWriteFlip(false),
    serialPort(INVALID_HANDLE_VALUE),
    name("")
        {
    SetBufferSize(bufferSizeKB);
	std::cout << "New serial port" << std::endl;
}

SerialPort::~SerialPort() {
    //Close();
	std::cout << "Delete serial port" << std::endl;
}


void SerialPort::SetBufferSize(unsigned int bufferSizeKB) {
    delete buffer;
    ClearReadBuffer();
    this->bufferSizeKB = bufferSizeKB;
    bufferSizeB = bufferSizeKB * 1024;
    bufferDataLength = bufferSizeB;
    buffer = new unsigned char[bufferSizeB];
}

bool SerialPort::TryRead(unsigned char*& data, unsigned int length)
{
    //unsigned char* data = new unsigned char[length+1];
    unsigned int cursor = 0;
    unsigned int bufferLeft;
    unsigned int dataLeftBeforeEndOfBuffer;
    bool flip = false;
    if (readWriteFlip) {
        dataLeftBeforeEndOfBuffer = bufferDataLength - bufferReadCursor;
        bufferLeft = dataLeftBeforeEndOfBuffer + bufferWriteCursor;
        flip = dataLeftBeforeEndOfBuffer < length;
    } else {
        bufferLeft = bufferWriteCursor - bufferReadCursor;
    }

    //std::cout << "bufferleft" << bufferLeft << "\tlength " << length << "\trwFlip: " << readWriteFlip << "\tbufferWriteCursor:" 
     //   << bufferWriteCursor << "\tbufferReadCursor " << bufferReadCursor << std::endl;
    if (bufferLeft < length) {
        return false;
    }

    if (readWriteFlip && flip) {
        //write the first part of data from the end of the buffer
        memcpy(data, &buffer[bufferReadCursor], sizeof(char) * dataLeftBeforeEndOfBuffer);
        //write the second part of the data from the beginning of the buffer
        memcpy(&data[dataLeftBeforeEndOfBuffer], &buffer[bufferReadCursor], sizeof(char) * length-dataLeftBeforeEndOfBuffer);
        bufferReadCursor = length - dataLeftBeforeEndOfBuffer;
        readWriteFlip = false;
    } else{
        memcpy(data, &buffer[bufferReadCursor], sizeof(char) * length);
        bufferReadCursor += length;
    }
    data[length] = '\0';
    //std::cout << data;
}

bool SerialPort::Write(unsigned char* data) {

    //WriteFile(serialPort, (LPCVOID)&oneChar, 1, &dwBytesWritten, NULL);
    return false;
}

bool SerialPort::WriteChar(unsigned char c) {
    return false;
}
