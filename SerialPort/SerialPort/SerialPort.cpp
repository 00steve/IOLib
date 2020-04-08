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
    unsigned char* data;
    unsigned int length;
    if (readWriteFlip) {
        data = new unsigned char[1];
        length = 1;
        bufferReadCursor = 0;
    }
    else {
        length = bufferWriteCursor - bufferReadCursor;
        data = new unsigned char[length+1];
        memcpy(data, &buffer[bufferReadCursor], sizeof(char)*length);
        bufferReadCursor += length;
    }
    data[length - 1] = '\0';
    return data;
}

char SerialPort::ReadChar() {
    if (bufferReadCursor == bufferDataLength) {
        bufferReadCursor = 0;
        readWriteFlip = false;
    }

    char c = 0;
    //if read cursor is behind the write cursor and the index of the read cursor is lower than the write cursor.
    //or read cursor is ahead of the write cursor and the index of the read cursor is higher than the write cursor.
    if (readWriteFlip ^ (bufferReadCursor < bufferWriteCursor)) {
        c = buffer[bufferReadCursor];

        ++bufferReadCursor;
    }

    //if(bufferReadCursor < bufferWriteCursor && )
    std::cout << "character[" << bufferReadCursor << "] : " << c << std::endl;
    return c;
}

bool SerialPort::ReadToBuffer() {

    ReadFile(serialPort, &serialData, bufferReadSizeB, &bytesRead, NULL); // what I tried to do, just outputs white space
    if (bytesRead > 0) {
        //data[bytesRead-1] = '\0';

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
        //output some text for the chumps.
        std::cout << bytesRead << " data: " << buffer << std::endl;
    }
    return bytesRead > 0;
}

SerialPort::SerialPort() :
    buffer(NULL),
    bufferDataLength(0),
    bufferReadCursor(0),
    bufferReadSizeB(128),
    bufferSizeB(1024),
    bufferSizeKB(1),
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
    buffer = new unsigned char[bufferSizeB];
}

bool SerialPort::Write(unsigned char* data) {

    //WriteFile(serialPort, (LPCVOID)&oneChar, 1, &dwBytesWritten, NULL);
    return false;
}

bool SerialPort::WriteChar(unsigned char c) {
    return false;
}
