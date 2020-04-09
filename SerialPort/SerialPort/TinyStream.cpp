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
	dataFlowDirection(DATAFLOWDIRECTION_READ),
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
	unsigned char* d;
	switch(state){
	//0 uninitialized
	case 0:
		//send out request to pair. 1 byte at a time
		IO->WriteChar(16);
		//try to read back data, see if anything is returned
		d = IO->ReadAll();
		//std::cout << "[TEST]" << d;
		break;
	case 10:
		break;

	default:
		break;
	}

	return false;
}
