#include "pch.h"
#include "Bit.h"

//most significant bit is at the lowest address
//for binary, that means the 0/1 is at bit 7,
//the 0/127 is at bit 0
bool Bit::bigEndian = true;
int Bit::bi = 0;

Bit::Bit()
{
}


Bit::~Bit()
{
}


bool Bit::Get(const char byte,const unsigned int index) {
	return ((byte >> index) & 0x01);
}

char Bit::Get(const char byte, const unsigned int bitStart, const unsigned int bitLength){

	char b = ((bitStart > 0 ? BITMASKL1 >> (bitStart-1) : 255) & byte) >> 8-(bitStart+bitLength);
	return b;
}

bool Bit::IsBigEndian() {
	return bigEndian;
}
bool Bit::IsBigEndian(const bool bigEndian) {
	return Bit::bigEndian = bigEndian;
}
bool Bit::IsLittleEndian() {
	return !bigEndian;
}
bool Bit::IsLittleEndian(const bool littleEndian) {
	return !(bigEndian = !littleEndian);
}


void Bit::Print(const char byte) {
	if (bigEndian) {
		bi = 8;
		while (bi-- > 0) {
			std::cout << Bit::Get(byte, bi);
		}
	}
	else {
		bi = -1;
		while (bi++ < 7) {
			std::cout << Bit::Get(byte, bi);
		}
	}

}

void Bit::Print(const std::string string) {
	unsigned int stringlength = string.length();
	for (unsigned int c = 0; c < stringlength; c++) {
		Print(string[c]); std::cout << std::endl;
	}
}
