#include "pch.h"
#include "TinyStream.h"

void TinyStream::SetDataFlowDirection(const unsigned char dataFlowDirection){

}

bool TinyStream::SetInput(IIO* IO)
{
	if (IO == nullptr) return false;
	this->IO = IO;

	return true;
}

TinyStream::TinyStream() : 
	dataFlowDirection(DATADIRECTION_READ),
	IO(nullptr),
	state(0)
		{

}


TinyStream::~TinyStream(){

}

/*
The different states:
0 = no paired device
10 = received reply from potential paired device
20 =
*/
bool TinyStream::Update(){
	unsigned char* data = new unsigned char[16]();
	unsigned int bytesRead;
	switch(state){
	//0 uninitialized
	case 0:
		//send out request to pair. 1 byte at a time
		IO->WriteChar(16);
		//try to read back data, see if anything is returned
		//d = IO->ReadAll();
		bytesRead = 0;
		while (IO->TryRead(data, 1)) {
			++bytesRead;
			std::cout << data[0];
		}
		std::cout << " bytes read[" << bytesRead << "]" << std::endl;

		//std::cout << "[TEST]" << d;
		break;
	case 10:
		break;

	default:
		break;
	}
	delete[] data;
	return false;
}
