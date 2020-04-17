#pragma once
#include <iostream>
//#include "stdafx.h"

enum {
	BITMASKL1 = 127,
	BITMASKL2 = 63
};

class Bit{

private:
	static bool bigEndian;
	static int bi; //bit iterator
public:
	Bit();
	~Bit();


	static bool Get(const char byte, const unsigned int index);
	static char Get(const char byte, const unsigned int bitStart, const unsigned int bitLength);

	static bool IsBigEndian();
	static bool IsBigEndian(bool bigEndian);
	static bool IsLittleEndian();
	static bool IsLittleEndian(bool littleEndian);

	static void Print(const char byte);
	static void Print(const std::string string);
};

